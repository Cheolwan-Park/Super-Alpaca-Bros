#include "Json.hpp"

namespace Base
{
    namespace JsonParseMethods
    {
        void ReadVector(const rapidjson::Value::Object &obj, glm::vec3 *vec)
        {
            assert(vec);

            assert(obj.HasMember("x"));
            assert(obj.HasMember("y"));
            assert(obj.HasMember("z"));
            assert(obj["x"].IsFloat());
            assert(obj["y"].IsFloat());
            assert(obj["z"].IsFloat());

            vec->x = obj["x"].GetFloat();
            vec->y = obj["y"].GetFloat();
            vec->z = obj["z"].GetFloat();
        }

        void ReadVector2(const rapidjson::Value::Object &obj, glm::vec2 *vec)
        {
            assert(vec);

            assert(obj.HasMember("x"));
            assert(obj.HasMember("y"));
            assert(obj["x"].IsFloat());
            assert(obj["y"].IsFloat());
            
            vec->x = obj["x"].GetFloat();
            vec->y = obj["y"].GetFloat();
        }

        void ReadRect(const rapidjson::Value::Object &obj, Math::Rect *rect)
        {
            assert(rect);

            assert(obj.HasMember("x"));
            assert(obj.HasMember("y"));
            assert(obj.HasMember("w"));
            assert(obj.HasMember("h"));
            assert(obj["x"].IsFloat());
            assert(obj["y"].IsFloat());
            assert(obj["w"].IsFloat());
            assert(obj["h"].IsFloat());

            rect->x = obj["x"].GetFloat();
            rect->y = obj["y"].GetFloat();
            rect->w = obj["w"].GetFloat();
            rect->h = obj["h"].GetFloat();
        }

        void ReadIRect(const rapidjson::Value::Object &obj, Math::IRect *irect)
        {
            assert(irect);

            assert(obj.HasMember("x"));
            assert(obj.HasMember("y"));
            assert(obj.HasMember("w"));
            assert(obj.HasMember("h"));
            assert(obj["x"].IsInt());
            assert(obj["y"].IsInt());
            assert(obj["w"].IsInt());
            assert(obj["h"].IsInt());

            irect->x = obj["x"].GetInt();
            irect->y = obj["y"].GetInt();
            irect->w = obj["w"].GetInt();
            irect->h = obj["h"].GetInt();
        }

        void ReadSize(const rapidjson::Value::Object &obj, Math::Size *size)
        {
            assert(size);

            assert(obj.HasMember("w"));
            assert(obj.HasMember("h"));
            assert(obj["w"].IsInt());
            assert(obj["h"].IsInt());

            size->w = obj["w"].GetInt();
            size->h = obj["h"].GetInt();
        }

        void ReadPoint(const rapidjson::Value::Object &obj, Math::Point *point)
        {
            assert(point);

            assert(obj.HasMember("x"));
            assert(obj.HasMember("y"));
            assert(obj["x"].IsInt());
            assert(obj["y"].IsInt());

            point->x = obj["x"].GetInt();
            point->y = obj["y"].GetInt();
        }
    }
}