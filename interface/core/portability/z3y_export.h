/**
* @file	z3y_export.h
* @brief ��ƽ̨����ĺ����ͺ��ͷ�ļ�
* @details
*	1������ƽ̨��صĺ꣬���������ĸ�ͷ�ļ����Դ�ʵ�ֿ�ƽ̨��
*	2����������ܼ��غ�ж��DLL�Ľӿں�����������ƽ̨����ֱ����ʵ�֣�ʹ�ú���ѡ��ִ�С�
* @note
*	������Ϊ�˿�ƽ̨����Windows��Unixƽ̨���еĺ��ͷ�ļ��ֿ�����
*	ʹ��ʱͨ����ѡ������ĸ�ƽ̨��ͷ�ļ�
*
* @version	1.0
* @author	������
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
