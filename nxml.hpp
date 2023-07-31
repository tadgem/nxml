#pragma once
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <sstream>
#include <cwctype>
#include <sstream>
#include <cassert>
#include <string>
#include <algorithm>
#include <cctype>
#include <regex>

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

        virtual string  ToString()      override {return "<?xml version=\"1.0\"?>";}
        virtual void    FromString(string str)    override {}
        
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

    /// <summary>
    /// Element which contains a value, no children
    /// </summary>
    struct ValueElement : Element
    {
        string InnerValue;

        virtual string  ToString()      override;
        virtual void    FromString(string str)    override {}
    };

    /// <summary>
    /// Element which contains child elements
    /// </summary>
    struct ComplexElement : Element
    {
        vector<Element*> InnerElements;
        virtual string  ToString()      override;
        virtual void    FromString(string str)    override {}
    };

    /// <summary>
    /// Container for declaration and root element
    /// </summary>
    struct Document : ISerializable
    {
        Declaration Decl;
        vector<Element*> RootElements;

        virtual string  ToString()      override;
        virtual void    FromString(string str)    override {}
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
        stack<Attribute> p_AttributeStack;
        
        string GetModeName(Mode& mode);

        void SwitchMode(Mode newMode, char current);
        void ProcessCharacter(std::string& xmlString, int charIndex);

        void CreateElement(bool complex = false);
        void CloseElement();
        void AssignElementValue();
        void CreateAttribute();
        void LogCurrentElementName();
        void LogCurrentAttributes();

        void ClearCurrentElement();
        void ClearCurrentAttribute();
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

std::string nxml::ValueElement::ToString()
{
    stringstream s;
    s << '<' << ElementName;
    for (auto attr : Attributes)
    {
        s << ' ' << attr.Key << '=' << '"' << attr.SerializedValue << '"';
    }
    s << '>' << InnerValue << "</" << ElementName << '>';
    return s.str();
}

std::string nxml::ComplexElement::ToString()
{
    stringstream s;
    s << '<' << ElementName;
    for (auto attr : Attributes)
    {
        s << ' ' << attr.Key << '=' << '"' << attr.SerializedValue << '"';
    }
    s << '>'; 
    for (auto inner : InnerElements)
    {
        s << inner->ToString();
    }

    s << "</" << ElementName << '>';

    return s.str();
}


std::string nxml::Document::ToString()
{
    stringstream s;
    s << Decl.ToString();
    for (auto e: RootElements)
    {
        s << e->ToString();
    }

    std::regex e("[ \t]+");   // matches trailing whitespace

    string docStringRaw = s.str();
    docStringRaw.erase(std::remove(docStringRaw.begin(), docStringRaw.end(), '\r'), docStringRaw.end());
    docStringRaw.erase(std::remove(docStringRaw.begin(), docStringRaw.end(), '\n'), docStringRaw.cend());
    docStringRaw.erase(std::remove(docStringRaw.begin(), docStringRaw.end(), '\t'), docStringRaw.end());

    docStringRaw = std::regex_replace (docStringRaw,e," $2"); // replace all trailing space wioth single space.
    return docStringRaw;
}


void nxml::Parser::ClearCurrentElement()
{        
    // CreateElement;
    p_ElementNameStream.str(std::string());
    p_ElementValueStream.str(std::string());
}

void nxml::Parser::ClearCurrentAttribute()
{
    // Clear streams
    p_AttributeNameStream.str(std::string());
    p_AttributeValueStream.str(std::string());
}

void nxml::Parser::CreateElement(bool complex)
{
    string elementName = p_ElementNameStream.str();
    if(complex)
    {
        p_ElementStack.emplace(new ComplexElement());
    }
    else
    {
        p_ElementStack.emplace(new ValueElement());
    }
    p_ElementStack.top()->ElementName = elementName;

    while (!p_AttributeStack.empty())
    {
        p_ElementStack.top()->Attributes.push_back(p_AttributeStack.top());
        p_AttributeStack.pop();
    }
}

void nxml::Parser::CloseElement()
{
    Element* e = p_ElementStack.top();
    p_ElementStack.pop();

    if (p_ElementStack.empty())
    {
        p_ElementStack.push(e);
        return;
    }

    ComplexElement* parent = static_cast<ComplexElement*>(p_ElementStack.top());

    NXML_ASSERT(parent != nullptr, "Parent Element is not a complex element");

    parent->InnerElements.emplace_back(e);
}

void nxml::Parser::AssignElementValue()
{
    ValueElement* e = static_cast<ValueElement*>(p_ElementStack.top());
    e->InnerValue = p_ElementValueStream.str();
}

void nxml::Parser::CreateAttribute()
{
    Attribute attr;
    attr.Key = p_AttributeNameStream.str();
    attr.SerializedValue = p_AttributeValueStream.str();

    p_AttributeStack.push(attr);
}

void nxml::Parser::LogCurrentElementName()
{
    // create Element object
    cout << "\n" << "Current Element Name : " << p_ElementNameStream.str() << endl;
    // Clear Element streams
    cout << "Current Element Value : " << p_ElementValueStream.str() << "\n" << endl;
}

void nxml::Parser::LogCurrentAttributes()
{
    // create attribute object
    cout << "\n" << "Current Attribute Name : " << p_AttributeNameStream.str() << endl;
    cout << "Current Attribute Value : " << p_AttributeValueStream.str() << "\n" << endl;
    ClearCurrentAttribute();
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
    if (charIndex + 1 == xmlString.size())
    {
        return;
    }
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
                SwitchMode(Mode::GetInnerElementType, c);
                return;
            }
            if(c == ' ')
            {
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
                CreateAttribute();
                LogCurrentAttributes();
                SwitchMode(Mode::WaitForAttribute, c);
                return;
            }
            if(c == '>')
            {
                CreateAttribute();
                LogCurrentAttributes();
                SwitchMode(Mode::GetInnerElementType, c);
                return;
            }
            // push char into attribute value stream;
            p_AttributeValueStream << c;
            break;
        case Mode::ElementClose:
            LogCurrentElementName();
            CloseElement();
            ClearCurrentElement();
            SwitchMode(Mode::WaitForElementOpen, c); 
            break;
        case Mode::GetInnerElementType:
            if(c == ' ') return;
            if(c == '<')
            {
                CreateElement(true);
                ClearCurrentElement();
                SwitchMode(Mode::ElementOpen, c);
                return;
            }
            if(iswalnum(c))
            {
                CreateElement(false);
                p_ElementValueStream << c;
                SwitchMode(Mode::ElementValue, c);
                return;
            }

            break;
        case Mode::ElementValue:
            if(c == '<')
            {
                AssignElementValue();
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
    
    cout << "Element Stack Size : " << p_ElementStack.size() << endl;

    while(!p_ElementStack.empty())
    {
        doc.RootElements.emplace_back(p_ElementStack.top());
        p_ElementStack.pop();
    }

    return doc;
}
#endif