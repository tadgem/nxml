#pragma once
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <sstream>
#include <cwctype>
#include <sstream>
#include <cassert>
namespace nxml
{
    using namespace std;

    /// <summary>
    /// For Elements / elements, what type of value does str represent
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
    /// Element attribute stuct
    /// </summary>
    struct Attribute : ISerializable
    {
        string Key;
        string SerializedValue;

        virtual ~Attribute() {};
    };

    template <typename T>
    struct TAttribute : Attribute
    {
        T Value;
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
    /// Base type for elements, every element will have N number of Elements, and a Name. 
    /// 0 Elements is valid
    /// </summary>
    struct Element : ISerializable
    {
        string ElementName;
        vector<Attribute> Attributes;        
    };

    struct ValueElement : Element
    {
        string InnerValue;

        virtual string  ToString()      override {return "<ElementName>InnerValue</ElementName>";}
        virtual void    FromString(string str)    override {}
    };

    struct ComplexElement : Element
    {
        vector<Element*> InnerElements;
        virtual string  ToString()      override {return "<ElementName>*InnerElements*/ElementName>";}
        virtual void    FromString(string str)    override {}
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
            WaitForElementOpen,
            ElementOpen,
            WaitForAttribute,
            ElementAttributeName,
            ElementAttributeValue,
            ElementClose,
            GetInnerElementType,
            ElementValue,
        };
    protected:
        Mode p_Mode;

        stringstream p_ElementNameStream;
        stringstream p_ElementValueStream;
        stringstream p_AttributeNameStream;
        stringstream p_AttributeValueStream;

        stack<Element*> p_ElementStack;
        
        string GetModeName(Mode& mode);

        void SwitchMode(Mode newMode, char current);
        void ProcessCharacter(std::string& xmlString, int charIndex);

        void CreateElement(bool complex = false);
        void CloseElement();
        void AssignElementValue();
        void CreateAttribute();
        void CloseAttribute();

        void LogCurrentElementName();
        void LogCurrentAttributes();
    };
}









// TODO Disable
#define NXML_IMPL
#ifdef NXML_IMPL

#define NXML_ASSERT(exp, msg) assert(((void)msg, exp))

nxml::Parser::Parser()
{
    p_Mode = Parser::Mode::Declaration;
}

void nxml::Parser::CreateElement(bool complex)
{
    if(complex)
    {
        p_ElementStack.emplace(new ComplexElement());
    }
    else
    {
        p_ElementStack.emplace(new ValueElement());
    }
    p_ElementStack.top()->ElementName = p_ElementNameStream.str();
}

void nxml::Parser::CloseElement()
{
    Element* e = p_ElementStack.top();
    p_ElementStack.pop();

    ComplexElement* parent = static_cast<ComplexElement*>(p_ElementStack.top());

    NXML_ASSERT(parent != nullptr, "Parent Element is not a complex element");

    parent->InnerElements.emplace_back(e);
}

void nxml::Parser::AssignElementValue()
{
    ValueElement* e = static_cast<ValueElement*>(p_ElementStack.top());
    // assign e value;
}

void nxml::Parser::CreateAttribute()
{

}

void nxml::Parser::CloseAttribute()
{

}


void nxml::Parser::LogCurrentElementName()
{
    // create Element object
    cout << "\n" << "Current Element Name : " << p_ElementNameStream.str() << endl;
    // Clear Element streams
    cout << "Current Element Value : " << p_ElementValueStream.str() << "\n" << endl;
    
    p_ElementNameStream.str(std::string());
    p_ElementValueStream.str(std::string());
}

void nxml::Parser::LogCurrentAttributes()
{
    // create attribute object
    cout << "\n" << "Current Attribute Name : " << p_AttributeNameStream.str() << endl;
    cout << "Current Attribute Value : " << p_AttributeValueStream.str() << "\n" << endl;
    // Clear streams
    p_AttributeNameStream.str(std::string());
    p_AttributeValueStream.str(std::string());
}


std::string nxml::Parser::GetModeName(nxml::Parser::Mode& mode)
{
    switch(mode)
    {
        case Mode::Declaration:
        return "Declaration";
        case Mode::WaitForElementOpen:
        return "WaitForElementOpen";
        case Mode::ElementOpen:
        return "ElementOpen";
        case Mode::ElementClose:
        return "ElementClose";
        case Mode::WaitForAttribute:
        return "WaitForAttribute";
        case Mode::ElementAttributeName:
        return "ElementAttributeName";
        case Mode::ElementAttributeValue:
        return "ElementAttributeValue";
        case Mode::GetInnerElementType:
        return "GetInnerElementType";
        case Mode::ElementValue:
        return "ElementValue";
        default:
        return "Unknown Mode Type";
    }
}

void nxml::Parser::SwitchMode(nxml::Parser::Mode newMode, char current)
{
    cout << "        Switch Mode : Current Mode : " << GetModeName(p_Mode) << ", New Mode : " << GetModeName(newMode) << ", From Character : '" << current << "'\n";
    p_Mode = newMode;
}

void nxml::Parser::ProcessCharacter(std::string& xmlString, int charIndex)
{
    char c  = xmlString.at(charIndex);
    char nc = xmlString.at(charIndex + 1);

    switch(p_Mode)
    {
        case Mode::Declaration:
            if(c == '?' && nc == '>') SwitchMode(Mode::WaitForElementOpen, c);
            break;
        case Mode::WaitForElementOpen:
            if(c != '<') return;
            SwitchMode(Mode::ElementOpen, c);
            break;
        case Mode::ElementOpen:
            if(c == '/')
            {
                LogCurrentElementName();
                SwitchMode(Mode::ElementClose, c);
                return;
            }
            if(c == '>')
            {
                LogCurrentElementName();
                SwitchMode(Mode::GetInnerElementType, c);
                return;
            }
            if(c == ' ')
            {
                LogCurrentElementName();
                SwitchMode(Mode::WaitForAttribute, c);
                return;
            }
            // push char into Element name stream
            p_ElementNameStream << c;
            break;
        case Mode::WaitForAttribute:
            if(c == ' ') return;
            if(c == '>') 
            {
                SwitchMode(Mode::GetInnerElementType, c);
                return;
            }
            if(c == '/')
            {
                SwitchMode(Mode::ElementClose, c);
                return;
            }
            SwitchMode(Mode::ElementAttributeName, c);
            p_AttributeNameStream << c;
            break;    
        case Mode::ElementAttributeName:
            if(c == '=') 
            {
                SwitchMode(Mode::ElementAttributeValue, c);
                return;
            }
            if(c == '"')
            {
                SwitchMode(Mode::ElementAttributeValue, c);
                return;
            } 
            // push char into attribute name stream
            p_AttributeNameStream << c;
            break;
        case Mode::ElementAttributeValue:
            if(c == '=') return;
            if(c == '"') return;
            if(c == ' ')
            {
                LogCurrentAttributes();
                SwitchMode(Mode::WaitForAttribute, c);
                return;
            }
            if(c == '>')
            {
                LogCurrentAttributes();
                SwitchMode(Mode::GetInnerElementType, c);
                return;
            }
            // push char into attribute value stream;
            p_AttributeValueStream << c;
            break;
        case Mode::ElementClose:
            LogCurrentElementName();
            SwitchMode(Mode::WaitForElementOpen, c); 
            break;
        case Mode::GetInnerElementType:
            if(c == ' ') return;
            if(c == '<')
            {
                SwitchMode(Mode::ElementOpen, c);
                return;
            }
            if(iswalnum(c))
            {
                p_ElementValueStream << c;
                SwitchMode(Mode::ElementValue, c);
                return;
            }

            break;
        case Mode::ElementValue:
            if(c == '<')
            {
                SwitchMode(Mode::ElementClose, c);
                return;
            }
            p_ElementValueStream << c;
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