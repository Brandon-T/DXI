#ifndef UTILITIES_HPP_INCLUDED
#define UTILITIES_HPP_INCLUDED

#include <windows.h>
#include <algorithm>
#include "Resources/Resource.hpp"

bool ExtractResource(short resourceID, const char* outputFilename, const char* resName);

bool ExtractD3D9X_43(const char* PluginsPath);

bool FindD3DX9_43(const char* PluginsPath);

#endif // UTILITIES_HPP_INCLUDED
