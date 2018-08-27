#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include "Math.hpp"

namespace Base
{
    namespace JsonParseMethods
    {
        void ReadVector(const rapidjson::GenericObject<true, rapidjson::Value> &obj, glm::vec3 *vec);
        void ReadVector2(const rapidjson::GenericObject<true, rapidjson::Value> &obj, glm::vec2 *vec);
        void ReadRect(const rapidjson::GenericObject<true, rapidjson::Value> &obj, Math::Rect *rect);
        void ReadIRect(const rapidjson::GenericObject<true, rapidjson::Value> &obj, Math::IRect *irect);
        void ReadSize(const rapidjson::GenericObject<true, rapidjson::Value> &obj, Math::Size *size);
        void ReadPoint(const rapidjson::GenericObject<true, rapidjson::Value> &obj, Math::Point *point);
    }
}