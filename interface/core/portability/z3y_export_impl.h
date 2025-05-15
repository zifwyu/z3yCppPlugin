/**
* @file	z3y_export_impl.h
* @brief ��ƽ̨����Ľӿڵ�ʵ��
* @details
*	1������ƽ̨��صĺ꣬���������ĸ�ͷ�ļ����Դ�ʵ�ֿ�ƽ̨��
*	2����������ܼ��غ�ж��DLL�Ľӿں�����������ƽ̨����ֱ����ʵ�֣�ʹ�ú���ѡ��ִ�С�
* @note
*	������Ϊ�˿�ƽ̨����Windows��Unixƽ̨���еĺ��ͷ�ļ��ֿ�����
*	ʹ��ʱͨ����ѡ������ĸ�ƽ̨��ͷ�ļ�
*
* @version	1.0
* @author	������
* @date		2025.5.15
*/

#pragma once
#ifndef Z3Y_PORTABILITY_EXPORT_IMPL_H
#define Z3Y_PORTABILITY_EXPORT_IMPL_H

#include "z3y_export.h"

#ifdef _WIN32
#include "z3y_export_impl_win.h"
#else
#include "z3y_export_impl_unix.h"
#endif

#endif