/**
* @file	object_ptr.h
* @brief 定义了智能指针类模板，外界调用插件时，统一使用该模板管理插件对象指针
*
* @version	1.0
* @author	孙鹏宇
* @date		2025.5.14
*/

#pragma once
#ifndef Z3Y_CORE_OBJECT_PTR_H
#define Z3Y_CORE_OBJECT_PTR_H

#include "iunknown.h"
#include "portability/z3y_export.h"

Z3Y_BEGIN_NAMESPACE

Z3Y_LOCAL_API bool CreateObject(const char* impl_class_id, const uint64_t& interface_id, IUnknown** ppv);

/**
* @brief 智能指针类模板
* @note
*	通过实现类的ID创建对象，并将指针转为接口类型，并管理生命周期.
*	外界调用插件时，统一通过本类进行实例化
*/
template <class InterfaceClass> class ObjectPtr
{
public:
	ObjectPtr() : p_impl_class_(nullptr) {}

	/**
	* @brief 构造函数中输入要创建的实现类ID，会自动创建
	*/
	ObjectPtr(const char* impl_class_id)
		: p_impl_class_(nullptr)
	{
		//根据接口类和实现类的ID创建实例化，类型是接口类指针，指向实现类的对象 
		CreateObject(impl_class_id, InterfaceClass::GetInterfaceID(), Address());
	}

	ObjectPtr(const IUnknown* p_other)
		: p_impl_class_(nullptr)
	{
		operator=(p_other);
	}

	ObjectPtr(const ObjectPtr<InterfaceClass>& other)
		: p_impl_class_(nullptr)
	{
		operator=(other.GetRawPoint());
	}

	template <class OtherInterfaceClsss>
	ObjectPtr(const ObjectPtr<OtherInterfaceClsss&> other)
		: p_impl_class_(nullptr)
	{
		operator=(other.GetRawPoint());
	}

	~ObjectPtr()
	{
		Release();
	}

	/**
	* @brief 释放管理的裸指针
	* @note
	*	插件框架中，所有的实现类都继承于IUnknown，所以都有Release()，用来减少引用计数
	*/
	void Release()
	{
		if (p_impl_class_)
		{
			p_impl_class_->Release();
			p_impl_class_ = nullptr;
		}
	}

	ObjectPtr<InterfaceClass>& Create(const char* impl_class_id = "")
	{
		//根据接口类和实现类的ID创建实例化，类型是接口类指针，指向实现类的对象 
		CreateObject(impl_class_id, InterfaceClass::GetInterfaceID(), Address());
		return *this;
	}

	ObjectPtr<InterfaceClass>& operator=(const ObjectPtr<InterfaceClass>& other)
	{
		// 已经重载了bool转换，看似判断的other，其实判断的是other中指针是否为空
		if (other)
		{
			other.p_impl_class_->AddRef();
		}
		if (p_impl_class_)
		{
			p_impl_class_->Release();
		}
		p_impl_class_ = other.p_impl_class_;

		return *this;
	}

	template <class OtherInterfaceClsss>
	ObjectPtr<InterfaceClass>& opertaor=(const ObjectPtr<OtherInterfaceClsss>& other)
	{
		// 这里为什么要判断InterfaceClass::GetInterfaceID() == OtherInterfaceClsss::GetInterfaceID()
		// GetInterfaceID()的原理是接口名称字符串哈希算法，接口名称不同，结果肯定不同
		// todo 这里要测试，是否是无用逻辑要不要删除
		if (InterfaceClass::GetInterfaceID() == OtherInterfaceClsss::GetInterfaceID()
			|| InterfaceClass::GetInterfaceID() == IUnknown::GetInterfaceID())
		{
			if (other)
			{
				other.p_impl_class_->AddRef();
			}
			if (p_impl_class_)
			{
				p_impl_class_->Release();
			}
			p_impl_class_ = other.p_impl_class_;

			return *this;
		}

		return operator=(other.GetRawPoint());
	}

	ObjectPtr<InterfaceClass>& operator=(const IUnknown* p_other)
	{
		if (p_impl_class_ != p_other)
		{
			Release();

			if (p_other)
			{
				if (InterfaceClass::GetInterfaceID() == IUnknown::GetInterfaceID())
				{
					p_other->AddRef();

					// 这里必须保证p_other指针是非const的，只是因为接口传递才变为const
					// 若p_other本身就是const，这里解除const会导致未定义行为
					// todo 这个找机会测一下，看看会有什么未定义行为
					p_impl_class_ = static<InterfaceClass*>(const_cast<IUnknown*>(p_other));
				}
				else
				{
					p_other->QueryInterface(InterfaceClass::GetInterfaceID(), Address());
				}
			}
		}
		return *this;
	}

	InterfaceClass* operator->() const
	{
		if (!p_impl_class_)
		{
			// todo 这里是否要抛出异常？
		}
		return p_impl_class_;
	}

	operator bool() const
	{
		return !!p_impl_class_;
	}

	bool operator!() const
	{
		return !p_impl_class_;
	}

	bool operator==(const ObjectPtr<InterfaceClass>& other) const
	{
		return p_impl_class_ == other.p_impl_class_;
	}

	bool operator!=(const ObjectPtr<InterfaceClass>& other) const
	{
		return p_impl_class_ != other.p_impl_class_;
	}

	/**
	* @brief 返回管理的裸指针是否不为空
	*/
	bool Vaild() const
	{
		// 两次取反，可将指针转为bool型
		// 等价于static_cast<bool>(p_impl_class_ != nullptr)
		return !!p_impl_class_;
	}

	/**
	* @brief 获取管理的裸指针
	* @warning 获取裸指针后，不能进行delete操作
	*/
	InterfaceClass* GetRawPoint() const
	{
		return p_impl_class_;
	}

private:

	/**
	* @brief 获取p_impl_class_指针的地址
	* @warning
	*	该函数用于拿到指针地址后，直接修改指针本身
	*	不应被外界调用
	*/
	IUnknown** Address()
	{
		Release();
		return reinterpret_cast<IUnknown**>(&p_impl_class_);
	}

private:
	InterfaceClass* p_impl_class_; /// 接口类型的指针，指向的是实现类的对象。
};

using AnyObjectPtr = ObjectPtr<IUnknown>;

Z3Y_END_NAMESPACE

#endif