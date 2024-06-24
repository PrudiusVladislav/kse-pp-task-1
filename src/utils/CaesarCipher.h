
#ifndef CEASARCIPHER_H
#define CEASARCIPHER_H
#include <windows.h>

typedef char*(*function_ptr)(char*, int);

class CaesarCipher {
public:
    CaesarCipher();
    explicit CaesarCipher(int shift);
    void setShift(int shift);
    int getShift();
    char *encrypt(char* text);
    char* decrypt(char* text);
private:
    int _shift;
    HINSTANCE _handle;
    function_ptr getFunction(const char *name);
};

#endif //CEASARCIPHER_H
