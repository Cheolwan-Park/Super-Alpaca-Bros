#ifndef GameAlpacaAction_hpp
#define GameAlpacaAction_hpp

#include "base.hpp"
#include "Alpaca.hpp"

namespace Game {
namespace Alpaca {
#define ACTION(__TYPENAME__)    \
        MAKE_TYPE_ID(__TYPENAME__);         \
        Uint32 getTypeID()const override { return ID; }

class Action {
 public:
  typedef ActionManager::ActionType Type;
  typedef Action *(FactoryFunc)(const ::rapidjson::Value::Object &, ::Base::StackAllocator &);
 public:
  Action();

  // not copying alpaca pointer
  Action(const Action &other);

  virtual ~Action();

  // not copying alpaca pointer
  Action &operator=(const Action &other);

  virtual void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

  virtual void update() = 0;

  virtual void act() = 0;

  virtual Type getType() const = 0;

  virtual int32_t isActing() const = 0;

  virtual uint32_t getTypeID() const = 0;

  void setAlpaca(Alpaca *alpaca);

  Alpaca *getAlpaca();

  float32 getCooltime() const;

 private:
  float32 m_cooltime;
  Alpaca *m_alpaca;
};

template<class T>
Action *ActionDefaultFactory(const ::rapidjson::Value::Object &obj, ::Base::StackAllocator &allocator) {
  Action *result = new(allocator.alloc<T>()) T();
  assert(result);
  result->initWithJson(obj, allocator);
  return result;
}

class Heading : public Action {
 public:
  ACTION(Heading);

  Heading();

  // copying time, length
  Heading(const Heading &other);

  ~Heading() override;

  // copying time, length
  Heading &operator=(const Heading &other);

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void update() override;

  void act() override;

  Action::Type getType() const override;

  int32_t isActing() const override;

  void stopHeading();

  void hooking();

  void stopHooking();

  // get
  int32_t isHeading() const;

  int32_t isHeadHooking() const;

 private:
  void SetHeading(int32_t val);

  void SetHooking(int32_t val);

 private:
  float32 m_time;
  float32 m_length;
  float32 m_elapsed_time;
  glm::vec3 m_heading_target;
  glm::vec3 m_hooking_target;
  glm::vec3 m_hooking_start;
  BitFlag m_flags;
  /*
   * 0 : heading
   * 1 : hooking
   */
};

class Dash : public Action {
 public:
  ACTION(Dash);

  Dash();

  // copying length , time
  Dash(const Dash &other);

  ~Dash() override;

  // copying length , time
  Dash &operator=(const Dash &other);

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void update() override;

  void act() override;

  Action::Type getType() const override;

  int32_t isActing() const override;

 private:
  float32 m_force;
};

class Spit;
class Spitting : public Action {
 public:
  ACTION(Spitting);

  Spitting();

  // copying length , time
  Spitting(const Spitting &other) = delete;

  ~Spitting() override;

  // copying length , time
  Spitting &operator=(const Spitting &other) = delete;

  void initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) override;

  void update() override;

  void act() override;

  Action::Type getType() const override;

  int32_t isActing() const override;

 private:
  Spit *GetSpit();

 private:
  uint32_t m_circular_spits;
  uint32_t m_max_spits;
  GameObject **m_spit_objects;
  Spit **m_spits;
};

class ActionFactory : public Storage<Action::FactoryFunc> {
 public:

  ActionFactory() : Storage<Action::FactoryFunc>() { ; }

  ActionFactory(const ActionFactory &other) = delete;

  ~ActionFactory() { clear(); }

  ActionFactory &operator=(const ActionFactory) = delete;

  template<typename T>
  void addFunction() {
    add(&ActionDefaultFactory<T>, T::ID);
  }

  template<typename T>
  Action::FactoryFunc *getFunction() {
    return (get(T::ID));
  }

  Action::FactoryFunc *getFunction(uint32_t id) {
    return (get(id));
  }

  static ActionFactory &getGlobal() {
    static ActionFactory instance;
    return instance;
  }
};
}
}

#endif