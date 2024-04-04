.PHONY: run

run: a.exe
	a.exe

a.exe: main.cc main_windows.cc Backend_Win32_Gdi.h Backend_Win32_Gdi.cc
	clang++ main.cc -lgdi32 -luser32 -DONE_SOURCE -DUNICODE
