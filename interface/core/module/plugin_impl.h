/**
* @file	plugin_impl.h
* @brief 定义了实现插件所需的一些初始化和反初始化函数等
* @warning 本头文件中定义了一些普通函数，因此本头文件在同一个工程下只能被一个CPP文件包含，否则会报重定义错误
*
* @version	1.0
* @author	孙鹏宇
* @date		2025.5.28
*/

#pragma once
#ifndef Z3Y_CORE_PLUGIN_IMPL_H
#define Z3Y_CORE_PLUGIN_IMPL_H

#include "../iobject.h"
#include "class_entry.h"
#include "singleton_manager.h"
#include "../portability/z3y_export_impl.h"
#include <atomic>

Z3Y_BEGIN_NAMESPACE

static HMODULE s_pln_hmod{ nullptr }; //插件DLL的句柄
static HMODULE s_pln_manager_hmod{ nullptr }; //插件管理器DLL的句柄
static bool s_load_pln_manager{ false }; //标记插件管理器是否由当前模块加载到进程的，只有加载插件管理器的模块才拥有其卸载权，其他模块只有使用权
static std::atomic<int32_t> s_pln_ref_count{ 0 }; //插件的引用计数

// 当没有插件管理器或者向插件管理器注册成功时，调用下面函数进行附加初始化和取消初始化
Z3Y_EXPORT_API bool z3yInitializePlugin();
Z3Y_EXPORT_API void z3yUninitializePlugin();

/**
* @brief 获取插件本身的句柄
* @note
*	此函数在插件内部使用，不会导出到DLL之外
* @warning
*	在头文件中定义的普通函数，只能被一个cpp文件包含，不会会报多重定义
*/
Z3Y_LOCAL_API HMODULE z3yGetModuleHandle()
{
	return s_pln_hmod;
}

/**
* @brief 获取插件管理器的句柄
* @note
*	虽然插件管理器是单独DLL，但各插件都可以拿到其句柄
*	此函数在插件内部使用，不会导出到DLL之外
* @warning
*	在头文件中定义的普通函数，只能被一个cpp文件包含，不会会报多重定义
*/
Z3Y_LOCAL_API HMODULE GetManagerHandle()
{
	return s_pln_manager_hmod;
}


/**
* @brief 获取插件中导出类中，某种类型的类的数量
* @param[in] class_type 需要查询的类型，普通or单例
* @return 导出类中该类型的数量
* @note
*	头文件中的static函数，每个包含该头文件的cpp文件，都会生成独立副本，可能造成代码冗余，尽量减少包含本头文件
*	这里为什么用static，因为要访问ClassEntry::classes，而ClassEntry::classes在每个插件中是不同的，所以需要每个插件生成副本
*	如果这里用inline，那么函数会在每个插件的ClassEntry::classes副本中，选择一个访问，就不是我们的目的
*/
static int GetClassCount(ClassType class_type)
{
	int count{ 0 };

	for (const ClassEntry* const* arr = ClassEntry::classes; *arr; ++arr)
	{
		for (const ClassEntry* cls = *arr; cls->p_func_object_creator != nullptr; ++cls)
		{
			if (cls->class_type == class_type)
			{
				count++;
			}
		}
	}

	return count;
}

/**
* @brief 获取导出类的唯一标识
* @param[in,out] export_class_ids 存放导出类唯一标识字符串的数组首地址,export_class_ids数组要提前开辟好内存，且元素个数必须要大于count
* @param[in] count	export_class_ids数组可写入的数量
* @return 返回export_class_ids，方便调用使用，无其他意义
*/
static const char** GetExportClassIDs(const char** export_class_ids, const int& count)
{
	int i{ 0 };
	for (const ClassEntry* const* arr = ClassEntry::classes; *arr && i < count; ++arr)
	{
		for (const ClassEntry* cls = *arr; cls->p_func_object_creator != nullptr && i < count; ++cls, ++i)
		{
			export_class_ids[i] = cls->impl_class_id;
		}
	}
	export_class_ids[i] = nullptr;
	return export_class_ids;
}

/**
* @brief 查找某个接口的默认实现类ID
* @param[in] interface_id 接口唯一标识
* @param[out] impl_class_id 接口实现类的唯一标识
* @retrun DLL中存在实现类继承了该接口，返回true，否则返回false
* @note
*	若查询的接口有多个实现类，则导出顺序返回第一个实现类的ID
*	若查询的接口没有被实现，则会遍历DLL中所有导出类，然后返回false
*/
static bool GetDefaultImplClassID(const InterfaceID& interface_id, const char*& impl_class_id)
{
	for (const ClassEntry* const* arr = ClassEntry::classes; *arr; ++arr)
	{
		for (const ClassEntry* cls = *arr; cls->p_func_object_creator != nullptr; ++cls)
		{
			if (cls->p_func_has_interface_id(interface_id))
			{
				impl_class_id = cls->impl_class_id;
				return true;
			}
		}
	}
	return false;
}

/**
* @brief 根据接口ID和实现类ID，创建实现类对象
* @param[in] impl_class_id 实现类ID
* @param[in] interface_id 接口ID
* @param[out] ppv 创建对象存放的地址
* @return 若创建成功返回true
* @todo 本函数的使用场景是什么，为什么之前没有见过调用？
*/
Z3Y_EXPORT_API bool z3yInternalCreateObject(const char* impl_class_id, const InterfaceID& interface_id, IObject** ppv)
{
	*ppv = nullptr;

	if (impl_class_id == nullptr || impl_class_id[0] == '\0')
	{
		if (!GetDefaultImplClassID(interface_id, impl_class_id))
		{
			return false;
		}
	}
	for (const ClassEntry* const* arr = ClassEntry::classes; *arr; ++arr)
	{
		for (const ClassEntry* cls = *arr; cls->p_func_object_creator != nullptr; ++cls)
		{
			if (strcmp(cls->impl_class_id, impl_class_id) == 0)
			{
				*ppv = cls->p_func_object_creator(interface_id);
				return *ppv != nullptr;
			}
		}
	}
	return false;
}

#ifndef Z3Y_EXPORT_CLASS_MAXCOUNT
#define Z3Y_EXPORT_CLASS_MAXCOUNT 64
#endif
#ifndef Z3Y_PLUGIN_MANAGER_NAME
#define Z3Y_PLUGIN_MANAGER_NAME "z3y_manager.pln"
#endif

/**
* @brief 初始化插件
* @param[in] pln_hmod 需要初始化的插件DLL的句柄
* @param[out] pln_manager_hmod 插件管理器的句柄，当输入为空时会自动加载插件管理器
* @return 返回初始化是否成功
* @note
*	1、初始化单例系统
*	2、初始化插件中所有被实现的接口的ID
*	3、向插件管理器注册插件
*	4、根据插件管理器注册结果，决定是否调用z3yInitializePlugin()进行附加初始化
*/
Z3Y_EXPORT_API bool z3yInitPlugin(HMODULE pln_hmod, HMODULE pln_manager_hmod)
{
	if (++s_pln_ref_count > 1)
	{
		return true;
	}

	// 单例系统初始化
	int singleton_class_count = GetClassCount(ClassType::kSingletonType);
	SingletonManager::Init(singleton_class_count);

	// 若查询的接口没有被实现，则会遍历DLL中所有导出类，然后返回false
	// 所有的DLL实现类都不可能继承IObject接口，因此查询IObject接口肯定找不到实现，因此DLL中所有类都会查一遍
	// 查询的底层原理是判断每个导出类继承的接口，调用其HasInterface函数
	// 而每个接口的HasInterface函数中又会调用其GetInterfaceID()函数
	// GetInterfaceID()函数中有每个类的ID，是函数中的局部静态变量，在第一次调用时被初始化
	// 因此，这里的代码作用就是强制调用每个导出实现类继承的接口的GetInterfaceID()函数，将其局部静态变量初始化一下，节约后续使用耗时
	const char* impl_class_ids_temp;
	GetDefaultImplClassID(IObject::GetInterfaceID(), impl_class_ids_temp);

	s_pln_hmod = pln_hmod;
	
	// 加载插件管理器
	if (!s_pln_manager_hmod)
	{
		// GetModuleHandleA: 获取已加载到当前进程地址空间中的模块（DLL 或 EXE）的句柄
		// 若传入 NULL，则返回当前进程可执行文件（.exe）的句柄 
		// 目标模块必须已被加载到调用进程的地址空间中，否则返回 NULL
		// 通过GetModuleHandleA获取DLL句柄，不会增加引用计数
		pln_manager_hmod = pln_manager_hmod ? pln_manager_hmod : GetModuleHandleA(Z3Y_PLUGIN_MANAGER_NAME);
		if (!pln_manager_hmod)
		{
			pln_manager_hmod = z3yLoadLibrary(Z3Y_PLUGIN_MANAGER_NAME);
		}
		if (pln_manager_hmod)
		{
			s_pln_manager_hmod = pln_manager_hmod;
			s_load_pln_manager = true;
		}
		
	}

	bool need_init_plugin{ true };

	// 向插件管理器注册插件
	if (s_pln_manager_hmod && s_pln_manager_hmod != s_pln_hmod)
	{
		using FuncPtrCreateObject = bool (*)(const char* impl_class_id, const InterfaceID& interface_id, IObject** ppv);
		using FuncPtrRegisterPlugin = bool (*)(FuncPtrCreateObject p_func_create_object, HMODULE pln_hmod, const char** impl_class_ids);

		FuncPtrRegisterPlugin p_func_register_plugin = reinterpret_cast<FuncPtrRegisterPlugin>(GetProcAddress(s_pln_manager_hmod, "z3yRegisterPlugin"));

		if (p_func_register_plugin)
		{
			const char* export_impl_class_ids[Z3Y_EXPORT_CLASS_MAXCOUNT + 1] = { nullptr };
			GetExportClassIDs(export_impl_class_ids, Z3Y_EXPORT_CLASS_MAXCOUNT);
			if (!(p_func_register_plugin(z3yInternalCreateObject, s_pln_hmod, export_impl_class_ids)))
			{
				need_init_plugin = false;
				if (s_load_pln_manager && s_pln_manager_hmod)
				{
					z3yFreeLibrary(s_pln_manager_hmod);
					s_pln_manager_hmod = nullptr;
					s_load_pln_manager = false;
				}
			}
		}
	}

	// need_init_plugin为true，说明没有插件管理器或者向插件管理器注册成功，调用z3yInitializePlugin()附加初始化
	// need_init_plugin为false，说明向插件管理器注册失败，不再调用z3yInitializePlugin()初始化插件
	return need_init_plugin ? z3yInitializePlugin() : true;
}

/**
* @breif 释放插件
* @retrun 每次调用引用计数减一，若减一后大于0，则返回false且不释放。若减一后等于0，则返回true，并且释放
* @note
*	1、引用计数减一
*	2、调用插件管理器的反注册函数
*	3、若没有插件管理器或者插件管理器反注册成功，调用插件本身的z3yUninitializePlugin()函数
*	4、释放单例系统
*/
Z3Y_EXPORT_API bool z3yFreePlugin()
{
	if (--s_pln_ref_count > 0)
	{
		return false;
	}

	bool need_free_plugin{ true };

	if (s_pln_manager_hmod && s_pln_manager_hmod != s_pln_hmod)
	{
		using FuncPtrCreateObject = bool (*)(const char* impl_class_id, const InterfaceID& interface_id, IObject** ppv);
		using FuncPtrUnregisterPlugin = bool (*)(FuncPtrCreateObject p_func_create_object);
		FuncPtrUnregisterPlugin p_func_unregister_plugin = reinterpret_cast<FuncPtrUnregisterPlugin>(GetProcAddress(s_pln_manager_hmod, "z3yUnregisterPlugin"));
		if (p_func_unregister_plugin && !(p_func_unregister_plugin(z3yInternalCreateObject)))
		{
			need_free_plugin = false;
		}
	}

	if (need_free_plugin)
	{
		z3yUninitializePlugin();
	}

	if (s_load_pln_manager && s_pln_manager_hmod)
	{
		z3yFreeLibrary(s_pln_manager_hmod);
		s_pln_manager_hmod = nullptr;
		s_load_pln_manager = false;
	}

	SingletonManager::Free();

	s_pln_hmod = nullptr;

	return true;
}

// todo 这个宏的作用，什么地方会使用这个宏做判断
#ifndef Z3Y_CREATE_OBJECT_IMPL
#define Z3Y_CREATE_OBJECT_IMPL
/**
* @brief 根据接口和实现类的ID，创建接口的实现类对象
* @param[in] impl_class_id 实现类的唯一标识
* @param[in] interface_id 接口的唯一标识
* @param[out] ppv 存放实现类对象的指针
* @return 创建成功返回true
* @todo 函数定义在宏中，什么地方会根据宏做判断呢？
*/
Z3Y_LOCAL_API bool CreateObject(const char* impl_class_id, const InterfaceID& interface_id, IObject** ppv)
{
	if (!z3yInternalCreateObject(impl_class_id, interface_id, ppv) && s_pln_manager_hmod)
	{
		using FuncPtrCreateObject = bool (*)(const char* impl_class_id, const InterfaceID& interface_id, IObject** ppv);
		FuncPtrCreateObject p_func_create_object = reinterpret_cast<FuncPtrCreateObject>(GetProcAddress(s_pln_manager_hmod, "z3yCreateObject"));
		return p_func_create_object && p_func_create_object(impl_class_id, interface_id, ppv);
	}
	return *ppv != nullptr;
}
#endif

// todo 这个宏是什么时候生效？想清楚使用场景
#ifndef Z3Y_EXCLUDE_CREATE_OBJECT
Z3Y_EXPORT_API bool z3yCreateObject(const char* impl_class_id, const InterfaceID& interface_id, IObject** ppv)
{
	return z3yInternalCreateObject(impl_class_id, interface_id, ppv);
}
#endif

Z3Y_END_NAMESPACE

#endif