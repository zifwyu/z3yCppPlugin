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

#include "../iobject.h"

Z3Y_BEGIN_NAMESPACE

using FuncPtrObjectCreator = IObject* (*)(const InterfaceID& interface_id);
using FuncPtrHasInterfaceID = bool (*)(const InterfaceID& interface_id);

/**
* @brief 枚举代表导出类的类型，普通or单例
* @note
*	匿名枚举的核心作用是将一组常量（如 kSingletonType）直接引入当前作用域（例如全局作用域、类作用域或命名空间），
*	使其像宏定义一样可直接使用，但具备类型安全和编译期检查的优势
*/
enum class ClassType
{
	kNormalType,
	kSingletonType
};

/**
* @brief 插件实现类的入口
* @details 本类的成员变量，包含了创建一个插件类实例所需的信息
*	通过这些变量中的信息，可以构造出一个插件类的对象
*/
class ClassEntry
{
public:
	ClassType class_type{ ClassType::kNormalType }; //类的类型，普通or单例
	const char* class_name;
	const char* impl_class_id;
	FuncPtrObjectCreator p_func_object_creator{ nullptr }; //用来创建对象的函数指针
	FuncPtrHasInterfaceID p_func_has_interface_id{ nullptr }; //判断是否实现了某个接口

	ClassEntry() : class_type(ClassType::kNormalType), class_name(""), impl_class_id(""), p_func_object_creator(nullptr), p_func_has_interface_id(nullptr)
	{
	}

	ClassEntry(ClassType _type, const char* _class_name,
		const char* _impl_class_id, FuncPtrObjectCreator _creator, FuncPtrHasInterfaceID _hasiid)
		: class_type(_type), class_name(_class_name), impl_class_id(_impl_class_id), p_func_object_creator(_creator), p_func_has_interface_id(_hasiid)
	{
	}

	// ClassEntry类对象组成的数组的数组，存储了本DLL所有导出的插件类的构造信息
	// 静态变量，二维指针数组类型，classes[]中每个元素都是ClassEntry*指针，指向ClassEntry类对象组成的数组或单个对象
	// 本数组在此处声明，在DLL导出插件的cpp代码处进行定义
	// 本数组不会导出，只会被同一个DLL中的代码访问
	// 二维指针数组是数组元素均为指针的数组，其本质是“数组的数组”，每个元素指向另一段内存（可能是一维数组或单个对象）
	// const ClassEntry*：表示指针指向的 ClassEntry 对象是常量（内容不可修改）
	// const classes[]：表示classes[]数组本身是常量（数组地址不可修改）
	static const ClassEntry* const  classes[];
};

Z3Y_END_NAMESPACE

#endif // !Z3Y_CORE_CLASS_ENTRY_H
