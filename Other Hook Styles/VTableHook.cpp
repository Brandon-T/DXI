#include <iostream>
#include <windows.h>

std::uint32_t* GetVtableAddress(void* pObject)
{
    return reinterpret_cast<std::uint32_t*>(*(static_cast<std::uint32_t*>(pObject)));
}

void HookFunction(DWORD* pVtable, void* pHookProc, void* pOldProc, int iIndex)
{
    // Enable writing to the vtable at address we aquired
    DWORD lpflOldProtect;
    VirtualProtect((void*)&pVtable[iIndex], sizeof(DWORD), PAGE_READWRITE, &lpflOldProtect);

    // Store old address
    if (pOldProc) {
        *(DWORD*)pOldProc = pVtable[iIndex];
    }

    // Overwrite original address
    pVtable[iIndex] = reinterpret_cast<DWORD>(pHookProc);

    // Restore protection
    VirtualProtect(pVtable, sizeof(DWORD), lpflOldProtect, &lpflOldProtect);
}

class Test
{
    public:
        virtual void Foo() {std::cout << "Foo\n";}
        virtual void Meh() {std::cout << "Meh\n";}
};

typedef void (__cdecl *ptr_Foo)(Test* t);
typedef void (__cdecl *ptr_Meh)(Test* t);

void Foo(Test* ptr)
{
    std::cout<<"Foo Hooked\n";
}

int main()
{
    Test T;
    std::uint32_t* Ptr = GetVtableAddress(&T);
    HookFunction(Ptr, &Foo, &T.Foo, 0);
}
