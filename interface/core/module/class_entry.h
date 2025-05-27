/**
* @file	class_entry.h
* @brief 定义了插件类的导出帮助类，此类中包含实例化一个插件类的所有信息
*
* @version	1.0
* @author	孙鹏宇
* @date		2025.5.15
*/

#pragma once
#ifndef Z3Y_CORE_CLASS_ENTRY_H
#define Z3Y_CORE_CLASS_ENTRY_H

#include "../IObject.h"

Z3Y_BEGIN_NAMESPACE

using FuncPtrObjectCreator = IObject* (*)(InterfaceID interface_id);
using FuncPtrHasInterfaceID = bool (*)(InterfaceID interface_id);

// todo 为什么只定义了单例模式？为什么是10？
enum { MIN_SINGLETON_TYPE = 10 };

/**
* @brief 插件实现类的入口
* @details 本类的成员变量，包含了创建一个插件类实例所需的信息
*	通过这些变量中的信息，可以构造出一个插件类的对象
*/
class ClassEntry
{
public:
	int class_type; //类的类型，普通or单例
	const char* class_name;
	const char* impl_class_id;
	FuncPtrObjectCreator p_func_object_creator; //用来创建对象的函数指针
	FuncPtrHasInterfaceID p_func_has_interface_id; //判断是否实现了某个接口

	ClassEntry() : class_type(0), class_name(""), impl_class_id(""), p_func_object_creator(nullptr), p_func_has_interface_id(nullptr)
	{
	}

	ClassEntry(int _type, const char* _class_name,
		const char* _impl_class_id, FuncPtrObjectCreator _creator, FuncPtrHasInterfaceID _hasiid)
		: class_type(_type), class_name(_class_name), impl_class_id(_impl_class_id), p_func_object_creator(_creator), p_func_has_interface_id(_hasiid)
	{
	}

	// ClassEntry类对象组成的数组，存储了本DLL所有导出的插件类的构造信息
	// 静态变量，指针类型，指向ClassEntry类对象组成的数组的首地址
	// 本变量在此处声明，在DLL导出插件的cpp代码处进行定义
	// 本变量不会导出，只会被同一个DLL中的代码访问
	static const ClassEntry* const  classes[];
};

Z3Y_END_NAMESPACE

#endif // !Z3Y_CORE_CLASS_ENTRY_H
