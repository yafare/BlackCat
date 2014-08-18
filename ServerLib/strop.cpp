#include "strop.h"

template <> const std::string general_const_strings<std::string>::blank_str = " \t\r\n";
template <> const std::wstring general_const_strings<std::wstring>::blank_str = L" \t\r\n";
