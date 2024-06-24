
#include "CaesarCipher.h"
#include <string>
#include <windows.h>
#include <filesystem>


std::string getDllPath(const char *dllName)
{
    std::filesystem::path cwd = std::filesystem::absolute(R"(C:\C\kse-pp-task-1\src)");
    cwd /= dllName;
    return cwd.string();
}

function_ptr CaesarCipher::getFunction(const char *name)
{
    if (_handle == nullptr || _handle == INVALID_HANDLE_VALUE)
    {
        throw std::runtime_error("Encrypting library not loaded");
    }

    return reinterpret_cast<function_ptr>(GetProcAddress(_handle, name));
}


CaesarCipher::CaesarCipher() {
    _handle = LoadLibrary(getDllPath("encrypt.dll").c_str());
    if (_handle == nullptr || _handle == INVALID_HANDLE_VALUE)
    {
        throw std::runtime_error("Could not load 'encrypt.dll'");
    }
}

CaesarCipher::CaesarCipher(const int shift) {
    _shift = shift;
    _handle = LoadLibrary(getDllPath("encrypt.dll").c_str());
    if (_handle == nullptr || _handle == INVALID_HANDLE_VALUE)
    {
        throw std::runtime_error("Could not load 'encrypt.dll'");
    }
}

void CaesarCipher::setShift(const int shift) {
    _shift = shift;
}

int CaesarCipher::getShift() {
    return _shift;
}

char * CaesarCipher::encrypt(char *text) {
    if (_shift == 0)
    {
        throw std::runtime_error("Shift not set");
    }

    const function_ptr encrypt = getFunction("encrypt");
    return encrypt(text, _shift);
}

char * CaesarCipher::decrypt(char *text) {
    if (_shift == 0)
    {
        throw std::runtime_error("Shift not set");
    }

    const function_ptr decrypt = getFunction("decrypt");
    return decrypt(text, _shift);
}

