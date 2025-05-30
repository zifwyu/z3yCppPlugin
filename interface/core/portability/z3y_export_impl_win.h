/**
* @file	z3y_export_impl_win.h
* @brief 实现了Windows平台下加载和卸载DLL的接口
* @note
*	这里是为了跨平台，插件框架声明了框架的加载和卸载DLL接口
*   然后在不同头文件中，使用平台API实现接口
*	使用时通过宏选择包含哪个平台的头文件
*
* @version	1.0
* @author	孙鹏宇
* @date		2025.5.15
*/

#pragma once
#ifndef Z3Y_PORTABILITY_EXPORT_IMPL_WIN_H
#define Z3Y_PORTABILITY_EXPORT_IMPL_WIN_H

#include "z3y_win.h"

HMODULE z3yLoadLibrary(const char* file_name)
{
    // LOAD_WITH_ALTERED_SEARCH_PATH，让系统DLL搜索顺序从DLL所在目录开始。
    HMODULE handle_dll = LoadLibraryExA(file_name, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

    // 在DLL中找到z3yInitPlugin的函数地址，并调用
    // 作用是初始化插件，若初始化失败，就卸载插件
    // todo z3yInitPlugin中具体做了什么 为什么只有GUN编译器才需要，msvc为什么不用？
#ifdef __GNUC__
    using FuncPtr = bool (*)(HMODULE, HMODULE);

    FuncPtr p_func_z3yInitPlugin = (FuncPtr)GetProcAddress(handle_dll, "z3yInitPlugin");

    if (p_func_z3yInitPlugin && !p_func_z3yInitPlugin(handle_dll, nullptr))
    {
        z3yFreeLibrary(handle_dll);
        handle_dll = nullptr;
    }
#endif

    return handle_dll;
}

bool z3yFreeLibrary(HMODULE handle_dll)
{
    // todo z3yFreePlugin中具体做了什么 为什么只有GUN编译器才需要，msvc为什么不用？
#ifdef __GNUC__
    using FuncPtr = void (*)();
    FuncPtr p_func_z3yFreePlugin = (FuncPtr)GetProcAddress(handle_dll, "z3yFreePlugin");
    if (p_func_z3yFreePlugin)
    {
        p_func_z3yFreePlugin();
    }
#endif
    return !!FreeLibrary(handle_dll);
}

#endif