#pragma once
#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <iostream>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "wpm.hpp";

bool MenuVisible = true;
bool ConsoleVisible = false;

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
            if (ImGui::Button("Attach to Notepad"))
            {
                Attach("Notepad.exe");
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