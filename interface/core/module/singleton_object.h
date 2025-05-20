/**
* @file	singleton_object.h
* @brief �����˲����ĵ������󹹽�����ģ��
*
* @version	1.0
* @author	������
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
	* @brief ����ʵ�������
	* @param[in] interface_id �ӿڱ�ʶ��
	* @return ���ش��������Ķ���ָ��
	* @note
	*	1��SingletonObject����ģ�壬����ʵ������������SingletonObject<ImplClass>��Ķ��󣬶�����ImplClass
	*	2��SingletonObject<ImplClass>��Ķ���ʵ�����󣬻�����ά�����ü���
	*	3�����ض���ָ�룬����SingletonObject<ImplClass>��Ķ���ͨ��QueryInterface����ת���õ��ģ�ʵ��ָ��Ķ�����SingletonObject<ImplClass>��Ķ���
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
	* @brief ���ص�������ָ��
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
