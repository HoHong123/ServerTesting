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

		1.	�˰���
		2.	�����̳�( �ڷᱸ�� )
		3.	�ݺ���( �ڷᱸ�� ������ ������ ������ �ϴ� Ŭ���� )
		4.	�Լ���( �Լ�ó�� ���ܼ� �Լ��� ô�ϴ� Ŭ���� )

	�ڷᱸ��

		����		�ڷᱸ��
			
			����
			ť
			��
			���� ����Ʈ( ���� �迭 )

		����		�ڷᱸ��

			���� ����Ʈ( ������,  )
			Ʈ��( ����Ž��Ʈ��, AVL )
			�׷���...


		���� ����Ʈ vs ���� ����Ʈ vs Ž��Ʈ��

		���� ����Ʈ( vector ) : ����������, ���Ի��� �༼
		���� ����Ʈ( list ) : ���Ի���, ����������
		Ž�� Ʈ��( map, set ) : �˻�, ���Ի��� �ؼ�
*/