#include <filesystem>
#include <fstream>
#include <meta>
#include <nlohmann/json.hpp>
#include <print>

struct TwoFloats {
  float x{1.f};
  double y{2.f};
};

struct Simple {
  char a{'a'};
  bool b{true};
  double c{1.1};
  int d{1};
  float e{1.5f};
  std::string f{"Hello, Reflection!"};
  TwoFloats myTwoFloats{};
};

template <typename T>
concept Reflectable = std::is_class_v<T> && std::is_aggregate_v<T>;

// Serialize
template <typename T> nlohmann::json StructToJson(const T &obj) {
  nlohmann::json j;
  constexpr auto ctx{std::meta::access_context::current()};
  constexpr auto members{
      define_static_array(nonstatic_data_members_of(^^T, ctx))};

  template for (constexpr auto member : members) {
    constexpr auto name{identifier_of(member)};

    if constexpr (Reflectable<typename[:type_of(member):]>) {
      j[std::string(name)] = StructToJson(obj.[:member:]);
    } else {
      j[std::string(name)] = obj.[:member:];
    }
  }
  return j;
}

// Deserialize
template <typename T> void JsonToStruct(const nlohmann::json &j, T &obj) {
  constexpr auto ctx{std::meta::access_context::current()};

  template for (constexpr auto member :
                define_static_array(nonstatic_data_members_of(^^T, ctx))) {
    constexpr auto name{identifier_of(member)};
    std::string key{name};

    if (j.contains(key)) {
      using MemberType = typename[:type_of(member):];

      if constexpr (Reflectable<MemberType>) {
        JsonToStruct(j[key], obj.[:member:]);
      } else {
        obj.[:member:] = j[key].get<MemberType>();
      }
    }
  }
}

int main() {
  Simple s;

  if (!std::filesystem::exists("input.json")) {
    std::ofstream create{"input.json"};
    create << StructToJson(s).dump(2);
    std::println("Created input.json with defaults");
  }

  std::ifstream in{"input.json"};
  nlohmann::json j(nlohmann::json::parse(in));
  JsonToStruct(j, s);

  std::ofstream out{"output.json"};
  out << StructToJson(s).dump(2);
}