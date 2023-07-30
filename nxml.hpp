#pragma once
#include <string>
#include <vector>
#include <iostream>

namespace nxml
{
    using namespace std;

    /// <summary>
    /// For tags / elements, what type of value does str represent
    /// </summary>
    enum class ValueType
    {
        Integer,
        Float,
        String
    };

    /// <summary>
    /// Interface allows derived types
    /// to define how they are de/serialized
    /// Parser does not need to care about type...
    /// </summary>
    struct ISerializable
    {
        virtual string  ToString() = 0;
        virtual void    FromString(string str) = 0;
    };
    /// <sumarry>
    /// Tag attribute stuct
    /// </summary>
    struct Attribute : ISerializable
    {
        string Key;
        string SerializedValue;
    };

    template <typename T>
    struct TAttribute : Attribute
    {
        T Value;
    };

    /// <summary>
    /// structure to encapsulate tags in element definitions
    /// </summary>
    struct Tag : ISerializable
    {
        ValueType Type;
        
        string Key;
        string Value;

        vector<Attribute> Attributes;
    };

    /// <summary>
    /// Probably dont need to change this...
    /// </summary>
    struct Declaration : ISerializable
    {
        const string VERSION = "1.0";

        virtual string  ToString()      override {return "<?xml version=\"1.0\"?>";}
        virtual void    FromString(string str)    override {}
    };
    
    /// <summary>
    /// Base type for elements, every element will have N number of tags, and a Name. 
    /// 0 Tags is valid
    /// </summary>
    struct Element : ISerializable
    {
        string Name;
        vector<Tag> Tags;        
    };

    struct Document
    {
        Declaration Decl;
        Element* Root;
    };

    class Parser
    {
    protected:
        enum class Mode
        {
            Declaration,
            TagOpen,
            TagAttributeName,
            TagAttributeValue,
            TagClose
        };

        Mode p_Mode;
        void HandleParserStateSwitch(std::string& xmlString, int charIndex);
    public:
        Parser();
        Document GetFromString(string& xml);
        string   ToString(Document& xml);
    };
}
// TODO Disable
#define NXML_IMPL
#ifdef NXML_IMPL
#include <sstream>
nxml::Parser::Parser()
{
    p_Mode = Parser::Mode::Declaration;
}

void nxml::Parser::HandleParserStateSwitch(std::string& xmlString, int charIndex)
{
    char c  = xmlString.at(charIndex);
    char nc = xmlString.at(charIndex + 1);

    switch(p_Mode)
    {
        case Mode::Declaration:
            if(c == '?' && nc == '>') p_Mode = Mode::TagOpen;
            break;
        case Mode::TagOpen:
            break;
        case Mode::TagAttributeName:
            break;
        case Mode::TagAttributeValue:
            break;
        case Mode::TagClose:
            break;
        default:
            std::cout << "nxml::Parser : Error : Unknown parsing state";
            break;
    }
}

nxml::Document nxml::Parser::GetFromString(std::string& xml)
{
    nxml::Document doc;

    for(int i = 0; i < xml.size(); i++)
    {
        HandleParserStateSwitch(xml, i);
    }

    return doc;
}
#endif