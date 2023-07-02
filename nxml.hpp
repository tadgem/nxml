#pragma once
#include <string>
#include <vector>

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
        string Value;
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
            TagAttribute,
            TagValue,
            TagClose
        };

        Mode p_Mode;

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

nxml::Document nxml::Parser::GetFromString(std::string& xml)
{
    std::istringstream iter(xml);
    std::string line;

    nxml::Document doc;

    while (getline(iter, line))
    {
        for(char c : line)
        {
            
        }
        std::cout << "Line : " << line << std::endl;
    }

    return doc;
}
#endif