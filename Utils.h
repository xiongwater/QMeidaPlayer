#pragma once

#include <WTypes.h>
#include <string>

WCHAR * charToWchar(char *s);

char* WCharToChar(WCHAR *s);

bool fileIsExits(std::string filePathName);

bool ensureDirectory(std::string path);
