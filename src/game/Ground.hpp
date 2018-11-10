#ifndef GameGround_hpp
#define GameGround_hpp

#include "base.hpp"

using namespace Base;

namespace Game {
class Ground : public BoxCollider {

 public:
  COMPONENT(Ground);

  Ground();

  Ground(const Ground &other);

  ~Ground() override;

  Ground &operator=(const Ground &other);

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void onTriggerStay(Collider *other) override;

  void onTriggerExit(Collider *other) override;

  void pass(GameObject *obj);

  int32 isPassable() const;

  void setPassable(int32 val);

 private:
  Uint32 m_ignore_list_size;
  Collider **m_ignore_list;
  /*
   * m_flags
   * BoxCollider's flags
   * 3 : isPassable
   */
};
}

#endif