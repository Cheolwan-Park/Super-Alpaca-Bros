#ifndef BaseTypes_hpp
#define BaseTypes_hpp

#include <stdio.h>
#include <assert.h>

namespace Base {
namespace Directories {
constexpr char root[] = "./";
constexpr char res[] = "./res/";
constexpr char Shader[] = "./res/shader/";
constexpr char Scene[] = "./res/scene/";
constexpr char GameObject[] = "./res/gameobject/";
constexpr char Texture[] = "./res/texture/";
constexpr char Sound[] = "./res/sound/";
}
//namespace Directories {
//constexpr char root[] = "/Users/cheolwanpark/Documents/Projects/Super-Alpaca-Bros";
//constexpr char res[] = "/Users/cheolwanpark/Documents/Projects/Super-Alpaca-Bros/res/";
//constexpr char Shader[] = "/Users/cheolwanpark/Documents/Projects/Super-Alpaca-Bros/res/shader/";
//constexpr char Scene[] = "/Users/cheolwanpark/Documents/Projects/Super-Alpaca-Bros/res/scene/";
//constexpr char GameObject[] = "/Users/cheolwanpark/Documents/Projects/Super-Alpaca-Bros/res/gameobject/";
//constexpr char Texture[] = "/Users/cheolwanpark/Documents/Projects/Super-Alpaca-Bros/res/texture/";
//constexpr char Sound[] = "/Users/cheolwanpark/Documents/Projects/Super-Alpaca-Bros/res/sound/";
//}

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

const uint32_t LOG_MAXLEVEL = LOG_ALL;
const uint32_t ASSERT_LEVEL = LOG_NONE;

}

#endif /* type_defs_h */
