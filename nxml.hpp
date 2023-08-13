#pragma once
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <sstream>
#include <fstream>
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
        virtual string  ToString()      override {return "<?xml version=\"1.0\"?>";}
        virtual void    FromString(string str)    override {}
    };
    
    struct ElementWithAttribute
    {
        string ElementName;
        string AttributeName;
        string AttributeValue;
    };

    /// <summary>
    /// Base type for elements, every element will have N number of Elements, and a Name. 
    /// 0 Elements is valid
    /// </summary>
    struct Element : ISerializable
    {
        enum class Type
        {
            Invalid,
            Value,
            Complex
        };

        static Element Invalid;

        Element(Type elementType);

        const Type ElementType;

        string ElementName;
        string InnerValue;

        vector<Attribute> Attributes;
        vector<Element> InnerElements;

        Element&    operator[](const char* key);
        Element&    operator[](const ElementWithAttribute& search);

        virtual string  ToString()      override;
        virtual void    FromString(string str)    override {}
    };

    /// <summary>
    /// Container for declaration and root element
    /// </summary>
    struct Document : ISerializable
    {
        Declaration Decl;
        vector<Element> RootElements;

        Element& operator[](const char* key);

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

        stack<Element> p_ElementStack;
        stack<Attribute> p_AttributeStack;
        
        string GetModeName(Mode& mode);

        void SwitchMode(Mode newMode, char current);
        void ProcessCharacter(std::string& xmlString, int charIndex);

        void CreateElement(Element::Type elementType = Element::Type::Invalid);
        void CloseElement();
        void AssignElementValue();
        void CreateAttribute();
        void LogCurrentElementName();
        void LogCurrentAttributes();

        void ClearCurrentElement();
        void ClearCurrentAttribute();
    };

    static Document ParseString(string& input);
    
    namespace utils {
        static void CleanWhiteSpace(string& input);
        static string LoadFileAsString(const char* path);
        static void SaveStringToFile(const char* path, string& str);
    }
}
// All credit to https://github.com/nlohmann/json for these hideous helpful macros
#define NXML_EXPAND(x) x
#define NXML_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, NAME, ...) NAME
#define NXML_PASTE(...) NXML_EXPAND(NXML_GET_MACRO(__VA_ARGS__, \
        NXML_PASTE64, \
        NXML_PASTE63, \
        NXML_PASTE62, \
        NXML_PASTE61, \
        NXML_PASTE60, \
        NXML_PASTE59, \
        NXML_PASTE58, \
        NXML_PASTE57, \
        NXML_PASTE56, \
        NXML_PASTE55, \
        NXML_PASTE54, \
        NXML_PASTE53, \
        NXML_PASTE52, \
        NXML_PASTE51, \
        NXML_PASTE50, \
        NXML_PASTE49, \
        NXML_PASTE48, \
        NXML_PASTE47, \
        NXML_PASTE46, \
        NXML_PASTE45, \
        NXML_PASTE44, \
        NXML_PASTE43, \
        NXML_PASTE42, \
        NXML_PASTE41, \
        NXML_PASTE40, \
        NXML_PASTE39, \
        NXML_PASTE38, \
        NXML_PASTE37, \
        NXML_PASTE36, \
        NXML_PASTE35, \
        NXML_PASTE34, \
        NXML_PASTE33, \
        NXML_PASTE32, \
        NXML_PASTE31, \
        NXML_PASTE30, \
        NXML_PASTE29, \
        NXML_PASTE28, \
        NXML_PASTE27, \
        NXML_PASTE26, \
        NXML_PASTE25, \
        NXML_PASTE24, \
        NXML_PASTE23, \
        NXML_PASTE22, \
        NXML_PASTE21, \
        NXML_PASTE20, \
        NXML_PASTE19, \
        NXML_PASTE18, \
        NXML_PASTE17, \
        NXML_PASTE16, \
        NXML_PASTE15, \
        NXML_PASTE14, \
        NXML_PASTE13, \
        NXML_PASTE12, \
        NXML_PASTE11, \
        NXML_PASTE10, \
        NXML_PASTE9, \
        NXML_PASTE8, \
        NXML_PASTE7, \
        NXML_PASTE6, \
        NXML_PASTE5, \
        NXML_PASTE4, \
        NXML_PASTE3, \
        NXML_PASTE2, \
        NXML_PASTE1)(__VA_ARGS__))
#define NXML_PASTE2(func, v1) func(v1)
#define NXML_PASTE3(func, v1, v2) NXML_PASTE2(func, v1) NXML_PASTE2(func, v2)
#define NXML_PASTE4(func, v1, v2, v3) NXML_PASTE2(func, v1) NXML_PASTE3(func, v2, v3)
#define NXML_PASTE5(func, v1, v2, v3, v4) NXML_PASTE2(func, v1) NXML_PASTE4(func, v2, v3, v4)
#define NXML_PASTE6(func, v1, v2, v3, v4, v5) NXML_PASTE2(func, v1) NXML_PASTE5(func, v2, v3, v4, v5)
#define NXML_PASTE7(func, v1, v2, v3, v4, v5, v6) NXML_PASTE2(func, v1) NXML_PASTE6(func, v2, v3, v4, v5, v6)
#define NXML_PASTE8(func, v1, v2, v3, v4, v5, v6, v7) NXML_PASTE2(func, v1) NXML_PASTE7(func, v2, v3, v4, v5, v6, v7)
#define NXML_PASTE9(func, v1, v2, v3, v4, v5, v6, v7, v8) NXML_PASTE2(func, v1) NXML_PASTE8(func, v2, v3, v4, v5, v6, v7, v8)
#define NXML_PASTE10(func, v1, v2, v3, v4, v5, v6, v7, v8, v9) NXML_PASTE2(func, v1) NXML_PASTE9(func, v2, v3, v4, v5, v6, v7, v8, v9)
#define NXML_PASTE11(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) NXML_PASTE2(func, v1) NXML_PASTE10(func, v2, v3, v4, v5, v6, v7, v8, v9, v10)
#define NXML_PASTE12(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) NXML_PASTE2(func, v1) NXML_PASTE11(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)
#define NXML_PASTE13(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12) NXML_PASTE2(func, v1) NXML_PASTE12(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12)
#define NXML_PASTE14(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13) NXML_PASTE2(func, v1) NXML_PASTE13(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13)
#define NXML_PASTE15(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14) NXML_PASTE2(func, v1) NXML_PASTE14(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14)
#define NXML_PASTE16(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15) NXML_PASTE2(func, v1) NXML_PASTE15(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15)
#define NXML_PASTE17(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16) NXML_PASTE2(func, v1) NXML_PASTE16(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16)
#define NXML_PASTE18(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17) NXML_PASTE2(func, v1) NXML_PASTE17(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17)
#define NXML_PASTE19(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18) NXML_PASTE2(func, v1) NXML_PASTE18(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18)
#define NXML_PASTE20(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19) NXML_PASTE2(func, v1) NXML_PASTE19(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19)
#define NXML_PASTE21(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20) NXML_PASTE2(func, v1) NXML_PASTE20(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20)
#define NXML_PASTE22(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21) NXML_PASTE2(func, v1) NXML_PASTE21(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21)
#define NXML_PASTE23(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22) NXML_PASTE2(func, v1) NXML_PASTE22(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22)
#define NXML_PASTE24(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23) NXML_PASTE2(func, v1) NXML_PASTE23(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23)
#define NXML_PASTE25(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24) NXML_PASTE2(func, v1) NXML_PASTE24(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24)
#define NXML_PASTE26(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25) NXML_PASTE2(func, v1) NXML_PASTE25(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25)
#define NXML_PASTE27(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26) NXML_PASTE2(func, v1) NXML_PASTE26(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26)
#define NXML_PASTE28(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27) NXML_PASTE2(func, v1) NXML_PASTE27(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27)
#define NXML_PASTE29(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28) NXML_PASTE2(func, v1) NXML_PASTE28(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28)
#define NXML_PASTE30(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29) NXML_PASTE2(func, v1) NXML_PASTE29(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29)
#define NXML_PASTE31(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30) NXML_PASTE2(func, v1) NXML_PASTE30(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30)
#define NXML_PASTE32(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31) NXML_PASTE2(func, v1) NXML_PASTE31(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31)
#define NXML_PASTE33(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32) NXML_PASTE2(func, v1) NXML_PASTE32(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32)
#define NXML_PASTE34(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33) NXML_PASTE2(func, v1) NXML_PASTE33(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33)
#define NXML_PASTE35(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34) NXML_PASTE2(func, v1) NXML_PASTE34(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34)
#define NXML_PASTE36(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35) NXML_PASTE2(func, v1) NXML_PASTE35(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35)
#define NXML_PASTE37(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36) NXML_PASTE2(func, v1) NXML_PASTE36(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36)
#define NXML_PASTE38(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37) NXML_PASTE2(func, v1) NXML_PASTE37(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37)
#define NXML_PASTE39(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38) NXML_PASTE2(func, v1) NXML_PASTE38(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38)
#define NXML_PASTE40(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39) NXML_PASTE2(func, v1) NXML_PASTE39(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39)
#define NXML_PASTE41(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40) NXML_PASTE2(func, v1) NXML_PASTE40(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40)
#define NXML_PASTE42(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41) NXML_PASTE2(func, v1) NXML_PASTE41(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41)
#define NXML_PASTE43(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42) NXML_PASTE2(func, v1) NXML_PASTE42(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42)
#define NXML_PASTE44(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43) NXML_PASTE2(func, v1) NXML_PASTE43(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43)
#define NXML_PASTE45(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44) NXML_PASTE2(func, v1) NXML_PASTE44(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44)
#define NXML_PASTE46(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45) NXML_PASTE2(func, v1) NXML_PASTE45(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45)
#define NXML_PASTE47(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46) NXML_PASTE2(func, v1) NXML_PASTE46(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46)
#define NXML_PASTE48(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47) NXML_PASTE2(func, v1) NXML_PASTE47(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47)
#define NXML_PASTE49(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48) NXML_PASTE2(func, v1) NXML_PASTE48(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48)
#define NXML_PASTE50(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49) NXML_PASTE2(func, v1) NXML_PASTE49(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49)
#define NXML_PASTE51(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50) NXML_PASTE2(func, v1) NXML_PASTE50(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50)
#define NXML_PASTE52(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51) NXML_PASTE2(func, v1) NXML_PASTE51(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51)
#define NXML_PASTE53(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52) NXML_PASTE2(func, v1) NXML_PASTE52(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52)
#define NXML_PASTE54(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53) NXML_PASTE2(func, v1) NXML_PASTE53(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53)
#define NXML_PASTE55(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54) NXML_PASTE2(func, v1) NXML_PASTE54(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54)
#define NXML_PASTE56(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55) NXML_PASTE2(func, v1) NXML_PASTE55(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55)
#define NXML_PASTE57(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56) NXML_PASTE2(func, v1) NXML_PASTE56(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56)
#define NXML_PASTE58(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57) NXML_PASTE2(func, v1) NXML_PASTE57(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57)
#define NXML_PASTE59(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58) NXML_PASTE2(func, v1) NXML_PASTE58(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58)
#define NXML_PASTE60(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59) NXML_PASTE2(func, v1) NXML_PASTE59(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59)
#define NXML_PASTE61(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60) NXML_PASTE2(func, v1) NXML_PASTE60(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60)
#define NXML_PASTE62(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61) NXML_PASTE2(func, v1) NXML_PASTE61(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61)
#define NXML_PASTE63(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62) NXML_PASTE2(func, v1) NXML_PASTE62(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62)
#define NXML_PASTE64(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62, v63) NXML_PASTE2(func, v1) NXML_PASTE63(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62, v63)


#ifdef NXML_IMPL

#define NXML_ASSERT(exp, msg) assert(((void)msg, exp))

nxml::Parser::Parser()
{
    p_Mode = Parser::Mode::Declaration;
}

nxml::Element::Element(Element::Type type) : ElementType(type)
{

}

nxml::Element nxml::Element::Invalid = nxml::Element(Element::Type::Invalid);

nxml::Element&  nxml::Element::operator[](const char* key)
{
    for (Element& e : InnerElements)
    {
        if (e.ElementName == std::string(key)) return e;
    }
    return Element::Invalid;
}

nxml::Element& nxml::Element::operator[](const ElementWithAttribute& search)
{
    for (Element& e : InnerElements)
    {
        if (e.ElementName == std::string(search.ElementName))
        {
            for (Attribute& attr : e.Attributes)
            {
                if (attr.Key == search.AttributeName && attr.SerializedValue == search.AttributeValue)
                {
                    return e;
                }
            }
        }
    }
    return Element::Invalid;
}

std::string nxml::Element::ToString()
{
    if (ElementType == Type::Invalid)
    {
        return std::string();
    }

    stringstream s;
    s << '<' << ElementName;
    for (Attribute& attr : Attributes)
    {
        s << ' ' << attr.Key << '=' << '"' << attr.SerializedValue << '"';
    }
    s << '>'; 
    if (ElementType == Type::Complex)
    {
        for (auto& inner : InnerElements)
        {
            s << inner.ToString();
        }

        s << "</" << ElementName << '>';
        return s.str();
    }

    s << '<' << ElementName;
    for (Attribute& attr : Attributes)
    {
        s << ' ' << attr.Key << '=' << '"' << attr.SerializedValue << '"';
    }
    s << '>' << InnerValue << "</" << ElementName << '>';
    return s.str();

}

nxml::Element& nxml::Document::operator[](const char* key)
{
    for (Element& e : RootElements)
    {
        if (e.ElementName == std::string(key)) return e;
    }
    return Element::Invalid;
}

std::string nxml::Document::ToString()
{
    stringstream s;
    s << Decl.ToString();
    for (auto& e: RootElements)
    {
        s << e.ToString();
    }

    

    string docStringRaw = s.str();
    utils::CleanWhiteSpace(docStringRaw);
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

void nxml::Parser::CreateElement(Element::Type elementType)
{
    string elementName = p_ElementNameStream.str();

    p_ElementStack.emplace(Element(elementType));
    p_ElementStack.top().ElementName = elementName;

    while (!p_AttributeStack.empty())
    {
        auto& e = p_ElementStack.top();
        e.Attributes.push_back(p_AttributeStack.top());
        p_AttributeStack.pop();
    }
}

void nxml::Parser::CloseElement()
{
    Element e = p_ElementStack.top();
    p_ElementStack.pop();

    if (p_ElementStack.empty())
    {
        p_ElementStack.push(e);
        return;
    }

    Element& parent = p_ElementStack.top();

    parent.InnerElements.emplace_back(e);
}

void nxml::Parser::AssignElementValue()
{
    auto& e = p_ElementStack.top();
    e.InnerValue = p_ElementValueStream.str();
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
    // cout << "        Switch Mode : Current Mode : " << GetModeName(p_Mode) << ", New Mode : " << GetModeName(newMode) << ", From Character : '" << current << "'\n";
    p_Mode = newMode;
}

void nxml::Parser::ProcessCharacter(std::string& xmlString, int charIndex)
{
    uint32_t nextIndex = static_cast<uint32_t>(charIndex + 1);

    if (nextIndex == xmlString.size())
    {
        return;
    }
    char c  = xmlString.at(charIndex);
    char nc = xmlString.at(nextIndex);

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
                CreateElement(Element::Type::Complex);
                ClearCurrentElement();
                SwitchMode(Mode::ElementOpen, c);
                return;
            }
            if(iswalnum(c))
            {
                CreateElement(Element::Type::Value);
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
    
    while(!p_ElementStack.empty())
    {
        doc.RootElements.emplace_back(p_ElementStack.top());
        p_ElementStack.pop();
    }

    return doc;
}

void nxml::utils::CleanWhiteSpace(std::string& input)
{
    static std::regex e("[ \t]+");   // matches trailing whitespace
    input.erase(std::remove(input.begin(), input.end(), '\r'), input.end());
    input.erase(std::remove(input.begin(), input.end(), '\n'), input.cend());
    input.erase(std::remove(input.begin(), input.end(), '\t'), input.end());

    input = std::regex_replace(input, e, " $2"); // replace all trailing space wioth single space.
    
}

std::string nxml::utils::LoadFileAsString(const char* path) {
    std::fstream file;
    file.open(path);

    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return str;
}

void nxml::utils::SaveStringToFile(const char* path, std::string & str) {
    std::ofstream out(path);
    out << str;
    out.close();
}

nxml::Document nxml::ParseString(std::string& input)
{
    nxml::Parser parser;
    return parser.GetFromString(input);
}

#endif