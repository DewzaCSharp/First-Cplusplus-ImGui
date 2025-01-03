#pragma once

#ifndef MEMORYREADER_HPP
#define MEMORYREADER_HPP

#include <iostream>
#include <Windows.h>
#include <psapi.h>
#include <string>

class MemoryReader {
public:
    MemoryReader(HANDLE hProcess) : processHandle(hProcess) {}

    template <typename T>
    T ReadMemory(LPVOID address) {
        T value;
        if (!ReadProcessMemory(processHandle, address, &value, sizeof(T), NULL)) {
            std::cerr << "Failed to read memory!" << std::endl;
            return T();
        }
        return value;
    }

private:
    HANDLE processHandle;
};

DWORD GetProcessIdByName(const std::wstring& processName) {
    DWORD processIds[1024], bytesNeeded;
    if (!EnumProcesses(processIds, sizeof(processIds), &bytesNeeded)) {
        std::cerr << "Failed to enumerate processes." << std::endl;
        return 0;
    }

    // Berechne die Anzahl der Prozesse
    unsigned int processCount = bytesNeeded / sizeof(DWORD);

    for (unsigned int i = 0; i < processCount; ++i) {
        DWORD processId = processIds[i];
        if (processId == 0)
            continue;

        // Öffne den Prozess
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
        if (hProcess) {
            TCHAR processNameBuffer[MAX_PATH];
            if (GetModuleBaseName(hProcess, NULL, processNameBuffer, sizeof(processNameBuffer) / sizeof(TCHAR))) {
                if (processName == processNameBuffer) {
                    CloseHandle(hProcess);
                    return processId; // Prozess-ID gefunden
                }
            }
            CloseHandle(hProcess);
        }
    }

    return 0; // Prozess nicht gefunden
}

#endif