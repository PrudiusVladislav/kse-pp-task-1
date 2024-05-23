#include "string_utils.h"
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void terminate_string(char *str, const int length) {
    if (str == NULL || length < 0) {
        return;
    }
    str[length] = '\0';
}

char *malloc_string(const int size) {
    char *str = (char *)malloc((size + 1) * sizeof(char));
    terminate_string(str, size);
    return str;
}


char *trim_whitespace(char *str) {
    while(isspace(*str)) str++;

    if(*str == 0)
        return str;

    char *end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;

    end[1] = '\0';

    return str;
}

char *to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }

    return str;
}
