#include "memory/object_mark.hpp"
#include "memory/gc.hpp"

#include "object_utils.hpp"
#include "memory.hpp"

#include "vm.hpp"
#include "state.hpp"

namespace rubinius {
namespace memory {
  VM* ObjectMark::vm() {
    return gc->vm();
  }

  /**
   * Checks the reference to the target obj, and if it is valid, notifies the GC
   * that the object is still alive.
   *
   * /param obj The object reference to be marked as still reachable/alive.
   * /returns the location of the object after GC has processed the object,
   *          which may involve moving it.
   */
  Object* ObjectMark::call(Object* obj) {
    if(!obj->reference_p()) return NULL;
#ifdef RBX_GC_DEBUG
    if(unlikely(obj->zone() == UnspecifiedZone)) {
      rubinius::bug("Unspecified zone for object");
    }
#endif
    return gc->saw_object(0, obj);
  }

  /**
   * Updates the reference pos in target with the new location of val, and
   * then runs the write-barrier.
   *
   * /param target The object containing the reference to val.
   * /param pos    A pointer to the location in target that references val.
   * /param val    The object being referenced.
   */
  void ObjectMark::set(Object* target, Object** pos, Object* val) {
    *pos = val;
    if(val->reference_p()) {
      gc->memory_->write_barrier(target, val);
    }
  }

  void ObjectMark::set_value(Object* target, Object** pos, Object* val) {
    *reinterpret_cast<VALUE*>(pos) = MemoryHandle::value(val);
    if(val->reference_p()) {
      gc->memory_->write_barrier(target, val);
    }
  }

  /**
   * Runs the write-barrier for the reference from target to val.
   *
   * /param target The object containing the reference to val.
   * /param val    The object being referenced.
   */
  void ObjectMark::just_set(Object* target, Object* val) {
    if(val->reference_p()) {
      gc->memory_->write_barrier(target, val);
    }
  }
}
}
