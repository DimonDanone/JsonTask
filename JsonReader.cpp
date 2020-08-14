#include "JsonReader.h"
using namespace std;

namespace JsonReading {

    //JsonData definition
    JsonType JsonData::GetJsonDataType() const {
        return type;
    }

    int JsonData::GetIntData() const {
        if (type != JsonType::JINTEGER) {
            throw JsonException("this json_data type is not integer\n");
        }

        return data_int;
    }

    bool JsonData::GetBoolData() const {
        if (type != JsonType::JBOOLEAN) {
            throw JsonException("this json_data type is not bool\n");
        }

        return data_bool;
    }

    string JsonData::GetStringData() const {
        if (type != JsonType::JSTRING) {
            throw JsonException("this json_data type is not string\n");
        }

        return data_str;
    }


    //JsonReader definition
    JsonReader::JsonReader(const string &json_file_path)
            : json_file(json_file_path) {
        if (!json_file.is_open()) {
            throw JsonException("json file was not opened\n");
        }

        for (string now_row; getline(json_file, now_row);) {
            lines_count++;
            lines_keys.emplace_back();
            json_lines.emplace_back();
            //"{key1:val1,key2:val2}" ->  { "key1:val1", "key2:val2" }
            vector<string> json_datas = SplitBySeparator(now_row, ", ");

            for (size_t i = 0; i < json_datas.size(); ++i) {
                // "key:value" -> {"key", "val"}
                vector<string> key_val = SplitBySeparator(json_datas[i], ":");

                //delete {} from json record
                if (key_val[0].front() == '{') {
                    key_val[0] = key_val[0].substr(1, key_val[0].size() - 1);
                }
                if (key_val.back().back() == '}') {
                    key_val.back() = key_val.back().substr(0, key_val.back().size() - 1);
                }

                string key = key_val[0];
                string val = key_val[1];
                lines_keys.back()[key] = i + 1;

                JsonType data_type = FindJsonTypeFromString(val);
                if (data_type == JsonType::JBOOLEAN) {
                    bool bool_value = (val[0] == 'T');
                    json_lines.back()[i + 1] = JsonData(bool_value);
                } else if (data_type == JsonType::JINTEGER) {
                    json_lines.back()[i + 1] = JsonData(stoi(val));
                } else {
                    json_lines.back()[i + 1] = JsonData(val);
                }
            }
        }

        json_file.close();
    }

    JsonType JsonReader::FindJsonTypeFromString(const string &str) {
        if (str[0] == 'T' || str[0] == 'F') {
            return JsonType::JBOOLEAN;
        } else if (str[0] >= '0' && str[0] <= '9') {
            return JsonType::JINTEGER;
        }

        return JsonType::JSTRING;
    }

    const JsonKeysLinesMap& JsonReader::GetKeysMapForLine(int index) const {
        return lines_keys[index];
    }

    const JsonLinesMap& JsonReader::GetJsonLine(int index) const {
        return json_lines[index];
    }

    //Some functions
    vector<int> Z_Function(const string &s) {
        vector<int> z_arr(s.size());

        int l = 0, r = 0;
        for (int i = 1; i < s.size(); ++i) {
            if (i <= r)
                z_arr[i] = min(z_arr[i - l], r - i + 1);

            for (; i + z_arr[i] < s.size() && s[z_arr[i]] == s[i + z_arr[i]];)
                z_arr[i]++;

            if (i + z_arr[i] - 1 > r) {
                l = i;
                r = i + z_arr[i] - 1;
            }
        }

        return z_arr;
    }


    vector<string> SplitBySeparator(const string &str, const string &separator) {
        vector<string> result;

        vector<int> z_func = Z_Function(separator + "|" + str);

        string tmp_str = "";
        int diff = separator.size() + 1; //between str and z_func
        for (int i = diff; i < z_func.size(); ++i) {
            if (z_func[i] == separator.size()) {
                result.push_back(tmp_str);
                tmp_str = "";
                if (separator.size() > 1) {
                    ++i;
                }
            } else {
                tmp_str.push_back(str[i - diff]);
            }
        }
        if (!tmp_str.empty()) {
            result.push_back(tmp_str);
        }

        return result;
    }


}