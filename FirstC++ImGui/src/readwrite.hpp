#ifndef READWRITE_HPP
#define READWRITE_HPP

#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include <type_traits>

namespace readwrite {

    DWORD getProcessIdByName(const std::wstring& processName) {
        DWORD processId = 0;
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 processEntry;
            processEntry.dwSize = sizeof(processEntry);
            if (Process32First(snapshot, &processEntry)) {
                do {
                    if (processName == processEntry.szExeFile) {
                        processId = processEntry.th32ProcessID;
                        break;
                    }
                } while (Process32Next(snapshot, &processEntry));
            }
        }
        CloseHandle(snapshot);
        return processId;
    }

    DWORD_PTR getModuleBaseAddress(DWORD pid, const std::wstring& moduleName) {
        DWORD_PTR moduleBaseAddress = 0;
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
        if (snapshot != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 moduleEntry;
            moduleEntry.dwSize = sizeof(moduleEntry);
            if (Module32First(snapshot, &moduleEntry)) {
                do {
                    if (moduleName == moduleEntry.szModule) {
                        moduleBaseAddress = reinterpret_cast<DWORD_PTR>(moduleEntry.modBaseAddr);
                        break;
                    }
                } while (Module32Next(snapshot, &moduleEntry));
            }
        }
        CloseHandle(snapshot);
        return moduleBaseAddress;
    }

    template<typename T>
    T read(HANDLE hProcess, DWORD_PTR address) {
        static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
        T value{};
        SIZE_T bytesRead;
        if (ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(address), &value, sizeof(T), &bytesRead) && bytesRead == sizeof(T)) {
            return value;
        }
        return T{}; // Fehler: Kein Wert gelesen
    }

    template<typename T>
    bool write(HANDLE hProcess, DWORD_PTR address, const T& value) {
        static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
        SIZE_T bytesWritten;
        return WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(address), &value, sizeof(T), &bytesWritten) && bytesWritten == sizeof(T);
    }

} // namespace readwrite

#endif // READWRITE_HPP