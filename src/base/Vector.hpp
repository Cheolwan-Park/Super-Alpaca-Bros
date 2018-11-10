#ifndef BaseVector_hpp
#define BaseVector_hpp

#include "types.hpp"

namespace Base {
namespace Math {
class Vector2 {
 public:
  Vector2();

  Vector2(float32 x, float32 y);

  Vector2(const Vector2 &other);

  ~Vector2();

  void rotate(float32 angle);

  void normalize();

  Vector2 getRotated(float32 angle) const;

  Vector2 getNormalized() const;

  Vector2 &operator=(const Vector2 &other);

  Vector2 &operator+=(const Vector2 &other);

  Vector2 &operator-=(const Vector2 &other);

  Vector2 &operator*=(float32 scala);

  Vector2 &operator/=(float32 scala);

  friend int32 operator==(const Vector2 &lhs, const Vector2 &rhs);

  friend int32 operator!=(const Vector2 &lhs, const Vector2 &rhs);

  friend Vector2 operator+(Vector2 lhs, const Vector2 &rhs);

  friend Vector2 operator-(Vector2 lhs, const Vector2 &rhs);

  friend Vector2 operator*(Vector2 vec, float32 scala);

  friend float operator*(const Vector2 &lhs, const Vector2 &rhs);

  friend Vector2 operator/(Vector2 vec, float32 scala);

  // members
  float32 x;
  float32 y;
};

int32 operator==(const Vector2 &lhs, const Vector2 &rhs);

int32 operator!=(const Vector2 &lhs, const Vector2 &rhs);

Vector2 operator+(Vector2 lhs, const Vector2 &rhs);

Vector2 operator-(Vector2 lhs, const Vector2 &rhs);

Vector2 operator*(Vector2 vec, float32 scala);

Vector2 operator*(Vector2 lhs, const Vector2 &rhs);

Vector2 operator/(Vector2 vec, float32 scala);

}
}

#endif


