#include "Vector.hpp"
#include "Math.hpp"

namespace Base {
namespace Math {
// Vector2 class
Vector2::Vector2()
    : x(0), y(0) {
  ;
}

Vector2::Vector2(float32 x, float32 y)
    : x(x), y(y) {
  ;
}

Vector2::Vector2(const Vector2 &other)
    : x(other.x), y(other.y) {
  ;
}

Vector2::~Vector2() {
  ;
}

void Vector2::rotate(float32 angle) {
  float32 cos = cosf(angle), sin = sinf(angle);
  float32 new_x = 0.0f, new_y = 0.0f;
  new_x = x * cos - y * sin;
  new_y = x * sin + y * cos;
  x = new_x;
  y = new_y;
}

void Vector2::normalize() {
  float32 length = sqrtf(x * x + y * y);
  x /= length;
  y /= length;
}

Vector2 Vector2::getRotated(float32 angle) const {
  float32 cos = cosf(angle), sin = sinf(angle);
  return Vector2(this->x * cos - this->y * sin,
                 this->x * sin + this->y * cos);
}

Vector2 Vector2::getNormalized() const {
  Vector2 result(0.0f, 0.0f);
  float length = sqrtf(x * x + y * y);
  return Vector2(this->x / length, this->y / length);
}

Vector2 &Vector2::operator=(const Vector2 &other) {
  assert(this != &other);
  this->x = other.x;
  this->y = other.y;
  return (*this);
}

Vector2 &Vector2::operator+=(const Vector2 &other) {
  assert(this != &other);
  this->x += other.x;
  this->y += other.y;
  return (*this);
}

Vector2 &Vector2::operator-=(const Vector2 &other) {
  assert(this != &other);
  this->x -= other.x;
  this->y -= other.y;
  return (*this);
}

Vector2 &Vector2::operator*=(float32 scala) {
  x *= scala;
  y *= scala;
  return (*this);
}

Vector2 &Vector2::operator/=(float32 scala) {
  x /= scala;
  y /= scala;
  return (*this);
}

int32 operator==(const Vector2 &lhs, const Vector2 &rhs) {
  return (lhs.x == rhs.x && lhs.y == rhs.y);
}

int32 operator!=(const Vector2 &lhs, const Vector2 &rhs) {
  return (lhs.x != rhs.x || lhs.y != rhs.y);
}

Vector2 operator+(Vector2 lhs, const Vector2 &rhs) {
  lhs.x += rhs.x;
  lhs.y += rhs.y;
  return lhs;
}

Vector2 operator-(Vector2 lhs, const Vector2 &rhs) {
  lhs.x -= rhs.x;
  lhs.y -= rhs.y;
  return lhs;
}

Vector2 operator*(Vector2 lhs, float32 scala) {
  lhs.x *= scala;
  lhs.y *= scala;
  return lhs;
}

float operator*(const Vector2 &lhs, const Vector2 &rhs) {
  return (lhs.x * rhs.x + lhs.y * rhs.y);
}

Vector2 operator/(Vector2 lhs, float32 scala) {
  lhs.x /= scala;
  lhs.y /= scala;
  return lhs;
}
}
}
