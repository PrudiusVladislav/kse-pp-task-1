#include "StringUtils.h"
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <cstring>


void StringUtils::TerminateString(char *str, const int length) {
    if (str == nullptr || length < 0) {
        return;
    }
    str[length] = '\0';
}

char * StringUtils::CreateString(const int size) {
    const auto str = new char[size + 1];
    TerminateString(str, size);
    return str;
}

char * StringUtils::Trim(char *str) {
    while(isspace(*str)) str++;

    if(*str == 0)
        return str;

    char *end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;

    end[1] = '\0';

    return str;
}

char * StringUtils::ToLower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }

    return str;
}