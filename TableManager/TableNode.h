#pragma once

#include "pch.h"
#include <any>
#include <list>
#include <map>
#include <string>
#include <sstream>


template<typename STRUC>
class Field
{
public:
	virtual std::any Set(STRUC* struc, const std::string& value) const = 0;
};


template<typename STRUC, typename FIELDTYPE>
class FieldImpl : public Field<STRUC>
{
public:
	using MemberPtr = FIELDTYPE(STRUC::*);

public:
	FieldImpl(MemberPtr memberPtr_)
	{
		m_MemberPtr = memberPtr_;
	}

public:
	virtual std::any Set(STRUC* struc_, const std::string& value_) const
	{
		std::istringstream iss(value_);
		iss >> (*struc_).*m_MemberPtr;

		return (*struc_).*m_MemberPtr;
	}


private:
	MemberPtr m_MemberPtr;
};


template<typename STRUC>
class FieldMap
{
public:
	using FieldNameMap = std::map<std::string, Field<STRUC>*>;


public:
	FieldMap()
	{
	}

	~FieldMap()
	{
	}


public:
	void Bind(const std::string& name, Field<STRUC>* field)
	{
		m_FieldMap[name] = field;
	}

	template<typename FIELDTYPE>
	void Bind(const std::string& name, FIELDTYPE(STRUC::* member))
	{
		m_FieldMap[name] = new FieldImpl<STRUC, FIELDTYPE>(member);
	}

	std::any SetValue(const std::string& name, const std::string& value)
	{
		auto iter = m_FieldMap.find(name);

		if (iter == m_FieldMap.end())
			throw std::runtime_error(std::string("No field binding found for ") + name);

		return (*iter).second->Set(static_cast<STRUC*>(this), value);
	}


private:
	FieldNameMap  m_FieldMap;
};


class TableNode
{
public:
	using KeyValueListType = std::list<std::any>;
	using KeyListType = std::list<std::string>;
	using TableCompoListType = std::list<std::pair<std::string, std::string>>;

public:
	explicit TableNode(KeyListType keyList_)
		: m_KeyList(keyList_)
	{
	}

	TableNode()
	{
	}

	virtual ~TableNode()
	{
	}


public:
	virtual bool MakeTableRow(TableCompoListType const& componentList_) = 0;


public:
	inline static bool IsEnableDuplicatedKey = false;


protected:
	KeyListType m_KeyList;
	
};



