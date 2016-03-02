#ifndef RBX_BUILTIN_LIST_HPP
#define RBX_BUILTIN_LIST_HPP

#include "object_utils.hpp"

#include "builtin/fixnum.hpp"
#include "builtin/object.hpp"

namespace rubinius {

  class ListNode : public Object {
  public:
    const static object_type type = ListNodeType;

  private:
    Object* object_;  // slot
    ListNode* next_; // slot

  public:
    /* accessors */

    attr_accessor(object, Object);
    attr_accessor(next, ListNode);

    /* interface */
    static void initialize(STATE, ListNode* obj) {
      obj->object_ = nil<Object>();
      obj->next_ = nil<ListNode>();
    }

    class Info : public TypeInfo {
    public:
      BASIC_TYPEINFO(TypeInfo)
    };

    friend class Info;

  };

  class List : public Object {
  public:
    const static object_type type = ListType;

  private:
    Fixnum* count_;   // slot
    ListNode* first_; // slot
    ListNode* last_;  // slot

  public:
    /* accessors */

    attr_accessor(count, Fixnum);
    attr_accessor(first, ListNode);
    attr_accessor(last, ListNode);

    /* interface */
    static void initialize(STATE, List* obj) {
      obj->count_ = Fixnum::from(0);
      obj->first_ = nil<ListNode>();
      obj->last_ = nil<ListNode>();
    }

    bool empty_p();
    size_t size();
    static void bootstrap(STATE);
    static List* create(STATE);
    void append(STATE, Object* obj);
    List* clear(STATE);
    Object* shift(STATE);
    Object* locate(STATE, size_t index);
    size_t remove(STATE, const Object* obj);

    class Info : public TypeInfo {
    public:
      BASIC_TYPEINFO(TypeInfo)
    };

    friend class Info;

  };

};
#endif