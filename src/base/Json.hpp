#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include "Math.hpp"
#include "FileIO.hpp"

namespace Base
{
    namespace JsonParseMethods
    {
        void ReadVector(const rapidjson::Value::Object &obj, glm::vec3 *vec);
        void ReadVector2(const rapidjson::Value::Object &obj, glm::vec2 *vec);
        void ReadRect(const rapidjson::Value::Object &obj, Math::Rect *rect);
        void ReadIRect(const rapidjson::Value::Object &obj, Math::IRect *irect);
        void ReadSize(const rapidjson::Value::Object &obj, Math::Size *size);
        void ReadPoint(const rapidjson::Value::Object &obj, Math::Point *point);
    }
    
    void OpenJsonFile(const FileIO &f, rapidjson::Document *doc);
    int32 OpenJsonFile(const char *filename, rapidjson::Document *doc);
}