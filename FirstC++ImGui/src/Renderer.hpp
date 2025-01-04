#pragma once
#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <iostream>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "wpm.hpp"
#include "main.cpp"
#include "readwrite.hpp"

bool MenuVisible = true;
bool ConsoleVisible = false;

std::string wstringToString(const std::wstring& wstr) {
    if (wstr.empty()) return {};
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string strTo(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], sizeNeeded, nullptr, nullptr);
    return strTo;
}

void Render()
{
    if (GetKeyState(VK_RSHIFT) & 0x8000)
    {
        system("echo [i] Changed ImGui Visibility");
        MenuVisible = !MenuVisible;

        sleepfor(100);
    }
    if (MenuVisible)
    {
        ImGui::SetNextWindowSize(ImVec2(600, 400));
        if (ImGui::Begin("MainWindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
        {
            ImGui::SetCursorPos(ImVec2(260, 10));
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Project1 c++");
            ImGui::SetCursorPos(ImVec2(560, 10));
            if (ImGui::Button("X", ImVec2(30, 30)))
            {
                exit(0);
            }
            if (ImGui::Button("Attach to CombatMaster"))
            {
                Attach("CombatMaster.exe");
                std::wstring processName = L"CombatMaster.exe";
                std::wstring moduleName = L"GameAssembly.dll";
                DWORD_PTR offset = 0x061E9A18;

                // Prozess-ID anhand des Namens ermitteln
                DWORD pid = readwrite::getProcessIdByName(processName);
                if (pid == 0) {
                    std::cerr << "Fehler: Prozess \"" << wstringToString(processName) << "\" nicht gefunden." << std::endl;
                    return;
                }

                HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, pid);
                if (hProcess) {
                    DWORD_PTR moduleBase = readwrite::getModuleBaseAddress(pid, moduleName);
                    DWORD_PTR address = moduleBase + offset;

                    // Beispiel: Lesen und Schreiben von Werten
                    int intValue = readwrite::read<int>(hProcess, address);
                    float floatValue = readwrite::read<float>(hProcess, address);
                    std::cout << "int: " << intValue << std::endl;
                    std::cout << "float: " << floatValue << std::endl;

                    CloseHandle(hProcess);
                }
                else {
                    std::cerr << "Fehler beim Öffnen des Prozesses: " << GetLastError() << std::endl;
                }
            }
            ImGui::SetCursorPos(ImVec2(470, 360));
            if (ImGui::Checkbox("Show Console", &ConsoleVisible))
            {
                system("echo [i] Console Visibility Changed.");
            }
        }ImGui::End();
    }
    if (ConsoleVisible)
    {
        ::ShowWindow(::GetConsoleWindow(), SW_SHOW);

    }
    else if (!ConsoleVisible)
    {
        ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    }
}

#endif 