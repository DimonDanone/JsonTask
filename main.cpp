#include <iostream>

#include "JsonReader.h"
#include "TLVencoding.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc == 1) {
        cout << "Please, write path to json file" << endl;
        return 0;
    }

    JsonReading::JsonReader reader(argv[1]);

    ofstream file_for_keys("keys_lines_file", ios_base::binary);
    ofstream file_for_json_lines("json_lines_file", ios_base::binary);

    TLVencoding::TLVencoder tlv_encoder(reader);

    tlv_encoder.EncodeJsonKeysToFile(file_for_keys);
    tlv_encoder.EncodeJsonLinesToFile(file_for_json_lines);

    file_for_keys.close();
    file_for_json_lines.close();


    cout << "Output files :\n\t- keys_lines_file\n\t- json_lines_file" << endl;
    
    //Small encoding testing
    /*
    TLVencoding::TLVdecoder decoder;

    ifstream input_for_keys("keys_file");
    ifstream input_for_json_lines("lines_file");

    auto test_keys_map = decoder.DecodeJsonKeysFromFile(input_for_keys);
    auto test_json_lines = decoder.DecodeJsonLinesFromFile(input_for_json_lines);
    */

    return 0;
}

