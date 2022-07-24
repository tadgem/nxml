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
        virtual void    FromString() = 0;
    };

    /// <summary>
    /// structure to encapsulate tags in element definitions
    /// </summary>
    struct Tag : ISerializable
    {
        ValueType Type;

        string Key;
        string Value;
    };

    /// <summary>
    /// Probably dont need to change this...
    /// </summary>
    struct Declaration : ISerializable
    {
        const string VERSION = "1.0";

        virtual string  ToString()      override;
        virtual void    FromString()    override;
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

    /// <summary>
    /// Derived type for elements with a value. Cannot have any children.
    /// </summary>
    struct ValueElement : Element
    {
        ValueType Type;
        string ValueText;
    };

    /// <summary>
    /// Derived type for element which contains N number of child elements.
    /// </summary>
    struct ContainerElement : Element
    {
        vector<Element*> ChildElements;
    };

    struct Document
    {
        Declaration Decl;
        Element* Root;
    };

    class Parser
    {
    public:
        static Document GetFromString(const string& xml);
        static string   ToString(const Document& xml);
    };
}