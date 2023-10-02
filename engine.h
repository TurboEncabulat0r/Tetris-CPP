#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <chrono>
#include <ctime>

typedef uint32_t u32;
using fn = void(void);
using fnc = void(char);

void DrawRectangle(int RectangleX, int RectangleY, int RectangleWidth, int RectangleHeight, u32 Color);

void DrawPixel(int X, int Y, u32 Color);

void onDraw(fn* func);

void onUpdate(fn* func);

void onKeyDown(fnc* func);

bool isKeyDown(char key);

float getTime();

int random(int min, int max);
