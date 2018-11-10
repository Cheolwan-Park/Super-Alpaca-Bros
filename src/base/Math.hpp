#ifndef BaseMath_hpp
#define BaseMath_hpp

#include "types.hpp"
#include "Vector.hpp"
#include <math.h>

namespace Base {
namespace Math {
struct Rect {
  float32 x;
  float32 y;
  float32 w;
  float32 h;
};

struct IRect {
  int32 x;
  int32 y;
  int32 w;
  int32 h;
};

struct Size {
  int32 w;
  int32 h;
};

struct Point {
  int32 x;
  int32 y;
};

namespace Constant {
constexpr float32 PIf = 3.141592f;
constexpr float64 PI = 3.141592;
constexpr float32 DEG2RADf = PIf / 180.0f;
constexpr float64 DEG2RAD = PI / 180.0;
constexpr float32 RAD2DEGf = 180.0f / PIf;
constexpr float64 RAD2DEG = 180.0 / PI;
}
}
}

#endif
