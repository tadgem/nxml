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

void SaveStringToFile(const char* path, std::string& str) {
    std::ofstream out(path);
    out << str;
    out.close();
}

void RecursePrintElement(nxml::Element* element)
{
    std::cout << "Element Name : " << element->ElementName << std::endl;
}

int main() {
    using namespace std;
    string sampleXml = LoadFileAsString("../sample.xml");
    nxml::Parser parser;

    nxml::Document doc = parser.GetFromString(sampleXml);

    string parsedXml = doc.ToString();
    SaveStringToFile("../sample_parsed.xml", parsedXml);

    cout << "Parsex XML String : " << parsedXml << endl;
}