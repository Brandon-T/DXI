#include "Library.hpp"

Library::Library(std::string Library)
{
    this->Module = LoadLibrary(Library.c_str());
}

Library::~Library()
{
    FreeLibrary(this->Module);
}

/*
bool Library::xGetFunctionAddress(void* FunctionDefinition, std::string FunctionName)
{
    return *static_cast<std::int64_t(__stdcall **)()>(FunctionDefinition) = this->xGetProcAddress(FunctionName.c_str());
}
*/
