/**
* @file	z3y_export.h
* @brief 跨平台所需的函数和宏的头文件
* @details
*	1、根据平台相关的宏，决定包含哪个头文件，以此实现跨平台。
*	2、定义插件框架加载和卸载DLL的接口函数声明，各平台代码分别进行实现，使用宏来选择执行。
* @note
*	这里是为了跨平台，将Windows和Unix平台特有的宏和头文件分开处理。
*	使用时通过宏选择包含哪个平台的头文件
*
* @version	1.0
* @author	孙鹏宇
* @date		2025.5.14
*/

#pragma once
#ifndef Z3Y_PORTABILITY_EXPORT_H
#define Z3Y_PORTABILITY_EXPORT_H

#if defined(_WIN64) && !defined(_WIN32)
#define _WIN32
#endif

#ifdef _WIN32
#include "z3y_win.h"
#else
#include "z3y_unix.h"
#endif

HMODULE z3yLoadLibrary(const char* file_name);
bool z3yFreeLibrary(HMODULE handle_dll);

#endif // !Z3Y_PORTABILITY_EXPORT_H
