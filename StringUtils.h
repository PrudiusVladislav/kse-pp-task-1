//
// Created by User on 5/23/2024.
//

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

class StringUtils {
public:
    static void TerminateString(char *str, int length);
    [[nodiscard]] static char *CreateString(int size);
    static char *Trim(char *str);
    static char *ToLower(char *str);
};

#endif //STRING_UTILS_H
