/**
* @file	object_ptr.h
* @brief ����������ָ����ģ�壬�����ò��ʱ��ͳһʹ�ø�ģ�����������ָ��
*
* @version	1.0
* @author	������
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
* @brief ����ָ����ģ��
* @note
*	ͨ��ʵ�����ID�������󣬲���ָ��תΪ�ӿ����ͣ���������������.
*	�����ò��ʱ��ͳһͨ���������ʵ����
*/
template <class InterfaceClass> class ObjectPtr
{
public:
	ObjectPtr() : p_impl_class_(nullptr) {}

	/**
	* @brief ���캯��������Ҫ������ʵ����ID�����Զ�����
	*/
	ObjectPtr(const char* impl_class_id)
		: p_impl_class_(nullptr)
	{
		//���ݽӿ����ʵ�����ID����ʵ�����������ǽӿ���ָ�룬ָ��ʵ����Ķ��� 
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
	* @brief �ͷŹ������ָ��
	* @note
	*	�������У����е�ʵ���඼�̳���IUnknown�����Զ���Release()�������������ü���
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
		//���ݽӿ����ʵ�����ID����ʵ�����������ǽӿ���ָ�룬ָ��ʵ����Ķ��� 
		CreateObject(impl_class_id, InterfaceClass::GetInterfaceID(), Address());
		return *this;
	}

	ObjectPtr<InterfaceClass>& operator=(const ObjectPtr<InterfaceClass>& other)
	{
		// �Ѿ�������boolת���������жϵ�other����ʵ�жϵ���other��ָ���Ƿ�Ϊ��
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
		// ����ΪʲôҪ�ж�InterfaceClass::GetInterfaceID() == OtherInterfaceClsss::GetInterfaceID()
		// GetInterfaceID()��ԭ���ǽӿ������ַ�����ϣ�㷨���ӿ����Ʋ�ͬ������϶���ͬ
		// todo ����Ҫ���ԣ��Ƿ��������߼�Ҫ��Ҫɾ��
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

					// ������뱣֤p_otherָ���Ƿ�const�ģ�ֻ����Ϊ�ӿڴ��ݲű�Ϊconst
					// ��p_other�������const��������const�ᵼ��δ������Ϊ
					// todo ����һ����һ�£���������ʲôδ������Ϊ
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
			// todo �����Ƿ�Ҫ�׳��쳣��
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
	* @brief ���ع������ָ���Ƿ�Ϊ��
	*/
	bool Vaild() const
	{
		// ����ȡ�����ɽ�ָ��תΪbool��
		// �ȼ���static_cast<bool>(p_impl_class_ != nullptr)
		return !!p_impl_class_;
	}

	/**
	* @brief ��ȡ�������ָ��
	* @warning ��ȡ��ָ��󣬲��ܽ���delete����
	*/
	InterfaceClass* GetRawPoint() const
	{
		return p_impl_class_;
	}

private:

	/**
	* @brief ��ȡp_impl_class_ָ��ĵ�ַ
	* @warning
	*	�ú��������õ�ָ���ַ��ֱ���޸�ָ�뱾��
	*	��Ӧ��������
	*/
	IUnknown** Address()
	{
		Release();
		return reinterpret_cast<IUnknown**>(&p_impl_class_);
	}

private:
	InterfaceClass* p_impl_class_; /// �ӿ����͵�ָ�룬ָ�����ʵ����Ķ���
};

using AnyObjectPtr = ObjectPtr<IUnknown>;

Z3Y_END_NAMESPACE

#endif