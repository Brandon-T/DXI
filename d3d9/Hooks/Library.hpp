#ifndef PLATFORM_HPP_INCLUDED
#define PLATFORM_HPP_INCLUDED

#include <windows.h>
#include <string>
#include <cstdint>
#include <stdexcept>

class Library
{
    private:
        HMODULE Module;

    public:
        Library(std::string Library);
        ~Library();

        template<typename T>
        T FunctionAddress(std::string FunctionName);

        template<typename T>
        bool FunctionAddress(T &FunctionDefinition, std::string FunctionName);

        template<typename T, typename ...Args>
        auto CallFunction(void* Function, Args... args) -> decltype(reinterpret_cast<T>(Function)(args...));
};

template<typename T>
T Library::FunctionAddress(std::string FunctionName)
{
    T Result = reinterpret_cast<T>(GetProcAddress(this->Module, FunctionName.c_str()));
    return Result ? Result : throw std::runtime_error(FunctionName);
}

template<typename T>
bool Library::FunctionAddress(T &FunctionDefinition, std::string FunctionName)
{
    return (FunctionDefinition = this->FunctionAddress<T>(FunctionName));
}

template<typename T, typename ...Args>
auto Library::CallFunction(void* Function, Args... args) -> decltype(reinterpret_cast<T>(Function)(args...))
{
    return reinterpret_cast<T>(Function)(args...);
}

#endif // PLATFORM_HPP_INCLUDED
