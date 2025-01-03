#pragma once

#include <chrono>
#include <thread>
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include <cstdlib>
#ifndef MISC_HPP
#define MISC_HPP

void sleepfor(int miliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
}

#endif 