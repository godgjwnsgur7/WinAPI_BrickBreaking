// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define _CRT_SECURE_NO_WARNINGS
// Windows 헤더 파일
#include <windows.h>
#include <windowsx.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <commdlg.h>
#include <stdio.h>

#include <vector>
#include <string>
#include <iostream>
#include <numbers>
#include <random>
#include <algorithm>

#pragma comment(lib, "Msimg32.lib")

#define M_PI       3.14159265358979323846   // pi
#define NOMINMAX
#undef max
#undef min

const int g_screenX = 1000;
const int g_screenY = 700;