#pragma once

#define UNICODE

#include <windows.h>
#include <cstdlib>
#include <cstdint>

#include "state.h"
#include "backend_win32_gdi32.h"
#include "backend_win32_bitmap.h"

static RECT rct;

static LARGE_INTEGER liStart;
static LARGE_INTEGER liStop;

static bool use_gdi = false;

static INT height(RECT rect)
{
	return rect.bottom - rect.top;
}

static INT width(RECT rect)
{
	return rect.right - rect.left;
}

static void Step(HDC dc, State &state, UINT64 delta_ns)
{
	INT w = width(rct);
	INT h = height(rct);
	HDC memDC = CreateCompatibleDC(dc);
	HBITMAP memBM  = CreateCompatibleBitmap(dc, w, h);

	SelectObject(memDC, memBM);

	if (!use_gdi) {
		INT rw = ((w * 32 + 31) / 32);
		SIZE_T bmSize = rw * h * 4;
		static UINT8 *buf = (UINT8 *)malloc(bmSize);
		static SIZE_T bcBuf = bmSize;
		if (bmSize != bcBuf) {
			buf = (UINT8 *)realloc(buf, bmSize);
			bcBuf = bmSize;
		}

		Backend_Win32_Bitmap backend(buf, bcBuf, w, h, rw);

		state.update_and_draw(backend, delta_ns);

		SetBitmapBits(memBM, bmSize, buf);
	} else {
		Backend_Win32_Gdi32 backend(memDC, w, h);

		state.update_and_draw(backend, delta_ns);
	}

	BitBlt(dc, 0, 0, w, h, memDC, 0, 0, SRCCOPY);

	DeleteObject(memBM);
	DeleteDC(memDC);
}

static LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch(msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		GetClientRect(hwnd, &rct);
		break;
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int run(State &state)
{
	LPCWSTR CLASS_NAME = L"C3DE";
	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASSW wc = {
		.lpfnWndProc = WindowProc,
		.hInstance = hInstance,
		.hCursor = LoadCursorW(NULL, IDC_ARROW),
		.lpszClassName = CLASS_NAME,
	};
	RegisterClassW(&wc);

	HWND hwnd = CreateWindowExW(
		0,
		CLASS_NAME,
		L"C3DE",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (hwnd == NULL) {
		return -1;
	}

	HDC dc = GetDC(hwnd);

	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);

	if(!QueryPerformanceCounter(&liStart)) {
		MessageBoxW(hwnd, L"Couldn't get the initial time.", L"Error", 0);
		return -3;
	}

	MSG msg = {};
	while (true) {
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			if(!QueryPerformanceCounter(&liStop)) {
				MessageBoxW(hwnd, L"Couldn't get the step time.", L"Error", 0);
				return -4;
			}
			UINT64 delta_ns = liStop.QuadPart - liStart.QuadPart;
			liStart = liStop;
			Step(dc, state, delta_ns);
			Sleep(5);
		}
	}

	return 0;
}