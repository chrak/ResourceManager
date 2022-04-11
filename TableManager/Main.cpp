#include "pch.h"
#include <iostream>

#include "MixedKeyTable.hpp"
#include "SingleKeyTable.hpp"
#include "TestTable.h"


int main()
{
	// 테이블 값 입력을 필드이름, 필드 값 형태로 세팅 가능합니다. 이로써 기획 테이블 세팅 방식을 하나로 통일할 수 있습니다.
	TestTable temp;	
	temp.SetValue("a", "11");
	temp.SetValue("b", "1234.5678");
	temp.SetValue("c", "hello");

	TestTable temp2;
	temp2.SetValue("a", "21");
	temp2.SetValue("b", "2234.5678");
	temp2.SetValue("c", "hello!!!");

	
	// 속도는 더 빠르나 키 타입을 섞을 수 없습니다.
	auto temp3 = new SingleKeyTable<TestTable, int>();
	temp3->Insert(new TestTable(4, 22), 3, 3, 5);
	temp3->Insert(new TestTable(4, 22), 10, 2);

	auto re0 = temp3->Get(3, 3, 5);
	auto re1 = temp3->Get(3, 3, 6);
	auto re2 = temp3->Get(10, 2);
	
	
	// std::any를 키로 쓰기 때문에 키에 의한 정렬을 할 때 타입 비교가 필요하고 느리지만 편의성이 좋습니다.
	auto temp4 = new MixedKeyTable<TestTable>();
	temp4->Insert(new TestTable(10, 2), TableNode::KeyValueListType({ 1, std::string("2"), 3 }));
	temp4->Insert(new TestTable(1, 4), TableNode::KeyValueListType({ 1, std::string("2"), 4 }));
	temp4->Insert(new TestTable(2, 4), TableNode::KeyValueListType({ 3, std::string("1"), 4 }));
	temp4->Insert(new TestTable(3, 4), TableNode::KeyValueListType({ 1, std::string("4"), 2 }));

	auto re3 = temp4->Get(1, "2", 3);
	auto re4 = temp4->Get(1, "4", 2);
	auto re5 = temp4->Get(3, "1", 4);
	
	// 각종 리소스를 아래와 같이 컴포넌트 리스트에 대입한 후
	TableNode::TableCompoListType componentList1;
	componentList1.push_back(std::make_pair("a", "100"));
	componentList1.push_back(std::make_pair("b", "20"));
	componentList1.push_back(std::make_pair("c", "test!!"));

	TableNode::TableCompoListType componentList2;
	componentList2.push_back(std::make_pair("a", "50"));
	componentList2.push_back(std::make_pair("b", "90"));
	componentList2.push_back(std::make_pair("c", "first"));

	TableNode::TableCompoListType componentList3;
	componentList3.push_back(std::make_pair("a", "50"));
	componentList3.push_back(std::make_pair("b", "150"));
	componentList3.push_back(std::make_pair("c", "first"));
	
	TableNode::TableCompoListType componentList4;
	componentList4.push_back(std::make_pair("a", "50"));
	componentList4.push_back(std::make_pair("b", "90"));
	componentList4.push_back(std::make_pair("c", "second"));


	// 키가 되는 필드를 무엇으로 할지 세팅합니다. 키는 복수개의 형태도 가능합니다.
	MixedKeyTable<TestTable> table1(TableNode::KeyListType({ "a", "c" }));
	SingleKeyTable<TestTable, int>::SetEnableDuplicatedKey(false);
	table1.MakeTableRow(componentList1);
	table1.MakeTableRow(componentList2); // 50,90,first
	table1.MakeTableRow(componentList3); // 50,150,fisrt  키가 겹치는데 중복 허용이 안되므로 b가 90을 기대
	

	auto ret6 = table1.Get(100, "test!!");    // ok
	auto ret7 = table1.Get(100, 20, "test!!");
	auto ret8 = table1.Get("test!!");
	auto ret9 = table1.Get(50, "first");   // ok
	bool ret10 = ret9->b == 90; // ok

	// 같은 타입의 키 테이블
	SingleKeyTable<TestTable, int> table2(TableNode::KeyListType({ "a", "b" }));
	SingleKeyTable<TestTable, int>::SetEnableDuplicatedKey(true);
	table2.MakeTableRow(componentList1);
	table2.MakeTableRow(componentList2); // 50,90,first
	table2.MakeTableRow(componentList4); // 50,90,second  키가 겹치는데 중복 허용이 되므로 c가 second임을 기대


	auto ret11 = table2.Get(100, 20);  // ok
	auto ret12 = table2.Get(50, 90);   // ok
	bool ret13 = ret12->c.compare("second") == 0; // ok
	
    std::cout << "Hello World!\n";
}



