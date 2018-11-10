#include "Math.hpp"
#include <math.h>

namespace Base {
namespace Math {
float32 Sin(float32 angle) {
  return sinf(angle);
}

float64 Sin(float64 angle) {
  return sin(angle);
}

float32 Cos(float32 angle) {
  return cosf(angle);
}

float64 Cos(float64 angle) {
  return cos(angle);
}

float32 Tan(float32 angle) {
  return tanf(angle);
}

float64 Tan(float64 angle) {
  return tan(angle);
}

float32 ASin(float32 value) {
  return asinf(value);
}

float64 ASin(float64 value) {
  return asin(value);
}

float32 ACos(float32 value) {
  return acosf(value);
}

float64 ACos(float64 value) {
  return acos(value);
}

float32 ATan(float32 value) {
  return atanf(value);
}

float64 ATan(float64 value) {
  return atan(value);
}

float32 ATan2(float32 x, float32 y) {
  return atan2f(x, y);
}

float64 ATan2(float64 x, float64 y) {
  return atan2(x, y);
}

float32 Abs(float32 value) {
  return value >= 0 ? value : -1 * value;
}

float64 Abs(float64 value) {
  return value >= 0 ? value : -1 * value;
}

int32 Abs(int32 value) {
  return value >= 0 ? value : -1 * value;
}

int64 Abs(int64 value) {
  return value >= 0 ? value : -1 * value;
}

float32 Deg2Radf(float32 angle) {
  return Constant::DEG2RADf * angle;
}

float64 Deg2Rad(float64 angle) {
  return Constant::DEG2RAD * angle;
}

float32 Rad2Degf(float32 angle) {
  return Constant::RAD2DEGf * angle;
}

float64 Rad2Deg(float64 angle) {
  return Constant::RAD2DEG * angle;
}

float32 Square(float32 value) {
  return value * value;
}

float64 Square(float64 value) {
  return value * value;
}

float32 Pow(float32 n, float32 exponent) {
  return powf(n, exponent);
}

float64 Pow(float64 n, float64 exponent) {
  return pow(n, exponent);
}

float32 Sqrt(float32 value) {
  return sqrtf(value);
}

float64 Sqrt(float64 value) {
  return sqrt(value);
}

float32 Log(float32 value) {
  return log(value);
}

float64 Log(float64 value) {
  return log(value);
}
}
}
