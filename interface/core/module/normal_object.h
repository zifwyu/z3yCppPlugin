/**
* @file	normal_object.h
* @brief 定义了插件类的普通对象构建器类模板
*
* @version	1.0
* @author	孙鹏宇
* @date		2025.5.16
*/

#pragma once
#ifndef Z3Y_CORE_NORMAL_OBJECT_H
#define Z3Y_CORE_NORMAL_OBJECT_H

#include "../IObject.h"
#include <atomic>

Z3Y_BEGIN_NAMESPACE

/**
* @brief 插件类的普通对象构建器类模板
* @details
*	插件框架中，实现接口的类并不是最下层的类，它还有子类，就是插件对象构建器类
*	接口的实现类，只需要专注实现接口功能即可，没有普通类和单例类之分。
*	具体是普通类还是单例，可以调用不同构建器创建
*	构建器类会继承接口的实现类，并实现IObject类的纯虚函数
*/
template<class ImplClass>
class NormalObject : public ImplClass
{
public:

	/**
	* @brief 创建实现类对象
	* @param[in] interface_id 接口标识符
	* @return 返回创建出来的对象指针
	* @note
	*	1、NormalObject是类模板，真正实例化出来的是NormalObject<ImplClass>类的对象，而不是ImplClass
	*	2、NormalObject<ImplClass>类的对象实例化后，会自身维护引用计数
	*	3、返回对象指针，是由NormalObject<ImplClass>类的对象，通过QueryInterface向上转换得到的，实际指向的对象还是NormalObject<ImplClass>类的对象
	*/
	static IObject* Create(InterfaceID interface_id)
	{
		IObject* ret{ nullptr };

		NormalObject<ImplClass>* p = new NormalObject<ImplClass>();

		p->QueryInterface(interface_id, &ret);
		p->AddRef();

		return ret;
	}

	static bool HasInterface(InterfaceID interface_id)
	{
		return ImplClass::_QueryObject(nullptr, interface_id, nullptr);
	}

protected:
	NormalObject(): ref_count_(1)
	{
	}

	virtual ~NormalObject() {}

	virtual bool QueryInterface(const InterfaceID& interface_id, IObject** ppv) const override
	{
		return ImplClass::_QueryObject(this, interface_id, ppv);
	}

	virtual int32_t AddRef() const override
	{
		return ++ref_count_;
	}

	virtual int32_t Release() const override
	{
		int32_t ref_count = --ref_count_;
		if (0 == ref_count)
		{
			delete this;
		}
		return ref_count;
	}

	virtual const char* GetImplClassID() const override
	{
		return ImplClass::_GetImplClassID();
	}

	virtual const char* GetImplClassName() const override
	{
		return ImplClass::_GetImplClassName();
	}

	// 这里应该使用 public权限+delete关键字来禁止拷贝构造和赋值运算符重载
	// 当用户尝试调用这两种函数时，编译器会报错“函数已被删除”
	// 如果使用 private+delete，调用时会报错“函数为private”，不如上面直观。
public:
	NormalObject(const NormalObject&) = delete;
	NormalObject& operator=(const NormalObject&) = delete;
private:
	mutable std::atomic<int32_t> ref_count_{ 0 };
};

Z3Y_END_NAMESPACE

#endif