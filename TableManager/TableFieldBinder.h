#pragma once

#include <map>
#include <string>
#include <sstream>

template<class STRUC>
struct Field
{
	virtual void set(STRUC& struc, const std::string& value) const = 0;
};

template<class STRUC, class FIELDTYPE>
struct FieldImpl : public Field<STRUC>
{
	typedef FIELDTYPE(STRUC::*MemberPtr);

	FieldImpl(MemberPtr memberPtr) { memberPtr_ = memberPtr; }

	virtual void set(STRUC& struc, const std::string& value) const
	{
		std::istringstream iss(value);
		iss >> struc.*memberPtr_;
	}

private:
	MemberPtr memberPtr_;
};


template<class STRUC>
class FieldMap
{
private:
	typedef std::map<std::string, Field<STRUC>*> FieldNameMap;
	FieldNameMap  fieldMap_;

public:
	~FieldMap()
	{
		// delete fieldMap_ members.
	}

	void bind(const std::string& name, Field<STRUC>* field)
	{
		fieldMap_[name] = field;
	}

	template<typename FIELDTYPE>
	void bind(const std::string& name, FIELDTYPE(STRUC::* member))
	{
		fieldMap_[name] = new FieldImpl<STRUC, FIELDTYPE>(member);
	}

	void setValue(STRUC& struc, const std::string& name, const std::string& value)
	{
		FieldNameMap::const_iterator iter = fieldMap_.find(name);

		if (iter == fieldMap_.end())
			throw std::runtime_error(std::string("No field binding found for ") + name);

		(*iter).second->set(struc, value);
	}
};
