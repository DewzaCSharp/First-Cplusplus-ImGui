#pragma once
#ifndef WPM_HPP
#define WPM_HPP

#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <tlhelp32.h>
#include "MemoryReader.hpp";

using namespace std;

// Funktion, um Fenster eines bestimmten Prozesses zu finden
HWND FindWindowByProcessID(DWORD processID) {
    HWND hwnd = GetTopWindow(NULL); // Startpunkt: Erstes Fenster

    while (hwnd != NULL) {
        DWORD windowProcessID = 0;
        GetWindowThreadProcessId(hwnd, &windowProcessID);

        // Prozess-ID abgleichen
        if (windowProcessID == processID) {
            return hwnd;
        }

        hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
    }

    return NULL; // Kein Fenster gefunden
}

// Funktion, um den HWND eines Prozesses anhand des Prozessnamens zu finden
HWND GetWindowByProcessName(const std::wstring& processName) {
    HWND hwnd = NULL;

    // Schnappschuss aller Prozesse erstellen
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Fehler beim Erstellen des Snapshots!" << std::endl;
        return NULL;
    }

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    // Prozesse durchsuchen
    if (Process32FirstW(hSnapshot, &pe32)) {
        do {
            // Prozessnamen vergleichen (Unicode-Version)
            if (_wcsicmp(pe32.szExeFile, processName.c_str()) == 0) {
                DWORD processID = pe32.th32ProcessID;

                // Fenster suchen, das zu diesem Prozess gehört
                hwnd = FindWindowByProcessID(processID);
                break;
            }
        } while (Process32NextW(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return hwnd;
}

void Attach(const std::string procname)
{
    int readTest = 0;

    std::wstring wprocname(procname.begin(), procname.end());
    // Hier suchen wir nach Notepad (oder einem anderen Prozess, z. B. "notepad.exe")
    HWND hwnd = GetWindowByProcessName(wprocname); // Unicode-Literal für "notepad.exe"

    // Check if HWND found the Window
    if (hwnd == NULL) {
        cout << "Can't find Process." << endl;
        Sleep(2000); // Sleep 2 seconds
        exit(-1); // Exit the program if it did not find the Window
    }
    else {
        DWORD procID; // A 32-bit unsigned integer, DWORDS are mostly used to store Hexadecimal Addresses
        GetWindowThreadProcessId(hwnd, &procID); // Getting our Process ID, as an ex. like 000027AC
        HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID); // Opening the Process with All Access

        if (procID == NULL) {
            cout << "Can't find Process." << endl;
            Sleep(2000); // Sleep 2 seconds
            exit(-1); // Exit the program if it did not find the Window
        }
        else
        {
            // ReadProcessMemory(handle, (PBYTE*)0x061E9A18, &readTest, sizeof(readTest), 0);
            cout << readTest << endl;
            cout << "Found Process" << endl;
            MemoryReader memoryReader(handle);
            
            LPVOID address = reinterpret_cast<LPVOID>(0x061E9A18);
            
            int intValue = memoryReader.ReadMemory<int>(address);
            float floatValue = memoryReader.ReadMemory<float>(address);
            string stringValue = memoryReader.ReadMemory<string>(address);
            
            std::cout << "Read int: " << intValue << std::endl;
            std::cout << "Read float: " << floatValue << std::endl;
            std::cout << "Read string: " << stringValue << std::endl;
        }
    }
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

#endif
