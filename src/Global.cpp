#include <iostream>

using std::cout, std::endl, std::string;
namespace windows {
    #include "windows.h"
}

#include "Global.hpp"

void Error(std::string _Error){
    windows::MessageBoxA(NULL, (windows::LPCSTR)_Error.data(), (windows::LPCSTR)"Error", MB_ICONERROR | MB_OK);
}