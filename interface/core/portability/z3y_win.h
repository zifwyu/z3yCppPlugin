/**
* @file	z3y_win.h
* @brief 定义了Windows平台下的一些宏和头文件包含
* @note
*	这里是为了跨平台，将Windows和Unix平台特有的宏和头文件分开处理
*   使用时通过宏选择包含哪个平台的头文件
*
* @version	1.0
* @author	孙鹏宇
* @date		2025.5.14
*/

#pragma once
#ifndef Z3Y_PORTABILITY_WIN_H
#define Z3Y_PORTABILITY_WIN_H

#ifdef _AFXDLL //当项目使用 ​MFC动态链接库（DLL模式）​​ 时，此分支生效
	#define VC_EXTRALEAN //精简MFC头文件内容，排除不常用组件以加快编译速度
	#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS //强制ATL的CString类使用显式构造函数，避免隐式转换导致的安全隐患
	#define _AFX_ALL_WARNINGS //启用所有MFC特有的编译器警告，帮助捕捉潜在问题
	#include <afxwin.h>         // MFC核心组件（如CWinApp、CWnd等）和标准控件
	#include <afxext.h>         // FC扩展功能（如工具栏、状态栏等高级界面组件）
	#ifndef _AFX_NO_OLE_SUPPORT
		#include <afxdisp.h>        // 仅在未禁用OLE支持时包含，提供MFC Automation类（用于COM交互）
	#endif
#else //当项目为 ​非MFC环境（如纯Win32 API或静态链接MFC）​​ 时，此分支生效
	#define _WIN32_LEAN_AND_MEAN // 排除Windows头文件中不常用的API定义，减少编译时间
	#include <windows.h>        // Windows API主头文件，包含所有基础Win32函数和数据结构
#endif // _AFXDLL

#ifdef _MSC_VER //当使用 ​Microsoft Visual C++（MSVC）编译器时，此分支生效
#include <shlwapi.h>        // <shlwapi.h>是Windows平台下轻量级工具库（Shell Lightweight Utility APIs）的头文件，提供路径处理、字符串操作等实用功能
#pragma comment(lib, "shlwapi.lib")
#endif

#ifdef __GNUC__ //当检测到GNU编译器（如GCC、Clang）时，此分支生效
#define Z3Y_EXPORT_API      extern "C" __attribute__((dllexport))
#else
#define Z3Y_EXPORT_API      extern "C" __declspec(dllexport)
#endif

#define Z3Y_LOCAL_API

#endif // !Z3Y_PORTABILITY_WIN_H
