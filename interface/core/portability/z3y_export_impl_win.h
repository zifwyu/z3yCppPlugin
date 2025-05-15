/**
* @file	z3y_export_impl_win.h
* @brief ʵ����Windowsƽ̨�¼��غ�ж��DLL�Ľӿ�
* @note
*	������Ϊ�˿�ƽ̨�������������˿�ܵļ��غ�ж��DLL�ӿ�
*   Ȼ���ڲ�ͬͷ�ļ��У�ʹ��ƽ̨APIʵ�ֽӿ�
*	ʹ��ʱͨ����ѡ������ĸ�ƽ̨��ͷ�ļ�
*
* @version	1.0
* @author	������
* @date		2025.5.15
*/

#pragma once
#ifndef Z3Y_PORTABILITY_EXPORT_IMPL_WIN_H
#define Z3Y_PORTABILITY_EXPORT_IMPL_WIN_H

#include "z3y_win.h"

HMODULE z3yLoadLibrary(const char* file_name)
{
	// LOAD_WITH_ALTERED_SEARCH_PATH����ϵͳDLL����˳���DLL����Ŀ¼��ʼ��
	HMODULE handle_dll = LoadLibraryExA(file_name, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

    // ��DLL���ҵ�z3yInitPlugin�ĺ�����ַ��������
    // �����ǳ�ʼ�����������ʼ��ʧ�ܣ���ж�ز��
    // todo z3yInitPlugin�о�������ʲô Ϊʲôֻ��GUN����������Ҫ��msvcΪʲô���ã�
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
    // todo z3yFreePlugin�о�������ʲô Ϊʲôֻ��GUN����������Ҫ��msvcΪʲô���ã�
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