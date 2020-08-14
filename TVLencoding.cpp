#include "TLVencoding.h"

using namespace std;

namespace TLVencoding {
    //Encoder
    ofstream& TLVencoder::EncodeJsonKeysToFile(ofstream& output) {
        for (int i = 0; i < json_reader.GetLinesCount(); ++i) {
            for (const auto& [key, val] : json_reader.GetKeysMapForLine(i)) {
                vector<unsigned char> key_bytes = GetByteImageString(key);
                for (int j = 0; j < key_bytes.size(); ++j) {
                    output << key_bytes[j];
                }

                vector<unsigned char> val_bytes = GetByteImageInt(val);
                for (int j = 0; j < val_bytes.size(); ++j) {
                    output << val_bytes[j];
                }
            }
            output << new_line_tag;
        }

        return output;
    }

    std::ofstream & TLVencoder::EncodeJsonLinesToFile(std::ofstream &output) {
        for (int i = 0; i < json_reader.GetLinesCount(); ++i) {
            for (const auto& [key, val] : json_reader.GetJsonLine(i)) {
                vector<unsigned char> key_bytes = GetByteImageInt(key);
                for (int j = 0; j < key_bytes.size(); ++j) {
                    output << key_bytes[j];
                }

                vector<unsigned char> val_bytes = GetByteImageJsonData(val);
                for (int j = 0; j < val_bytes.size(); ++j) {
                    output << val_bytes[j];
                }
            }
            output << new_line_tag;
        }

        return output;
    }

    vector<unsigned char> TLVencoder::GetByteImageInt(int val) {
        vector<unsigned char> result(5);

        result[0] = integer_tag;
        for (int i = 0; i < 4; ++i) {
            result[4 - i] = val >> (i * 8);
        }

        return result;
    }

    vector<unsigned char> TLVencoder::GetByteImageBool(bool val) {
        vector<unsigned char> result(2);

        result[0] = bool_tag;
        result[1] = val;

        return result;
    }

    vector<unsigned char> TLVencoder::GetByteImageString(const string& s) {
        vector<unsigned char> result(s.size() + 5);

        result[0] = string_tag;
        //I think, I need to add size_t bytes image for string size
        vector<unsigned char> s_size_int_bytes = GetByteImageInt(s.size());
        for (int i = 1; i < 5; ++i) {
            result[i] = s_size_int_bytes[i];
        }

        for (int i = 0; i < s.size(); ++i) {
            result[i + 5] = s[i];
        }

        return result;
    }

    vector<unsigned char> TLVencoder::GetByteImageJsonData(const JsonReading::JsonData& data) {
        if (data.GetJsonDataType() == JsonReading::JsonType::JINTEGER) {
            return GetByteImageInt(data.GetIntData());
        } else if (data.GetJsonDataType() == JsonReading::JsonType::JBOOLEAN) {
            return GetByteImageBool(data.GetBoolData());
        }

        return GetByteImageString(data.GetStringData());
    }

    //Decoder
    vector<JsonReading::JsonKeysLinesMap> TLVdecoder::DecodeJsonKeysFromFile(std::ifstream& input) {
        vector<JsonReading::JsonKeysLinesMap> result;

        vector<DecodedData> all_data = DecodeFile(input);

        if (!all_data.empty()) {
            result.emplace_back();
            for (int i = 0; i < all_data.size() - 1; ++i) {
                DecodedData key = all_data[i];
                if (key.GetType() == DecodedTypes::DENDL) {
                    result.emplace_back();
                } else {
                    DecodedData val = all_data[++i];
                    result.back()[key.GetStringData()] = val.GetIntData();
                }
            }
        }

        return result;
    }

    vector<JsonReading::JsonLinesMap> TLVdecoder::DecodeJsonLinesFromFile(std::ifstream& input) {
        vector<JsonReading::JsonLinesMap> result;

        vector<DecodedData> all_data = DecodeFile(input);

        if (!all_data.empty()) {
            result.emplace_back();
            for (int i = 0; i < all_data.size() - 1; ++i) {
                DecodedData key = all_data[i];
                if (key.GetType() == DecodedTypes::DENDL) {
                    result.emplace_back();
                } else {
                    DecodedData val = all_data[++i];
                    if (val.GetType() == DecodedTypes::DECODED_BOOL) {
                        result.back()[key.GetIntData()] = JsonReading::JsonData(val.GetBoolData());
                    } else if (val.GetType() == DecodedTypes::DECODED_INT) {
                        result.back()[key.GetIntData()] = JsonReading::JsonData(val.GetIntData());
                    } else {
                        result.back()[key.GetIntData()] = JsonReading::JsonData(val.GetStringData());
                    }
                }
            }
        }

        return result;
    }

    vector<TLVdecoder::DecodedData> TLVdecoder::DecodeFile(std::ifstream& input) {
        vector<DecodedData> all_data;

        while (input.peek() != EOF) {
            unsigned char type;
            type = input.get();
            //to check endl
            if (type == new_line_tag) {
                all_data.emplace_back();
                continue;
            }

            vector<unsigned char> data_bytes;
            if (type == bool_tag) {
                all_data.push_back(GetBoolFromBytes(ReadBytes(input, 1)));
            } else if (type == integer_tag) {
                all_data.push_back(GetIntFromBytes(ReadBytes(input, 4)));
            } else {
                int string_size = GetIntFromBytes(ReadBytes(input, 4));
                all_data.push_back(GetStringFromBytes(ReadBytes(input, string_size)));
            }
        }

        return all_data;
    }

    vector<unsigned char> TLVdecoder::ReadBytes(ifstream &input, int count) {
        vector<unsigned char> result(count);
        for (int i = 0; i < count; ++i) {
            result[i] = input.get();
        }

        return result;
    }

    bool TLVdecoder::GetBoolFromBytes(const vector<unsigned char>& bytes) {
        return bytes[0];
    }

    string TLVdecoder::GetStringFromBytes(const vector<unsigned char>& bytes) {
        string result;

        for (auto ch : bytes) {
            result.push_back(ch);
        }

        return result;
    }

    int TLVdecoder::GetIntFromBytes(const std::vector<unsigned char>& bytes) {
        int result;

        for (int i = 0; i < bytes.size(); ++i) {
            result = result << 8;
            result |= bytes[i];
        }

        return result;
    }
}