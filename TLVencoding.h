#pragma once

#include "JsonReader.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <cstddef>

namespace TLVencoding {

    class TLVencoder {
    private:
        static const unsigned char bool_tag = 'b';
        static const unsigned char string_tag = 's';
        static const unsigned char integer_tag = 'i';
        static const unsigned char double_tag = 'd';
        static const unsigned char new_line_tag = 'n';

        const JsonReading::JsonReader& json_reader;

    public:
        explicit TLVencoder(const JsonReading::JsonReader& json_reader)
            : json_reader(json_reader)
        {}

        std::ofstream& EncodeJsonLinesToFile(std::ofstream& output);

        std::ofstream& EncodeJsonKeysToFile(std::ofstream& output);
    private:
        //create bytes line for int
        static std::vector<unsigned char> GetByteImageInt(int val);

        //create bytes line for double
        static std::vector<unsigned char> GetByteImageDouble(double val);

        //create bytes line for bool
        static std::vector<unsigned char> GetByteImageBool(bool val);

        //create bytes line for string
        static std::vector<unsigned char> GetByteImageString(const std::string& s);

        //create bytes line for JsonData
        static std::vector<unsigned char> GetByteImageJsonData(const JsonReading::JsonData& data);
    };

    class TLVdecoder {
    private:
        static const unsigned char bool_tag = 'b';
        static const unsigned char string_tag = 's';
        static const unsigned char integer_tag = 'i';
        static const unsigned char double_tag = 'd';
        static const unsigned char new_line_tag = 'n';

        enum class DecodedTypes {
            DECODED_INT,
            DECODED_DOUBLE,
            DECODED_BOOL,
            DECODED_STRING,
            DENDL
        };

        class DecodedData {
        private:
            int int_data;
            double double_data;
            bool bool_data;
            std::string string_data;

            DecodedTypes type;
        public:
            DecodedData()
                : type(DecodedTypes::DENDL)
            {}

            DecodedData(int val)
                : int_data(val)
                , type(DecodedTypes::DECODED_INT)
            {}

            DecodedData(double val)
                    : double_data(val)
                    , type(DecodedTypes::DECODED_DOUBLE)
            {}

            DecodedData(bool val)
                : bool_data(val)
                , type(DecodedTypes::DECODED_BOOL)
            {}

            DecodedData(const std::string& val)
                : string_data(val)
                , type(DecodedTypes::DECODED_STRING)
            {}


            DecodedTypes GetType() const { return type; }

            int GetIntData() const { return int_data; }
            double GetDoubleData() const { return double_data; }
            bool GetBoolData() const { return bool_data; }
            const std::string& GetStringData() const { return string_data; }
        };

    public:
        TLVdecoder() = default;

        std::vector<JsonReading::JsonLinePairs> DecodeJsonLinesFromFile(std::ifstream& input);

        JsonReading::JsonKeysMap DecodeJsonKeysFromFile(std::ifstream& input);

    private:
        static int GetIntFromBytes(const std::vector<unsigned char>& bytes);

        static double GetDoubleFromBytes(const std::vector<unsigned char>& bytes);

        static bool GetBoolFromBytes(const std::vector<unsigned char>& bytes);

        static std::string GetStringFromBytes(const std::vector<unsigned char>& bytes);

        static std::vector<DecodedData> DecodeFile(std::ifstream& input);

        static std::vector<unsigned char> ReadBytes(std::ifstream& input, int count);
    };

}
