#pragma once
#include <Windows.h>

LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void InitWindow(HINSTANCE hInstance, int nCmdShow);