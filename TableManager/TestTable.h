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
		// string���� ���� ������ �� �ֵ��� ��������� ���ε����ݴϴ�.
		Bind("a", &TestTable::a);
		Bind("b", &TestTable::b);
		Bind("c", &TestTable::c);
	}


	TestTable()
	{
		// string���� ���� ������ �� �ֵ��� ��������� ���ε����ݴϴ�.
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
