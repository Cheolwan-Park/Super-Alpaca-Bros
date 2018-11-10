#include "Ground.hpp"

namespace Game {
Ground::Ground()
    : BoxCollider() {
  ;
}

Ground::Ground(const Ground &other)
    : BoxCollider(other) {
  ;
}

Ground::~Ground() {
  ;
}

Ground &Ground::operator=(const Ground &other) {
  assert(this != &other);
  BoxCollider::operator=(other);
  return (*this);
}

void Ground::initWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator) {
  BoxCollider::initWithJson(obj, allocator);

  assert(obj.HasMember("ignorelist_size"));
  assert(obj.HasMember("isPassable"));
  assert(obj["isPassable"].IsBool());
  assert(obj["ignorelist_size"].IsUint());

  m_ignore_list_size = obj["ignorelist_size"].GetUint();
  m_ignore_list = allocator.alloc<Collider *>(m_ignore_list_size);
  memset(m_ignore_list, 0, sizeof(Collider *) * m_ignore_list_size);
  setPassable(obj["isPassable"].GetBool());
}

void Ground::onTriggerStay(Collider *other) {
  if (other->getTag() != "player"_hash) {
    return;
  }

  for (Uint32 i = 0; i < m_ignore_list_size; ++i) {
    if (other == m_ignore_list[i])
      return;
  }

  glm::vec3 otherpos(0.0f), thispos(0.0f);
  other->getWorldPosition(&otherpos);
  this->getWorldPosition(&thispos);

  float32 distance = getBox().h / 2.0f;
  switch (other->getType()) {
    case Collider::Type::BOX: {
      distance += ((BoxCollider *) other)->getBox().h / 2.0f;
      break;
    }
    case Collider::Type::CIRCLE: {
      distance += ((CircleCollider *) other)->getRadius();
      break;
    }
    default: {
      fprintf(stderr, "invalid collider type\n");
      exit(-1);
    }
  }

  if (otherpos.y > thispos.y && otherpos.y < (thispos.y + distance)) {
    if (other->getTag() != "player"_hash) {
      return;
    }

    Rigidbody *other_rigidbody = other->getRigidbody();
    if (other_rigidbody) {
      const glm::vec3 &v = other_rigidbody->getVelocity();
      if (v.y >= 0.0f) {
        return;
      }
      other_rigidbody->setVelocity(v.x, 0.0f, v.z);
    }

    otherpos.y = thispos.y + distance - 0.05f;
    other->setWorldPosition(otherpos);
  }
}

void Ground::onTriggerExit(Collider *other) {
  for (Uint32 i = 0; i < m_ignore_list_size; ++i) {
    if (m_ignore_list[i] == other) {
      m_ignore_list[i] = nullptr;
      break;
    }
  }
}

void Ground::pass(GameObject *obj) {
  assert(obj);

  if (isPassable()) {
    Collider *collider = obj->getComponent<BoxCollider>();
    if (!collider)
      collider = obj->getComponent<CircleCollider>();
    if (!collider)
      return;
    for (Uint32 i = 0; i < m_ignore_list_size; ++i) {
      if (!m_ignore_list[i]) {
        m_ignore_list[i] = collider;
        break;
      }
    }
  }
}

int32 Ground::isPassable() const {
  return m_flags.getFlag(3);
}

void Ground::setPassable(int32 val) {
  m_flags.setFlag(3, val);
}
}