#ifndef BaseMath_hpp
#define BaseMath_hpp

#include "types.hpp"
#include "Vector.hpp"

namespace Base
{
    namespace Math
    {
        struct Rect
        {
            float32 x;
            float32 y;
            float32 w;
            float32 h;
        };
        
        struct IRect
        {
            int32 x;
            int32 y;
            int32 w;
            int32 h;
        };
        
        namespace Constant
        {
            constexpr float32 PIf        = 3.141592f;
            constexpr float64 PI         = 3.141592;
            constexpr float32 DEG2RADf   = PIf/180.0f;
            constexpr float64 DEG2RAD    = PI/180.0;
            constexpr float32 RAD2DEGf   = 180.0f/PIf;
            constexpr float64 RAD2DEG    = 180.0/PI;
        }
        
        // All Triangular function return Radian value and recieve Radian value
        float32 Sin(float32 angle);
        
        float64 Sin(float64 angle);
        
        float32 Cos(float32 angle);
        
        float64 Cos(float64 angle);
        
        float32 Tan(float32 angle);
        
        float64 Tan(float64 angle);
        
        float32 ASin(float32 value);
        
        float64 ASin(float64 value);
        
        float32 ACos(float32 value);
        
        float64 ACos(float64 value);
        
        float32 ATan(float32 value);
        
        float64 ATan(float64 value);
        
        float32 ATan2(float32 x, float32 y);
        
        float64 ATan2(float64 x, float64 y);
        
        float32 Abs(float32 value);
        
        float64 Abs(float64 value);
        
        int32 Abs(int32 value);
        
        int64 Abs(int64 value);
        
        float32 Deg2Radf(float32 angle);
        
        float64 Deg2Rad(float64 angle);
        
        float32 Rad2Degf(float32 angle);
        
        float64 Rad2Deg(float64 angle);
        
        float32 Square(float32 value);
        
        float64 Square(float64 value);
        
        float32 Pow(float32 n, float32 exponent);
        
        float64 Pow(float64 n, float64 exponent);
        
        float32 Sqrt(float32 value);
        
        float64 Sqrt(float64 value);
    }
}

#endif
