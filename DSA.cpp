// DSA.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "src/Game.h"

#ifdef _MSC_VER
    // Always use standard main() even when SubSystem is set to Windows Desktop
#pragma comment(linker, "/ENTRY:mainCRTStartup")

#ifdef _DEBUG
    // Debug build: Show Console window alongside the Raylib Desktop window
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#else
    // Release build: Pure Desktop application (hides the black console window)
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#endif
#endif

int main()
{
    std::cout << "Starting Snake Desktop App..." << std::endl;

    Game game;
    game.Start();

    return 0;
}