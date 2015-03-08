#ifndef _STROP_H
#define _STROP_H

#include <string>
#include <vector>

template <typename T>
struct general_const_strings
{
    static const T blank_str;
    static const typename T::value_type quote;
};

// FUCK GCC...
#ifdef _MSC_VER
    #define WEAK_SYMBOL __declspec(selectany)
#else
    #define WEAK_SYMBOL __attribute__((weak))
#endif // _MSC_VER

template<>
WEAK_SYMBOL const std::string general_const_strings<std::string>::blank_str = " \t\r\n";
template<>
WEAK_SYMBOL const std::wstring general_const_strings<std::wstring>::blank_str = L" \t\r\n";
template<>
WEAK_SYMBOL const std::string::value_type general_const_strings<std::string>::quote = '"';
template<>
WEAK_SYMBOL const std::wstring::value_type general_const_strings<std::wstring>::quote = L'"';

// input: "    \r\n\t1234   \r\n"
// output: "1234"
template <typename T>
T TrimLeft(const T& str)
{
    const T& sig = general_const_strings<T>::blank_str;
    int index = 0;
    int len = (int)str.size();
    for (; index < len; index++) {
        if (sig.find(str[index]) == T::npos) {
            break;
        }
    }
    return str.substr(index, len - index);
}
template <typename T>
T TrimRight(const T& str)
{
    const T& sig = general_const_strings<T>::blank_str;
    int len = (int)str.size();
    int index = len - 1;
    for (; index >= 0; index--) {
        if (sig.rfind(str[index]) == T::npos) {
            break;
        }
    }
    return str.substr(0, index + 1);
}
template <typename T>
T TrimString(const T& str)
{
    return TrimLeft(TrimRight(str));
}

// input: "1,2,3,4,5,6", splitter: ','
// output: {"1", "2", "3", "4", "5", "6"}
template <typename T>
std::vector<T> SplitString(const T& input, typename T::value_type splitter, bool need_null_str = false, bool need_trim = true)
{
    std::vector<T> tmp;

    auto add_to_vec = [&](const T& tmp_str) {
        T str(tmp_str);
        if (need_trim) {
            str = TrimString(str);
        }
        if (!str.empty() || need_null_str) {
            tmp.push_back(str);
        }
    };

    auto start = 0;
    auto pos = input.find(splitter, start);
    while (pos != T::npos) {
        T tmp_str(input.substr(start, pos - start));
        add_to_vec(tmp_str);
        start = pos + 1;
        pos = input.find(splitter, start);
    }
    // last one
    if (start != input.size()) {
        T tmp_str(input.substr(start));
        add_to_vec(tmp_str);
    } else {
        add_to_vec(T());
    }
    return tmp;
}

template <typename T>
std::vector<T> SplitCsvString(const T& input, const typename T::value_type splitter)
{
    const typename T::value_type quote(general_const_strings<T>::quote);

    std::vector<T> tmp;
    auto add_to_vec = [&](const T& tmp_str) {
        auto str(TrimString(tmp_str));
        tmp.push_back(str);
    };

    auto str = input.c_str();
    T value;
    while (*str != 0) {
        auto ch = *str++;
        switch (ch)
        {
        case quote:
            while (true) {
                ch = *str++;
                if (ch == quote || ch == 0) {
                    break;
                }
                value.push_back(ch);
            }
            break;
        case 0:
            add_to_vec(value);
            str--;
            break;
        default:
            if (ch == splitter) {
                add_to_vec(value);
                value.clear();
            } else {
                value.push_back(ch);
            }
            break;
        }
    }
    add_to_vec(value);
    return tmp;
}

// input: "abcdabcdabcdabcd", from: "ab", to: "cd"
// output: "cdcdcdcdcdcdcdcd"
template <typename T>
T ReplaceString(const T& input, const T& from, const T& to)
{
    if (from.empty() || from == to) {
        return input;
    }

    T tmp = input;
    auto start = 0;
    auto pos = tmp.find(from, start);
    while (pos != T::npos) {
        tmp.replace(pos, from.size(), to.c_str());
        start = pos + to.size();
        pos = tmp.find(from, start);
    }
    return tmp;
}

// input: "1234567890", start: "12", end: "90"
// output: "345678"
template <typename T>
T GetSubString(const T& input, const T& start, const T& end)
{
    if (start.empty() || end.empty()) {
        return T();
    }
    auto st = input.find(start);
    if (st == T::npos) {
        return T();
    }
    auto ed_from = st + start.size();
    auto ed = input.find(end, ed_from);
    if (ed == T::npos) {
        return T();
    }
    return input.substr(st + start.size(), ed - ed_from);
}

#endif  // _STROP_H
