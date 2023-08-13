#define NXML_IMPL;
#include "nxml.hpp"
using namespace std;

struct Vec2
{
    double X;
    double Y;
};

struct TestStruct
{
    string Name;
    float  SomeFloat;
    double SomeDouble;
    Vec2   SomeUserType;

    operator nxml::Element& () { return nxml::Element::Invalid; }
    operator nxml::Element () const { return nxml::Element::Invalid; }


};

int main() {
    // Simple
    string sampleXml = nxml::utils::LoadFileAsString("../sample.xml");
    nxml::Document doc = nxml::ParseString(sampleXml);
    nxml::Element bookElement = doc["catalog"][{"book", "id", "bk103"}];

    string parsedXml = doc.ToString();
    nxml::utils::SaveStringToFile("../sample_parsed.xml", parsedXml);

    // Usertypes
    Vec2 v{ 1.0, 2.0 };
    TestStruct userData{ "Hello", 1.5f, 3.0, v };

    nxml::Element userDataElement = userData;
}