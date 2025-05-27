/**
* @file	class_macro.h
* @brief 定义了插件框架中关于类的元数据的一些宏
*
* @version	1.0
* @author	孙鹏宇
* @date		2025.5.22
*/

#pragma once
#ifndef Z3Y_CORE_CLASS_MECRO_H
#define Z3Y_CORE_CLASS_MECRO_H


/**
* @brief 实现类接口声明的开始
*/
#define Z3Y_BEGIN_IMPL_CLASS_DECLARE(impl_class_name, impl_class_id) \
public: \
	static const char* _GetImplClassID() { return impl_class_id;} \
	static const char* _GetImplClassName() {return #impl_class_name;} \
	static bool _QueryObject(const impl_class_name* self, z3y::InterfaceID interface_id, z3y::IObject** ppv) \
	{

/**
* @brief 当某个实现类只需要查询，不需要获取类的ID和名称时，使用本宏
* @todo 
*	具体什么场景下用？为什么有这个需求？应当时确定知道实现类不会被实例化，还有子类继承的时候使用。
*	有必要这样吗？多定义两个函数也没什么，还是防止使用者错误的将不该被实例化的实现类给实例化出来
*/
#define Z3Y_BEGIN_IMPL_CLASS_DECLARE0(impl_class_name, impl_class_id) \
	static bool _QueryObject(const impl_class_name* self, z3y::InterfaceID interface_id, z3y::IObject** ppv) \
	{

/**
* @brief 在实现类的头文件中，声明实现的接口
* @note
*	这里使用双重转换是为了防止继承多个接口时，指针的错误偏移
*	todo 这里要验证一下，要深度理解
*/
#define Z3Y_DEFINE_INTERFACE_ENTRY(interface_name) \
		if(interface_id == interface_name::GetInterfaceID()) \
		{ \
			if(self) \
			{ \
				*ppv = static_cast<z3y::IObject*>(static_cast<z3y::interface_name*>(self)); \
				(*ppv)->AddRef(); \
			} \
			return true; \
		}

/**
* @brief 当实现类的父类也是实现类的时候，使用此宏声明
*/
#define Z3Y_USE_BASE_IMPL_CLASS_INTERFACE_ENTRY(base_impl_class) \
		if(base_impl_class::_QueryObject(self, interface_id, ppv)) \
		{ \
			return true; \
		}

#define Z3Y_END_IMPL_CLASS_DECLARE() \
		return false; \
	} \
protected: //	实现类的函数不必是public

#endif // !Z3Y_CORE_CLASS_MECRO_H