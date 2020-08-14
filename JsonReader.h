#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <optional>
#include <limits>

namespace JsonReading {

    enum class JsonType {
        JBOOLEAN,
        JINTEGER,
        JSTRING
    };

    class JsonException {
    private:
        std::string error_message;

    public:
        JsonException(const std::string& error)
                : error_message(error)
        {}

        const char* getError() { return error_message.c_str(); }
    };

    class JsonData {
    private:
        int data_int;
        bool data_bool;
        std::string data_str;

        JsonType type;
    public:
        JsonData() = default;

        JsonData(int val)
            : data_int(val)
            , type(JsonType::JINTEGER)
        {}

        JsonData(bool val)
                : data_bool(val)
                , type(JsonType::JBOOLEAN)
        {}

        JsonData(const std::string& val)
                : data_str(val)
                , type(JsonType::JSTRING)
        {}

        JsonType GetJsonDataType() const;

        int GetIntData() const;
        bool GetBoolData() const;
        std::string GetStringData() const;

    };

    using JsonLinesMap = std::unordered_map<int64_t, JsonData>;
    using JsonKeysLinesMap = std::unordered_map<std::string, int64_t>;

    class JsonReader {
    private:
        std::ifstream json_file;
        std::vector<JsonLinesMap> json_lines;
        std::vector<JsonKeysLinesMap> lines_keys;

        int lines_count = 0;

    public:
        explicit JsonReader(const std::string& json_file_path);

        const JsonKeysLinesMap& GetKeysMapForLine(int index) const;
        const JsonLinesMap& GetJsonLine(int index) const;

        int GetLinesCount() const { return lines_count; }

    private:
        JsonType FindJsonTypeFromString(const std::string& str);

    };

    std::vector<std::string> SplitBySeparator(const std::string& str, const std::string& separator);

}
