#pragma once

#include "pch.h"
#include <map>
#include <queue>
#include <list>
#include <any>

#include "TableNode.h"


template <typename T_TABLE>
class MixedKeyTable : 
	public TableNode
{

public:
	struct KeyComparePred
	{
		bool operator()(std::any const& lhs_, std::any const& rhs_)
		{
			if (lhs_.type() == rhs_.type())
			{
				if (lhs_.type() == typeid(int))
				{
					int lhs = std::any_cast<int>(lhs_);
					int rhs = std::any_cast<int>(rhs_);
					return lhs < rhs;
				}
				else if (lhs_.type() == typeid(long long))
				{
					long long lhs = std::any_cast<long long>(lhs_);
					long long rhs = std::any_cast<long long>(rhs_);
					return lhs < rhs;
				}
				else if (lhs_.type() == typeid(double))
				{
					double lhs = std::any_cast<double>(lhs_);
					double rhs = std::any_cast<double>(rhs_);
					return lhs < rhs;
				}
				else if (lhs_.type() == typeid(char const*))
				{
					char const* lhs = std::any_cast<char const*>(lhs_);
					char const* rhs = std::any_cast<char const*>(rhs_);

					return strcmp(lhs, rhs) > 0;
				}
				else if (lhs_.type() == typeid(wchar_t const*))
				{
					wchar_t const* lhs = std::any_cast<wchar_t const*>(lhs_);
					wchar_t const* rhs = std::any_cast<wchar_t const*>(rhs_);

					return wcscmp(lhs, rhs) > 0;
				}

			}

			return false;
		}
	};


public:
	using TreeNodeMapType = std::map<std::any, TableNode*, KeyComparePred>;
	using TreeNodePairType = std::pair<std::any, TableNode*>;


public:
	explicit MixedKeyTable(std::any const& key_) : 
		m_Key(key_)
	{
	}

	explicit MixedKeyTable(KeyListType&& keyList_) :
		TableNode(keyList_)
	{
	}


	MixedKeyTable()
	{
	}


	~MixedKeyTable()
	{
	}


public:
	virtual bool MakeTableRow(TableCompoListType const& componentList_) override
	{
		T_TABLE* table = new T_TABLE();
		KeyValueListType keyValueList;

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
					keyValueList.push_back(keyValue);
				}
			}

			bool result = Insert(table, std::move(keyValueList));
			return result;
		}

		return false;
	}


	T_TABLE* Get()
	{
		return m_Table;
	}


	template <typename T_KEY_0, typename... T_KEYS>
	T_TABLE* Get(T_KEY_0 key_, T_KEYS&&... keyList_)
	{
		std::any key = key_;

		// 타입별로 정렬을 하고 있기 때문에 다른 로직을 쓰면 검색 속도가 조금 더 빨라질 수 있을 것 같습니다.
		auto itor = std::find_if(m_TreeNodeMap.begin(), m_TreeNodeMap.end(), [&](const TreeNodePairType& pair_) {
			
			if (key.type() == typeid(char const*) && pair_.first.type() == typeid(std::string))
			{
				char const* lhs = std::any_cast<char const*>(key);
				std::string rhs = std::any_cast<std::string>(pair_.first);
				return strcmp(lhs, rhs.c_str()) == 0;
			}
			else if (key.type() == typeid(wchar_t const*) && pair_.first.type() == typeid(std::wstring))
			{
				wchar_t const* lhs = std::any_cast<wchar_t const*>(key);
				std::wstring   rhs = std::any_cast<std::wstring>(pair_.first);
				return wcscmp(lhs, rhs.c_str()) == 0;
			}
			else if (key.type() == pair_.first.type())
			{
				if (key.type() == typeid(int))
				{
					int lhs = std::any_cast<int>(key);
					int rhs = std::any_cast<int>(pair_.first);
					return lhs == rhs;
				}
				else if (key.type() == typeid(long long))
				{
					long long lhs = std::any_cast<long long>(key);
					long long rhs = std::any_cast<long long>(pair_.first);
					return lhs == rhs;
				}
				else if (key.type() == typeid(double))
				{
					double lhs = std::any_cast<double>(key);
					double rhs = std::any_cast<double>(pair_.first);
					return lhs == rhs;
				}
			}

			return false;
		});


		if (itor != m_TreeNodeMap.end())
		{
			auto& pair = *(itor);
			auto node = static_cast<MixedKeyTable<T_TABLE>*>(pair.second);
			return node->Get(std::forward<T_KEYS>(keyList_)...);
		}

		return NULL;
	}


	bool Insert(T_TABLE const* table_)
	{
		m_Table = const_cast<T_TABLE*>(table_);
		return true;
	}


	bool Insert(T_TABLE const* table_, KeyValueListType&& keyValueList_)
	{
		if (keyValueList_.size() > 0)
		{
			std::any const& key = keyValueList_.front();
			TableNode* table = new MixedKeyTable<T_TABLE>(key);
			auto ret = m_TreeNodeMap.emplace(key, table);

			keyValueList_.pop_front();
			if (ret.second)
			{
				auto& pair = *(ret.first);
				auto node = static_cast<MixedKeyTable<T_TABLE>*>(pair.second);
				return node->Insert(table_, std::move(keyValueList_));
			}
			else
			{
				if (keyValueList_.size() > 0 || IsEnableDuplicatedKey)
				{
					auto& pair = *(ret.first);
					auto node = static_cast<MixedKeyTable<T_TABLE>*>(pair.second);
					return node->Insert(table_, std::move(keyValueList_));
				}
				else
				{
					return false;
				}
			}
		}

		m_Table = const_cast<T_TABLE*>(table_);
		return true;
	}


	static void SetEnableDuplicatedKey(bool enable_)
	{
		IsEnableDuplicatedKey = enable_;
	}


private:
	T_TABLE*     m_Table;
	std::any     m_Key;

	TreeNodeMapType m_TreeNodeMap;
};
