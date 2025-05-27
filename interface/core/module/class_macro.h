/**
* @file	class_macro.h
* @brief �����˲������й������Ԫ���ݵ�һЩ��
*
* @version	1.0
* @author	������
* @date		2025.5.22
*/

#pragma once
#ifndef Z3Y_CORE_CLASS_MECRO_H
#define Z3Y_CORE_CLASS_MECRO_H


/**
* @brief ʵ����ӿ������Ŀ�ʼ
*/
#define Z3Y_BEGIN_IMPL_CLASS_DECLARE(impl_class_name, impl_class_id) \
public: \
	static const char* _GetImplClassID() { return impl_class_id;} \
	static const char* _GetImplClassName() {return #impl_class_name;} \
	static bool _QueryObject(const impl_class_name* self, z3y::InterfaceID interface_id, z3y::IObject** ppv) \
	{

/**
* @brief ��ĳ��ʵ����ֻ��Ҫ��ѯ������Ҫ��ȡ���ID������ʱ��ʹ�ñ���
* @todo 
*	����ʲô�������ã�Ϊʲô���������Ӧ��ʱȷ��֪��ʵ���಻�ᱻʵ��������������̳е�ʱ��ʹ�á�
*	�б�Ҫ�����𣿶ඨ����������Ҳûʲô�����Ƿ�ֹʹ���ߴ���Ľ����ñ�ʵ������ʵ�����ʵ��������
*/
#define Z3Y_BEGIN_IMPL_CLASS_DECLARE0(impl_class_name, impl_class_id) \
	static bool _QueryObject(const impl_class_name* self, z3y::InterfaceID interface_id, z3y::IObject** ppv) \
	{

/**
* @brief ��ʵ�����ͷ�ļ��У�����ʵ�ֵĽӿ�
* @note
*	����ʹ��˫��ת����Ϊ�˷�ֹ�̳ж���ӿ�ʱ��ָ��Ĵ���ƫ��
*	todo ����Ҫ��֤һ�£�Ҫ������
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
* @brief ��ʵ����ĸ���Ҳ��ʵ�����ʱ��ʹ�ô˺�����
*/
#define Z3Y_USE_BASE_IMPL_CLASS_INTERFACE_ENTRY(base_impl_class) \
		if(base_impl_class::_QueryObject(self, interface_id, ppv)) \
		{ \
			return true; \
		}

#define Z3Y_END_IMPL_CLASS_DECLARE() \
		return false; \
	} \
protected: //	ʵ����ĺ���������public

#endif // !Z3Y_CORE_CLASS_MECRO_H