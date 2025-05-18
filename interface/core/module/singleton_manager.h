#pragma once
#ifndef Z3Y_CORE_SINGLETON_MANAGER_H
#define Z3Y_CORE_SINGLETON_MANAGER_H

#include "../iunknown.h"
#include <atomic>

Z3Y_BEGIN_NAMESPACE

/**
* @brief ���������࣬һ��DLL�����еĵ�����ͨ���������
*/
class SingletonManager
{
protected:
	SingletonManager() :index_(-1) {}
	virtual ~SingletonManager() {}

	/**
	* @����һ����������
	* @note
	*	�����������ҵ����Ŀ�λ�ã�������ǰ�����ָ�����
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
	* @brief ��ʼ������������DLL�е�����������ʼ���洢ָ�������
	* @param[in] singleton_class_count DLL�е����ĵ������������
	*/
	static void Init(const int32_t& singleton_class_count)
	{
		// !SingletonArray()�����ˣ�ֻ�е�һ�ε��ò���Ч
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
	* @brief �ͷ����еĵ��������ڴ�
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
	* @brief ���ش�ŵ������������飬��DLL���������е��������ָ�붼�洢�������С�
	*/
	static SingletonManager**& SingletonArray()
	{
		static SingletonManager** p_singleton_array{ nullptr };
		return p_singleton_array;
	}

	/**
	* @brief ���ع���ĵ��������У������±���С�Ķ�����±�
	* @note
	*	���鳤����Initʱ��֪��Ȼ��Ӵ�С��������������һ��Ԫ�ء�
	*/
	static std::atomic<int32_t>& MinUsedIndex()
	{
		static std::atomic<int32_t> min_used_index{ 0 };
		return min_used_index;
	}

	/**
	* @brief ������������ʵ����������Init()��ʼ��ʱ��̬����
	*/
	static int32_t& MaxCount()
	{
		static int32_t max_count{ 0 };
		return max_count;
	}

private:
	int32_t index_; // ��ʾ��ǰʵ���ڵ��������е�λ���������Ӹߵ��ͷ��䣩����ʼֵΪ-1��ע���ͨ��ԭ�Ӳ���������Ч���� 
};

Z3Y_END_NAMESPACE

#endif // !Z3Y_CORE_SINGLETON_MANAGER_H
