#ifndef BaseTypes_hpp
#define BaseTypes_hpp

#include <stdio.h>
#include <assert.h>

namespace Base {
namespace Directories {
constexpr char root[] = "/Users/cheolwanpark/Documents/Projects/Alpacas'_Head/";
constexpr char res[] = "/Users/cheolwanpark/Documents/Projects/Alpacas'_Head/res/";
constexpr char Shader[] = "/Users/cheolwanpark/Documents/Projects/Alpacas'_Head/res/shader/";
constexpr char Scene[] = "/Users/cheolwanpark/Documents/Projects/Alpacas'_Head/res/scene/";
constexpr char GameObject[] = "/Users/cheolwanpark/Documents/Projects/Alpacas'_Head/res/gameobject/";
constexpr char Texture[] = "/Users/cheolwanpark/Documents/Projects/Alpacas'_Head/res/texture/";
}

typedef char byte;
typedef unsigned char Uint8;
typedef unsigned short Uint16;
typedef unsigned int Uint32;
typedef unsigned long Uint64;
typedef char int8;
typedef short int16;
typedef int int32;
typedef long int64;
typedef float float32;
typedef double float64;

enum {
  RET_FAILED = 0,
  RET_SUCC = 1
};

enum {
  LOG_NONE = 0,
  LOG_ERROR = 10,
  LOG_BASIC = 20,
  LOG_WARNING = 30,
  LOG_INFO = 40,
  LOG_ALL = 1000
};

const Uint32 LOG_MAXLEVEL = LOG_ALL;
const Uint32 ASSERT_LEVEL = LOG_NONE;

}

#endif /* type_defs_h */
