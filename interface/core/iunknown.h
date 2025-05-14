/**
* @file	iunknown.h
* @brief 定义了一些基础宏和函数，以及插件框架的最根本基类IUnknown
* 
* @author	孙鹏宇
* @date		2025.5.13
* @note
*	本插件框架的实现大量学习借鉴了开源项目x3py
*	x3py网址为: https://github.com/rhcad/x3py
*/

#pragma once
#ifndef Z3Y_CORE_IUNKONWN_H
#define Z3Y_CORE_IUNKONWN_H

#include <cstdint>

#define Z3Y_BEGIN_NAMESPACE  namespace z3y {
#define Z3Y_END_NAMESPACE    }

Z3Y_BEGIN_NAMESPACE

/**
* @brief 字符串哈希算法，命名以下划线起始，插件库内部使用
* 
* @param[in] str 需要进行哈希计算的字符串
* @return 返回哈希计算结果
* 
* @note
*	目前采用的是自然溢出法，这个方法的优点是速度快，缺点是哈希冲突率较高，但在小批量数据的情况下一般不会冲突
*/
inline uint64_t _HashKey(const char* str)
{
	uint64_t base{ 131 };
	uint64_t hash{ 0 };

	for (; *str != '\0'; str++)
	{
		// static_cast<uint64_t>(*str) 可以将字符转为对应的ASCII码值
		hash = hash * base + static_cast<uint64_t>(*str); // 逐字符迭代计算
	}
	return hash;
}

/**
* @brief 定义接口标识符
* @note
*	1、类的静态成员函数无需实例化，只用类名就可以调用。类的普通成员函数有隐藏的this指针指向实例化的对象。
*	2、通过宏，定义虚析构函数，便于多态的资源释放
*	3、通过宏，实现接口GetInterfaceName，返回接口类名的字符串
*	4、通过宏，实现接口GetInterfaceID，返回接口的唯一标识符，原理是接口类名字符串的哈希算法得到一个uint64_t整数
*		使用哈希算法中的自然溢出法，算法速度在第一梯队，冲突概率稍微偏高，但当数据量小于几万时也基本不冲突
*	5、static uint64_t interface_id = z3y::_HashKey(#interface_name); 
*		这里使用static修饰interface_id，保证只在第一次调用GetInterfaceID接口时初始化一次，后续再调用，不再重复计算，而是直接返回已计算的值。
*/
#define Z3Y_DEFINE_INTERFACE_ID(interface_name)     \
public: \
    virtual ~interface_name() {}         \
    static const char* GetInterfaceName() { return #interface_name; }   \
    static uint64_t GetInterfaceID() { static uint64_t interface_id = z3y::_HashKey(#interface_name); return interface_id; }


/**
* @brief 插件框架中，所有插件的最根本基类
*/
class IUnknown
{
	Z3Y_DEFINE_INTERFACE_ID(IUnknown)

	/**
	* @brief 接口查询和转换函数
	* 
	* @param[in] interface_id 接口标识符，每个接口的标识符都应是全局唯一的。
	* @param[out] ppv 指针的地址，若当前对象支持iid所对应的接口，则将当前对象的指针放入ppv指向的地址中。
	* @return 若当前对象支持iid所对应的接口，则返回true；否则返回false
	* @note
	*	转换成功后，当前对象应自动增加引用计数
	*/
	virtual bool QueryInterface(const uint64_t& interface_id, IUnknown** ppv) const = 0;

	/**
	* @brief 引用计数加1
	* @return 返回增加之后的引用计数数值
	*/
	virtual int32_t AddRef() const = 0;

	/**
	* @brief 引用计数减一
	* @return 返回减少之后的引用计数数值
	* @note
	*	若减一后引用计数为零，将当前对象析构
	*/
	virtual int32_t Release() const = 0;

	/**
	* @brief 获取实现类的ID
	* @note
	*	在插件框架中，接口类和实现类都有一个全局唯一的标识符。
	*	本函数作用就是返回实现类的标识符
	*/
	virtual const char* GetImplClassID() const = 0;

	/**
	* @brief 获取实现类的名字
	*/
	virtual const char* GetImplClassName() const = 0;
};

Z3Y_END_NAMESPACE

#endif