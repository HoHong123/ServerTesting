#include <iostream>
using namespace std;

template< typename T, typename T2 >
T T2Add(T a, T2 b)
{
	return a + b;
}

template< typename T >
T TAdd(T a, T b)
{
	return a + b;
}

int Add(int a, int b)
{
	return a + b;
}

int main()
{
	cout << TAdd(10, 20) << endl;
	cout << TAdd(30.4f, 20.1f) << endl;
	cout << T2Add(30.1f, 10) << endl;
	return 0;
}

/*
*	Template
*	
*		C++	
* 
* 
	STL

		Standard Template Library

		1.	알고리즘
		2.	컨테이너( 자료구조 )
		3.	반복자( 자료구조 원소의 포인터 역할을 하는 클래스 )
		4.	함수자( 함수처럼 생겨서 함수인 척하는 클래스 )

	자료구조

		선형		자료구조
			
			스택
			큐
			덱
			선형 리스트( 동적 배열 )

		비선형		자료구조

			연결 리스트( 포인터,  )
			트리( 이진탐색트리, AVL )
			그래프...


		선형 리스트 vs 연결 리스트 vs 탐색트리

		선형 리스트( vector ) : 랜덤엑세스, 삽입삭제 약세
		연결 리스트( list ) : 삽입삭제, 랜덤엑세스
		탐색 트리( map, set ) : 검색, 삽입삭제 준수
*/