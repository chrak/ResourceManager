#pragma once

#include "pch.h"
#include <string>


class TestTable :
	public FieldMap<TestTable>
{
public:
	TestTable(int a_, int b_) : 
		a(a_), b(b_)
	{
		// string으로 값을 세팅할 수 있도록 멤버변수를 바인딩해줍니다.
		Bind("a", &TestTable::a);
		Bind("b", &TestTable::b);
		Bind("c", &TestTable::c);
	}


	TestTable()
	{
		// string으로 값을 세팅할 수 있도록 멤버변수를 바인딩해줍니다.
		Bind("a", &TestTable::a);
		Bind("b", &TestTable::b);
		Bind("c", &TestTable::c);
	}


	~TestTable()
	{
	}


	int a;
	int b;
	std::string c;
};
