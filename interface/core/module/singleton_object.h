/**
* @file	singleton_object.h
* @brief 定义了插件类的单例对象构建器类模板
*
* @version	1.0
* @author	孙鹏宇
* @date		2025.5.19
*/

#pragma once
#ifndef Z3Y_CORE_SINGLETON_OBJECT_H
#define Z3Y_CORE_SINGLETON_OBJECT_H

#include "../iunknown.h"
#include "singleton_manager.h"
#include <mutex>

Z3Y_BEGIN_NAMESPACE

template<class ImplClass>
class SingletonObject
	: public ImplClass
	, public SingletonManager
{
public:
	/**
	* @brief 创建实现类对象
	* @param[in] interface_id 接口标识符
	* @return 返回创建出来的对象指针
	* @note
	*	1、SingletonObject是类模板，真正实例化出来的是SingletonObject<ImplClass>类的对象，而不是ImplClass
	*	2、SingletonObject<ImplClass>类的对象实例化后，会自身维护引用计数
	*	3、返回对象指针，是由SingletonObject<ImplClass>类的对象，通过QueryInterface向上转换得到的，实际指向的对象还是SingletonObject<ImplClass>类的对象
	*/
	static IUnknown* Create(InterfaceID interface_id)
	{
		if (!Instance())
		{
			static std::once_flag init_flag;
			std::call_once(init_flag, []() {
				SingletonObject<ImplClass>* p_singleton_object = new SingletonObject<ImplClass>();
				Instance() = p_singleton_object;
				p_singleton_object->AddSingletonObject();
				});
		}

		IUnknown* p_object{ nullptr };
		Instance()->QueryObject(interface_id, p_object);

		return p_object;
	}

	static bool HasInterface(InterfaceID interface_id)
	{
		return ImplClass::_QueryObject(nullptr, interface_id, nullptr);
	}
protected:
	SingletonObject() : ref_count_(1) {}
	~SingletonManager()
	{
		if (Instance() == this)
		{
			Instance() = nullptr;
		}
	}

	virtual bool QueryInterface(const InterfaceID& interface_id, IUnknown** ppv) const override
	{
		return ImplClass::_QueryObject(this, interface_id, ppv);
	}

	virtual int32_t AddRef() const override
	{
		return ++ref_count_;
	}

	virtual int32_t Release() const override
	{
		return --ref_count_;
	}

	virtual const char* GetImplClassID() const override
	{
		return ImplClass::_GetImplClassID();
	}

	virtual const char* GetImplClassName() const override
	{
		return ImplClass::_GetImplClassName();
	}

public:
	SingletonObject(const SingletonObject&) = delete;
	SingletonObject& operator=(const SingletonObject&) = delete;
private:
	/**
	* @brief 返回单例对象指针
	*/
	static SingletonObject<ImplClass>*& Instance()
	{
		static SingletonObject<ImplClass>* p_singleton_object{ nullptr };
		return p_singleton_object;
	}
private:
	mutable std::atomic<int32_t> ref_count_{ 0 };
};

Z3Y_END_NAMESPACE

#endif // !Z3Y_CORE_SINGLETON_OBJECT_H
