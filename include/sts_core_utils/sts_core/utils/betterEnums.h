
/// source:
/// https://stackoverflow.com/questions/28828957/enum-to-string-in-modern-c11-c14-c17-and-future-c20
///
/// FGN:
/// i changed the map to "static" and all other internal class functions to "inline"
/// that way we can access the enum normally via different compilation units (.cpp's)
/// without getting "multiple definition of" errors from the linker

/*

######################################################################################
EXPLANATION:
######################################################################################
This is similar to Yuri Finkelstein; but does not required boost. I am using a map so you can assign any value to the enums, any order.
Declaration of enum class as:
DECLARE_ENUM_WITH_TYPE(TestEnumClass, int32_t, ZERO = 0x00, TWO = 0x02, ONE = 0x01, THREE = 0x03, FOUR);

The following code will automatically create the enum class and overload:
    '+' '+=' for std::string
    '<<' for streams
    '~' just to convert to string (Any unary operator will do, but I personally don't like it for clarity)
    '*' to get the count of enums

No boost required, all required functions provided.
######################################################################################
EXAMPLE:
######################################################################################
DECLARE_ENUM_WITH_TYPE(TestEnumClass, int32_t, ZERO = 0x00, TWO = 0x02, ONE = 0x01, THREE = 0x03, FOUR);

int main(void) {
    TestEnumClass first, second;
    first = TestEnumClass::FOUR;
    second = TestEnumClass::TWO;

    std::cout << first << "(" << static_cast<uint32_t>(first) << ")" << std::endl; // FOUR(4)

    std::string strOne;
    strOne = ~first;
    std::cout << strOne << std::endl; // FOUR

    std::string strTwo;
    strTwo = ("Enum-" + second) + (TestEnumClass::THREE + "-test");
    std::cout << strTwo << std::endl; // Enum-TWOTHREE-test

    std::string strThree("TestEnumClass: ");
    strThree += second;
    std::cout << strThree << std::endl; // TestEnumClass: TWO
    std::cout << "Enum count=" << *first << std::endl;
}
######################################################################################
*/

#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_map>
#include <sstream>
#include <string>
#include <vector>

#define STRING_REMOVE_CHAR(str, ch) str.erase(std::remove(str.begin(), str.end(), ch), str.end())

static std::vector<std::string> splitString(std::string str, char sep = ',') {
    std::vector<std::string> vecString;
    std::string item;

    std::stringstream stringStream(str);

    while (std::getline(stringStream, item, sep))
    {
        vecString.push_back(item);
    }

    return vecString;
}


//    inline size_t operator*(E enumTmp) { (void) enumTmp; return E##MapName.size(); }

#define DECLARE_ENUM_WITH_TYPE(E, T, ...)                                                                     \
    enum class E : T                                                                                          \
    {                                                                                                         \
        __VA_ARGS__                                                                                           \
    };                                                                                                        \
    static std::map<T, std::string> E##MapName(generateEnumMap<T>(#__VA_ARGS__));                                    \
    inline std::ostream &operator<<(std::ostream &os, E enumTmp)                                                     \
    {                                                                                                         \
        os << E##MapName[static_cast<T>(enumTmp)];                                                            \
        return os;                                                                                            \
    }                                                                                                         \
    inline T operator*(E enumTmp) {return static_cast<T>(enumTmp); }                                         \
    inline std::map<T, std::string>& operator!(E enumTmp) { (void)enumTmp; return E##MapName; }                                 \
    inline bool operator==(const E& lhs, const T& rhs){return static_cast<T>(lhs) == rhs;}       \
    inline bool operator==(const T& lhs, const E& rhs){return static_cast<T>(rhs) == lhs;}       \
    inline bool operator!=(const E& lhs, const T& rhs){return !(lhs == rhs);}                                     \
    inline bool operator!=(const T& lhs, const E& rhs){return !(lhs == rhs);}                                     \
    inline std::string operator~(E enumTmp) { return E##MapName[static_cast<T>(enumTmp)]; }                          \
    inline std::string operator+(std::string &&str, E enumTmp) { return str + E##MapName[static_cast<T>(enumTmp)]; } \
    inline std::string operator+(E enumTmp, std::string &&str) { return E##MapName[static_cast<T>(enumTmp)] + str; } \
    inline std::string &operator+=(std::string &str, E enumTmp)                                                      \
    {                                                                                                         \
        str += E##MapName[static_cast<T>(enumTmp)];                                                           \
        return str;                                                                                           \
    }                                                                                                         \
    inline E operator++(E &enumTmp)                                                                           \
    {                                                                                                         \
        auto iter = E##MapName.find(static_cast<T>(enumTmp));                                                 \
        if (iter == E##MapName.end() || std::next(iter) == E##MapName.end())                                  \
            iter = E##MapName.begin();                                                                        \
        else                                                                                                  \
        {                                                                                                     \
            ++iter;                                                                                           \
        }                                                                                                     \
        enumTmp = static_cast<E>(iter->first);                                                                \
        return enumTmp;                                                                                       \
    }                                                                                                         \
    inline bool valid##E(T value) { return (E##MapName.find(value) != E##MapName.end()); }

#define DECLARE_ENUM(E, ...) DECLARE_ENUM_WITH_TYPE(E, int32_t, __VA_ARGS__)
template <typename T>
std::map<T, std::string> generateEnumMap(std::string strMap)
{
    STRING_REMOVE_CHAR(strMap, ' ');
    STRING_REMOVE_CHAR(strMap, '(');

    std::vector<std::string> enumTokens(splitString(strMap));
    std::map<T, std::string> retMap;
    T inxMap;

    inxMap = 0;
    for (auto iter = enumTokens.begin(); iter != enumTokens.end(); ++iter)
    {
        // Token: [EnumName | EnumName=EnumValue]
        std::string enumName;
        //FGN: commented out because not used
        //#################
        //T enumValue;
        //#################
        if (iter->find('=') == std::string::npos)
        {
            enumName = *iter;
        }
        else
        {
            std::vector<std::string> enumNameValue(splitString(*iter, '='));
            enumName = enumNameValue[0];
            //inxMap = static_cast<T>(enumNameValue[1]);
            if (std::is_unsigned<T>::value)
            {
                inxMap = static_cast<T>(std::stoull(enumNameValue[1], 0, 0));
            }
            else
            {
                inxMap = static_cast<T>(std::stoll(enumNameValue[1], 0, 0));
            }
        }
        retMap[inxMap++] = enumName;
    }

    return retMap;
}
