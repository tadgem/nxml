#include <iostream>
#include <fstream>

#include "nxml.hpp"

std::string LoadFileAsString(const char* path) {
    std::fstream file;
    file.open(path);

    std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

    return str;
}

int main() {
    using namespace std;

    string sampleXml = LoadFileAsString("sample.xml");
    nxml::Parser parser;

    parser.GetFromString(sampleXml);
}