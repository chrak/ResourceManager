#pragma once

#include "pch.h"
#include <map>
#include <queue>
#include <initializer_list>
#include <any>

#include "TableNode.h"


template <typename T_TABLE, typename T_KEY>
class SingleKeyTable :
	public TableNode
{
public:
	using TreeNodeMapType = std::map<T_KEY, TableNode*>;


public:
	explicit SingleKeyTable(T_KEY const& key_) :
		m_Key(key_)
	{
	}


	explicit SingleKeyTable(KeyListType&& keyList_)
		: TableNode(keyList_)
	{
	}


	SingleKeyTable()
	{
	}

	~SingleKeyTable()
	{
	}


public:
	virtual bool MakeTableRow(TableCompoListType const& componentList_) override
	{
		T_TABLE* table = new T_TABLE();
		std::vector<T_KEY> keyValueVec;

		if (std::is_base_of<FieldMap<T_TABLE>, T_TABLE>::value)
		{
			for (auto& pair : componentList_)
			{
				std::string const& keyName = pair.first;
				std::string const& value = pair.second;

				auto keyValue = table->SetValue(keyName, value);

				auto itor = std::find(m_KeyList.begin(), m_KeyList.end(), keyName);
				if (itor != m_KeyList.end())
				{
					T_KEY realTypeValue = std::any_cast<T_KEY>(keyValue);
					keyValueVec.push_back(realTypeValue);
				}
			}

			bool result = false;

			switch (keyValueVec.size())
			{
			case 1:
				result = Insert(table, keyValueVec[0]);
				break;
			case 2:
				result = Insert(table, keyValueVec[0], keyValueVec[1]);
				break;
			case 3:
				result = Insert(table, keyValueVec[0], keyValueVec[1], keyValueVec[2]);
				break;
			case 4:
				result = Insert(table, keyValueVec[0], keyValueVec[1], keyValueVec[2], keyValueVec[3]);
				break;
			case 5:
				result = Insert(table, keyValueVec[0], keyValueVec[1], keyValueVec[2], keyValueVec[3], keyValueVec[4]);
				break;

			default:
				throw std::runtime_error(std::string("the number of singleType keys is invalid."));
			}

			return true;
		}

		return false;
	}


	T_TABLE* Get()
	{
		return m_Table;
	}


	template <typename... T_KEYS>
	T_TABLE* Get(T_KEY key_, T_KEYS&&... keyList_)
	{
		auto itor = m_TreeNodeMap.find(key_);
		if (itor != m_TreeNodeMap.end())
		{
			auto& pair = *(itor);
			auto node = static_cast<SingleKeyTable<T_TABLE, T_KEY>*>(pair.second);
			return node->Get(std::forward<T_KEYS>(keyList_)...);
		}

		return NULL;
	}


	bool Insert(T_TABLE const* table_)
	{
		m_Table = const_cast<T_TABLE*>(table_);

		return true;
	}


	template <typename T_TABLE, typename... T_KEYS>
	bool Insert(T_TABLE const* table_, T_KEY key_, T_KEYS&&... keyList_)
	{
		SingleKeyTable* node = new SingleKeyTable<T_TABLE, T_KEY>(key_);

		auto ret = m_TreeNodeMap.emplace(key_, node);
		if (ret.second)
		{
			auto& pair = *(ret.first);
			auto node = static_cast<SingleKeyTable<T_TABLE, T_KEY>*>(pair.second);

			node->Insert(table_, std::forward<T_KEYS>(keyList_)...);
		}
		else
		{
			if (sizeof...(keyList_) > 0 || IsEnableDuplicatedKey)
			{
				auto& pair = *(ret.first);
				auto node = static_cast<SingleKeyTable<T_TABLE, T_KEY>*>(pair.second);

				node->Insert(table_, std::forward<T_KEYS>(keyList_)...);
			}
			else
			{
				return false;
			}
		}

		return true;
	}


	static void SetEnableDuplicatedKey(bool enable_)
	{
		IsEnableDuplicatedKey = enable_;
	}


private:
		T_TABLE*      m_Table;
		T_KEY         m_Key;

		TreeNodeMapType m_TreeNodeMap;
};


