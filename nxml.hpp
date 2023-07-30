#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

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
    public:
        Parser();
        Document GetFromString(string& xml);
        string   ToString(Document& xml);

        enum class Mode
        {
            Declaration,
            WaitForTagOpen,
            TagOpen,
            WaitForAttribute,
            TagAttributeName,
            TagAttributeValue,
            TagClose
        };
    protected:
        Mode p_Mode;

        stringstream p_TagNameStream;
        stringstream p_TagValueStream;
        stringstream p_AttributeNameStream;
        stringstream p_AttributeValueStream;
        
        string GetModeName(Mode& mode);

        void SwitchMode(Mode newMode, char current);
        void ProcessCharacter(std::string& xmlString, int charIndex);

        void LogCurrentTagName();
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

void nxml::Parser::LogCurrentTagName()
{
    // create tag object
    cout << "Current Tag Name : " << p_TagNameStream.str() << endl;
    // Clear tag streams
    p_TagNameStream.str(std::string());
}

std::string nxml::Parser::GetModeName(nxml::Parser::Mode& mode)
{
    switch(mode)
    {
        case Mode::Declaration:
        return "Declaration";
        case Mode::WaitForTagOpen:
        return "WaitForTagOpen";
        case Mode::TagOpen:
        return "TagOpen";
        case Mode::TagClose:
        return "TagClose";
        case Mode::WaitForAttribute:
        return "WaitForAttribute";
        case Mode::TagAttributeName:
        return "TagAttributeName";
        case Mode::TagAttributeValue:
        return "TagAttributeValue";
        default:
        return "Unknown Mode Type";
    }
}

void nxml::Parser::SwitchMode(nxml::Parser::Mode newMode, char current)
{
    cout << "Current Mode : " << GetModeName(p_Mode) << ", Switching to Mode : " << GetModeName(newMode) << ", From Character : '" << current << "'\n";
    p_Mode = newMode;
}

void nxml::Parser::ProcessCharacter(std::string& xmlString, int charIndex)
{
    char c  = xmlString.at(charIndex);
    char nc = xmlString.at(charIndex + 1);

    switch(p_Mode)
    {
        case Mode::Declaration:
            if(c == '?' && nc == '>') SwitchMode(Mode::WaitForTagOpen, c);
            break;
        case Mode::WaitForTagOpen:
            if(c != '<') return;
            SwitchMode(Mode::TagOpen, c);
            break;
        case Mode::TagOpen:
            if(c == '/')
            {
                SwitchMode(Mode::TagClose, c);
                return;
            }
            if(c == '>')
            {
                LogCurrentTagName();
                SwitchMode(Mode::WaitForTagOpen, c);
                return;
            }
            if(c == ' ')
            {
                LogCurrentTagName();
                SwitchMode(Mode::WaitForAttribute, c);
                return;
            }
            // push char into tag name stream
            p_TagNameStream << c;
            break;
        case Mode::WaitForAttribute:
            if(c == ' ') return;
            // create attribute object
            cout << "Current Attribute Name : " << p_AttributeNameStream.str() << endl;
            cout << "Current Attribute Value : " << p_AttributeValueStream.str() << endl;
            // Clear streams
            p_AttributeNameStream.str(std::string());
            p_AttributeValueStream.str(std::string());

            if(c == '>') 
            {
                SwitchMode(Mode::WaitForTagOpen, c);
                return;
            }
            if(c == '/')
            {
                SwitchMode(Mode::TagClose, c);
                return;
            }
            SwitchMode(Mode::TagAttributeName, c);
            break;    
        case Mode::TagAttributeName:
            if(c == '=') SwitchMode(Mode::TagAttributeValue, c);
            // push char into attribute name stream
            p_AttributeNameStream << c;
            break;
        case Mode::TagAttributeValue:
            if(c == '=') return;
            if(c == '"' && p_AttributeValueStream.str().size() < 0) return;
            if(c == '"')
            {
                SwitchMode(Mode::WaitForAttribute, c);
                return;
            }
            // push char into attribute value stream;
            p_AttributeValueStream << c;
            break;
        case Mode::TagClose:
            LogCurrentTagName();
            SwitchMode(Mode::WaitForTagOpen, c); 
            break;
        default:
            break;
    }
}

nxml::Document nxml::Parser::GetFromString(std::string& xml)
{
    nxml::Document doc;

    for(int i = 0; i < xml.size(); i++)
    {
        ProcessCharacter(xml, i);
    }

    return doc;
}
#endif