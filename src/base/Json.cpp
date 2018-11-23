#include "Json.hpp"

namespace Base {
namespace JsonParseMethods {
void ReadVector(const rapidjson::Value::Object &obj, glm::vec3 *vec) {
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

void ReadVector2(const rapidjson::Value::Object &obj, glm::vec2 *vec) {
  assert(vec);

  assert(obj.HasMember("x"));
  assert(obj.HasMember("y"));
  assert(obj["x"].IsFloat());
  assert(obj["y"].IsFloat());

  vec->x = obj["x"].GetFloat();
  vec->y = obj["y"].GetFloat();
}

void ReadRect(const rapidjson::Value::Object &obj, Math::Rect *rect) {
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

void ReadIRect(const rapidjson::Value::Object &obj, Math::IRect *rect) {
  assert(rect);

  assert(obj.HasMember("x"));
  assert(obj.HasMember("y"));
  assert(obj.HasMember("w"));
  assert(obj.HasMember("h"));
  assert(obj["x"].IsInt());
  assert(obj["y"].IsInt());
  assert(obj["w"].IsInt());
  assert(obj["h"].IsInt());

  rect->x = obj["x"].GetInt();
  rect->y = obj["y"].GetInt();
  rect->w = obj["w"].GetInt();
  rect->h = obj["h"].GetInt();
}

void ReadSize(const rapidjson::Value::Object &obj, Math::Size *size) {
  assert(size);

  assert(obj.HasMember("w"));
  assert(obj.HasMember("h"));
  assert(obj["w"].IsInt());
  assert(obj["h"].IsInt());

  size->w = obj["w"].GetInt();
  size->h = obj["h"].GetInt();
}

void ReadPoint(const rapidjson::Value::Object &obj, Math::Point *point) {
  assert(point);

  assert(obj.HasMember("x"));
  assert(obj.HasMember("y"));
  assert(obj["x"].IsInt());
  assert(obj["y"].IsInt());

  point->x = obj["x"].GetInt();
  point->y = obj["y"].GetInt();
}
}

void OpenJsonFile(const FileIO &f, rapidjson::Document *doc) {
  assert(doc);
  doc->Parse(f.getBuffer());
  assert(doc->IsObject());
}

int32_t OpenJsonFile(const char *filename, rapidjson::Document *doc) {
  assert(doc);

  FILE *f = OpenFile(filename, "rb");
  assert(f);

  int64_t len_json = GetFileSize(f);
  void *mem_json = nullptr;

  FileIO json_io;
  mem_json = malloc(len_json + 1);
  memset(mem_json, 0, len_json + 1);
  if (RET_SUCC != json_io.open(f, mem_json, len_json)) {
    fprintf(stderr, "failed to read %s\n", filename);
    fclose(f);
    free(mem_json);
    return RET_FAILED;
  }

  doc->Parse(json_io.getBuffer());
  assert(doc->IsObject());
  fclose(f);
  free(mem_json);
  return RET_SUCC;
}
}