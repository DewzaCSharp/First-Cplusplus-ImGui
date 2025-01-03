#pragma once
#ifndef FINDWINDOWBYPARTIALTITLE_HPP
#define FINDWINDOWBYPARTIALTITLE_HPP

#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>


bool Contains(const std::string& str, const std::string& substr) {
	return str.find(substr) != std::string::npos;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	char windowTitle[256];
	GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));

	// Überprüfen, ob "Notepad" im Fenstertitel enthalten ist
	std::string title = windowTitle;
	if (Contains(title, "Notepad")) {
		std::cout << "Gefundenes Fenster: " << title << std::endl;
		*(HWND*)lParam = hwnd;
		return FALSE; // Stoppt die Enumeration, wenn das Fenster gefunden wurde
	}

	return TRUE; // Fortfahren mit der Enumeration
}

HWND FindWindowByPartialTitle(const std::string& partialTitle) {
	HWND hwnd = NULL;
	EnumWindows(EnumWindowsProc, (LPARAM)&hwnd);
	return hwnd;
}


#endif