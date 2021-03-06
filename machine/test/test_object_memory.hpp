#include "machine/test/test.hpp"

#include <iostream>

#include "memory/gc.hpp"
#include "memory/root.hpp"
#include "memory/mark_sweep.hpp"

#include "object_utils.hpp"

#include "class/array.hpp"

#include <cxxtest/TestSuite.h>

using namespace rubinius;

class TestMemory : public CxxTest::TestSuite, public VMTest {
public:

  memory::GCData* gc_data;
  memory::Roots* roots;
  memory::VariableRootBuffers variable_buffers;

  void setUp() {
    create();
    roots = &state->globals().roots;
    gc_data = new memory::GCData(state->vm());
  }

  void tearDown() {
    if(gc_data) {
      delete gc_data;
    }
    destroy();
  }

  Tuple* util_new_object(Memory &om, size_t count = 3) {
    return Tuple::create(state, count);
  }

  /* Causes a segfault when fails. */
  /* TODO: MemoryHeader
  void xtest_write_barrier_not_called_for_immediates() {
    Memory& om = *state->memory();
    Object* obj;
    Object* obj2;

    obj = util_new_object(om);
    obj->set_zone(MatureObjectZone);

    obj2 = cNil;

    om.write_barrier(obj, obj2);
    TS_ASSERT_EQUALS(obj->remembered_p(), 0U);
  }
  */

#define LARGE_OBJECT_BYTE_SIZE   30 * 1024 * 1024

  void test_new_large_object() {
    Memory& om = *state->memory();
    Tuple* obj;

    obj = util_new_object(om, LARGE_OBJECT_BYTE_SIZE);
    TS_ASSERT_EQUALS(obj->num_fields(), LARGE_OBJECT_BYTE_SIZE);
    TS_ASSERT_EQUALS(obj->region(), eLargeRegion);
  }

  void test_collect_full() {
    Memory& om = *state->memory();
    Object* mature;

    om.large_object_threshold = 10;

    mature = util_new_object(om, LARGE_OBJECT_BYTE_SIZE);

    TS_ASSERT_EQUALS(mature->region(), eLargeRegion);
    unsigned int mark = om.mark();
    TS_ASSERT(!mature->marked_p(mark));
    memory::Root r(roots, mature);

    om.collector()->collect(state);
    // marker thread cleans up gc_data
    gc_data = NULL;

    TS_ASSERT(mature->marked_p(mark));
  }

  void test_collect_full_marks_young_objects() {
    Memory& om = *state->memory();
    Tuple* young;
    Object* mature;

    young =  util_new_object(om);
    mature = util_new_object(om, LARGE_OBJECT_BYTE_SIZE);

    young->field[0] = mature; // dangerous, but ok in tests

    unsigned int mark = om.mark();
    TS_ASSERT(!young->marked_p(mark));
    memory::Root r(roots, young);

    om.collector()->collect(state);
    gc_data = NULL;

    TS_ASSERT(young->marked_p(mark));
  }

  /* Could segfault on failure due to infinite loop. */
  void test_collect_full_stops_at_already_marked_objects() {
    Memory& om = *state->memory();
    Tuple *young, *mature;

    om.large_object_threshold = 50;

    young =  as<Tuple>(util_new_object(om));
    mature = as<Tuple>(util_new_object(om, LARGE_OBJECT_BYTE_SIZE));

    young->field[0] = mature;
    mature->field[0] = young;

    om.write_barrier(young, mature);
    om.write_barrier(mature, young);

    memory::Root r(roots, young);

    om.collector()->collect(state);
    gc_data = NULL;

    mature = as<Tuple>(young->field[0]);

    TS_ASSERT_EQUALS(mature->field[0], young);
  }

  void test_valid_object_p() {
    Memory& om = *state->memory();
    Object* obj;

    obj = util_new_object(om);
    TS_ASSERT(om.valid_object_p(obj));
  }
};

