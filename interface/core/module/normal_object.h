/**
* @file	normal_object.h
* @brief �����˲�������ͨ���󹹽�����ģ��
*
* @version	1.0
* @author	������
* @date		2025.5.16
*/

#pragma once
#ifndef Z3Y_CORE_NORMAL_OBJECT_H
#define Z3Y_CORE_NORMAL_OBJECT_H

#include "../IObject.h"
#include <atomic>

Z3Y_BEGIN_NAMESPACE

/**
* @brief ��������ͨ���󹹽�����ģ��
* @details
*	�������У�ʵ�ֽӿڵ��ಢ�������²���࣬���������࣬���ǲ�����󹹽�����
*	�ӿڵ�ʵ���ֻ࣬��Ҫרעʵ�ֽӿڹ��ܼ��ɣ�û����ͨ��͵�����֮�֡�
*	��������ͨ�໹�ǵ��������Ե��ò�ͬ����������
*	���������̳нӿڵ�ʵ���࣬��ʵ��IObject��Ĵ��麯��
*/
template<class ImplClass>
class NormalObject : public ImplClass
{
public:

	/**
	* @brief ����ʵ�������
	* @param[in] interface_id �ӿڱ�ʶ��
	* @return ���ش��������Ķ���ָ��
	* @note
	*	1��NormalObject����ģ�壬����ʵ������������NormalObject<ImplClass>��Ķ��󣬶�����ImplClass
	*	2��NormalObject<ImplClass>��Ķ���ʵ�����󣬻�����ά�����ü���
	*	3�����ض���ָ�룬����NormalObject<ImplClass>��Ķ���ͨ��QueryInterface����ת���õ��ģ�ʵ��ָ��Ķ�����NormalObject<ImplClass>��Ķ���
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

	// ����Ӧ��ʹ�� publicȨ��+delete�ؼ�������ֹ��������͸�ֵ���������
	// ���û����Ե��������ֺ���ʱ���������ᱨ�������ѱ�ɾ����
	// ���ʹ�� private+delete������ʱ�ᱨ������Ϊprivate������������ֱ�ۡ�
public:
	NormalObject(const NormalObject&) = delete;
	NormalObject& operator=(const NormalObject&) = delete;
private:
	mutable std::atomic<int32_t> ref_count_{ 0 };
};

Z3Y_END_NAMESPACE

#endif