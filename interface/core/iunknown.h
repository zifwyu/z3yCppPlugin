/**
* @file	iunknown.h
* @brief ������һЩ������ͺ������Լ������ܵ����������IUnknown
* 
* @author	������
* @date		2025.5.13
* @note
*	�������ܵ�ʵ�ִ���ѧϰ����˿�Դ��Ŀx3py
*	x3py��ַΪ: https://github.com/rhcad/x3py
*/

#pragma once
#ifndef Z3Y_CORE_IUNKONWN_H
#define Z3Y_CORE_IUNKONWN_H

#include <cstdint>

#define Z3Y_BEGIN_NAMESPACE  namespace z3y {
#define Z3Y_END_NAMESPACE    }

Z3Y_BEGIN_NAMESPACE

/**
* @brief �ַ�����ϣ�㷨���������»�����ʼ��������ڲ�ʹ��
* 
* @param[in] str ��Ҫ���й�ϣ������ַ���
* @return ���ع�ϣ������
* 
* @note
*	Ŀǰ���õ�����Ȼ�����������������ŵ����ٶȿ죬ȱ���ǹ�ϣ��ͻ�ʽϸߣ�����С�������ݵ������һ�㲻���ͻ
*/
inline uint64_t _HashKey(const char* str)
{
	uint64_t base{ 131 };
	uint64_t hash{ 0 };

	for (; *str != '\0'; str++)
	{
		// static_cast<uint64_t>(*str) ���Խ��ַ�תΪ��Ӧ��ASCII��ֵ
		hash = hash * base + static_cast<uint64_t>(*str); // ���ַ���������
	}
	return hash;
}

/**
* @brief ����ӿڱ�ʶ��
* @note
*	1����ľ�̬��Ա��������ʵ������ֻ�������Ϳ��Ե��á������ͨ��Ա���������ص�thisָ��ָ��ʵ�����Ķ���
*	2��ͨ���꣬�������������������ڶ�̬����Դ�ͷ�
*	3��ͨ���꣬ʵ�ֽӿ�GetInterfaceName�����ؽӿ��������ַ���
*	4��ͨ���꣬ʵ�ֽӿ�GetInterfaceID�����ؽӿڵ�Ψһ��ʶ����ԭ���ǽӿ������ַ����Ĺ�ϣ�㷨�õ�һ��uint64_t����
*		ʹ�ù�ϣ�㷨�е���Ȼ��������㷨�ٶ��ڵ�һ�ݶӣ���ͻ������΢ƫ�ߣ�����������С�ڼ���ʱҲ��������ͻ
*	5��static uint64_t interface_id = z3y::_HashKey(#interface_name); 
*		����ʹ��static����interface_id����ֻ֤�ڵ�һ�ε���GetInterfaceID�ӿ�ʱ��ʼ��һ�Σ������ٵ��ã������ظ����㣬����ֱ�ӷ����Ѽ����ֵ��
*/
#define Z3Y_DEFINE_INTERFACE_ID(interface_name)     \
public: \
    virtual ~interface_name() {}         \
    static const char* GetInterfaceName() { return #interface_name; }   \
    static uint64_t GetInterfaceID() { static uint64_t interface_id = z3y::_HashKey(#interface_name); return interface_id; }


/**
* @brief �������У����в�������������
*/
class IUnknown
{
	Z3Y_DEFINE_INTERFACE_ID(IUnknown)

	/**
	* @brief �ӿڲ�ѯ��ת������
	* 
	* @param[in] interface_id �ӿڱ�ʶ����ÿ���ӿڵı�ʶ����Ӧ��ȫ��Ψһ�ġ�
	* @param[out] ppv ָ��ĵ�ַ������ǰ����֧��iid����Ӧ�Ľӿڣ��򽫵�ǰ�����ָ�����ppvָ��ĵ�ַ�С�
	* @return ����ǰ����֧��iid����Ӧ�Ľӿڣ��򷵻�true�����򷵻�false
	* @note
	*	ת���ɹ��󣬵�ǰ����Ӧ�Զ��������ü���
	*/
	virtual bool QueryInterface(const uint64_t& interface_id, IUnknown** ppv) const = 0;

	/**
	* @brief ���ü�����1
	* @return ��������֮������ü�����ֵ
	*/
	virtual int32_t AddRef() const = 0;

	/**
	* @brief ���ü�����һ
	* @return ���ؼ���֮������ü�����ֵ
	* @note
	*	����һ�����ü���Ϊ�㣬����ǰ��������
	*/
	virtual int32_t Release() const = 0;

	/**
	* @brief ��ȡʵ�����ID
	* @note
	*	�ڲ������У��ӿ����ʵ���඼��һ��ȫ��Ψһ�ı�ʶ����
	*	���������þ��Ƿ���ʵ����ı�ʶ��
	*/
	virtual const char* GetImplClassID() const = 0;

	/**
	* @brief ��ȡʵ���������
	*/
	virtual const char* GetImplClassName() const = 0;
};

Z3Y_END_NAMESPACE

#endif