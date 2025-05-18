#pragma once
#ifndef Z3Y_CORE_SINGLETON_MANAGER_H
#define Z3Y_CORE_SINGLETON_MANAGER_H

#include "../iunknown.h"
#include <atomic>

Z3Y_BEGIN_NAMESPACE

/**
* @brief 单例管理类，一个DLL中所有的单例都通过本类管理
*/
class SingletonManager
{
protected:
	SingletonManager() :index_(-1) {}
	virtual ~SingletonManager() {}

	/**
	* @增加一个单例对象
	* @note
	*	会在数组中找到最大的空位置，并将当前对象的指针存入
	*/
	void AddSingletonObject()
	{
		index_ = --MinUsedIndex();
		if (index_ >= 0)
		{
			SingletonArray()[index_] = this;
		}
	}
public:

	/**
	* @brief 初始化函数，根据DLL中单例类数量初始化存储指针的数组
	* @param[in] singleton_class_count DLL中导出的单例类的总数量
	*/
	static void Init(const int32_t& singleton_class_count)
	{
		// !SingletonArray()决定了，只有第一次调用才有效
		if (!SingletonArray() && singleton_class_count > 0)
		{
			if (singleton_class_count > MaxCount())
			{
				MaxCount() = singleton_class_count;
			}

			MinUsedIndex() = MaxCount();
			SingletonArray() = new SingletonManager* [MaxCount()];
		}
	}

	/**
	* @brief 释放所有的单例对象内存
	*/
	static void Free()
	{
		if (SingletonArray())
		{
			for (int32_t i = MinUsedIndex(); i < MaxCount(); ++i)
			{
				if (i >= 0 && SingletonArray()[i])
				{
					delete SingletonArray()[i];
					SingletonArray()[i] = nullptr;
				}

				MinUsedIndex() = 0;

				delete[] SingletonArray();
				SingletonArray() = nullptr;
			}
		}
	}
private:

	/**
	* @brief 返回存放单例类对象的数组，本DLL导出的所有单例类对象指针都存储在数组中。
	*/
	static SingletonManager**& SingletonArray()
	{
		static SingletonManager** p_singleton_array{ nullptr };
		return p_singleton_array;
	}

	/**
	* @brief 返回管理的单例对象中，数组下标最小的对象的下标
	* @note
	*	数组长度在Init时已知，然后从大到小填充对象，先填充最后一个元素。
	*/
	static std::atomic<int32_t>& MinUsedIndex()
	{
		static std::atomic<int32_t> min_used_index{ 0 };
		return min_used_index;
	}

	/**
	* @brief 返回最大可容纳实例数，可在Init()初始化时动态调整
	*/
	static int32_t& MaxCount()
	{
		static int32_t max_count{ 0 };
		return max_count;
	}

private:
	int32_t index_; // 表示当前实例在单例数组中的位置索引（从高到低分配）。初始值为-1，注册后通过原子操作分配有效索引 
};

Z3Y_END_NAMESPACE

#endif // !Z3Y_CORE_SINGLETON_MANAGER_H
