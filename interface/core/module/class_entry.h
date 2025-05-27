/**
* @file	class_entry.h
* @brief �����˲����ĵ��������࣬�����а���ʵ����һ��������������Ϣ
*
* @version	1.0
* @author	������
* @date		2025.5.15
*/

#pragma once
#ifndef Z3Y_CORE_CLASS_ENTRY_H
#define Z3Y_CORE_CLASS_ENTRY_H

#include "../IObject.h"

Z3Y_BEGIN_NAMESPACE

using FuncPtrObjectCreator = IObject* (*)(InterfaceID interface_id);
using FuncPtrHasInterfaceID = bool (*)(InterfaceID interface_id);

// todo Ϊʲôֻ�����˵���ģʽ��Ϊʲô��10��
enum { MIN_SINGLETON_TYPE = 10 };

/**
* @brief ���ʵ��������
* @details ����ĳ�Ա�����������˴���һ�������ʵ���������Ϣ
*	ͨ����Щ�����е���Ϣ�����Թ����һ�������Ķ���
*/
class ClassEntry
{
public:
	int class_type; //������ͣ���ͨor����
	const char* class_name;
	const char* impl_class_id;
	FuncPtrObjectCreator p_func_object_creator; //������������ĺ���ָ��
	FuncPtrHasInterfaceID p_func_has_interface_id; //�ж��Ƿ�ʵ����ĳ���ӿ�

	ClassEntry() : class_type(0), class_name(""), impl_class_id(""), p_func_object_creator(nullptr), p_func_has_interface_id(nullptr)
	{
	}

	ClassEntry(int _type, const char* _class_name,
		const char* _impl_class_id, FuncPtrObjectCreator _creator, FuncPtrHasInterfaceID _hasiid)
		: class_type(_type), class_name(_class_name), impl_class_id(_impl_class_id), p_func_object_creator(_creator), p_func_has_interface_id(_hasiid)
	{
	}

	// ClassEntry�������ɵ����飬�洢�˱�DLL���е����Ĳ����Ĺ�����Ϣ
	// ��̬������ָ�����ͣ�ָ��ClassEntry�������ɵ�������׵�ַ
	// �������ڴ˴���������DLL���������cpp���봦���ж���
	// ���������ᵼ����ֻ�ᱻͬһ��DLL�еĴ������
	static const ClassEntry* const  classes[];
};

Z3Y_END_NAMESPACE

#endif // !Z3Y_CORE_CLASS_ENTRY_H
