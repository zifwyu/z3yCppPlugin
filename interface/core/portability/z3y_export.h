/**
* @file	z3y_export.h
* @brief 根据平台相关的宏，决定包含哪个头文件，以此实现跨平台
* @note
*	这里是为了跨平台，将Windows和Unix平台特有的宏和头文件分开处理。
*	使用时通过宏选择包含哪个平台的头文件
*
* @version	1.0
* @author	孙鹏宇
* @date		2025.5.14
*/

#pragma once
#ifndef Z3Y_EXPORT_PORTABILITY_H
#define Z3Y_EXPORT_PORTABILITY_H

#if defined(_WIN64) && !defined(_WIN32)
#define _WIN32
#endif

#ifdef _WIN32
#include "z3y_win.h"
#else
#include "z3y_unix.h"
#endif

HMODULE z3yLoadLibrary(const char* file_name);
bool z3yFreeLibrary(HMODULE hdll);

#endif // !Z3Y_EXPORT_PORTABILITY_H
