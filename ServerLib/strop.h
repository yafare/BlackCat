#ifndef _STROP_H
#define _STROP_H

#include <string>
#include <vector>

template <typename T>
struct general_const_strings
{
    static const T blank_str;
};

// input: "    \r\n\t1234   \r\n"
// output: "1234"
template <typename T>
T TrimLeft(const T& str)
{
    const T& sig = general_const_strings<T>::blank_str;
    typename T::size_type index = 0;
    typename T::size_type len = str.size();
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
    typename T::size_type len = str.size();
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

    auto add_to_vec = [&](T& tmp_str) {
        if (need_trim) {
            tmp_str = TrimString(tmp_str);
        }
        if (!tmp_str.empty() || need_null_str) {
            tmp.push_back(tmp_str);
        }
    };

    int start = 0;
    auto pos = input.find(splitter, start);
    while (pos != T::npos) {
        T tmp_str(input.substr(start, pos - start));
        add_to_vec(tmp_str);
        start = pos + 1;
        pos = input.find(splitter, start);
    }
    // last one
    if (start != (int)input.size()) {
        T tmp_str(input.substr(start));
        add_to_vec(tmp_str);
    }
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
