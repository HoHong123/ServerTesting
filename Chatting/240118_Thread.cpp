#include <iostream>
#include <thread>
#include <atomic>
using namespace std;
using namespace this_thread;
using namespace chrono;

void HelloFunc()
{
	for (int i = 0; i < 10; i++)
	{
		cout << "�ϼ���" << endl;
		sleep_for(milliseconds(500));
	}
	
}
void ByeFunc()
{
	for (int i = 0; i < 10; i++)
	{
		cout << "�߰�" << endl;
	}
}

int main()
{
	thread tC([&]()
		{
			for (int i = 0; i < 10; i++)
			{
				cout << "�ȳ��ϼ���" << endl;
				sleep_for(milliseconds(1000));
			}
		});

	int cnt = 100;
	cout << "�Է� : ";
	cin >> cnt;

	cout << "�Է��� �� : " << cnt << endl;

	tC.join();

	return 0;

	atomic<int> num = 0;

	thread tA([&]() 
		{
			for (int i = 0; i < 100000000; i++)
			{
				num--;
			}
		});
	thread tB([&]()
		{
			for (int i = 0; i < 100000000; i++)
			{
				num++;
			}
		});

	tA.join();
	tB.join();

	cout << num << endl;

	return 1;


	//////////////////////////////////////
	thread t(HelloFunc);
	thread t2([]() 
		{
			for (int i = 0; i < 10; i++)
			{
				cout << "���� �߰�" << endl;
				sleep_for(milliseconds(500));
			}
		});

	for (int i = 0; i < 10; i++)
	{
		cout << "�ȳ�" << endl;
		sleep_for(milliseconds(500));
	}

	t.join();
	t2.join();

	return 0;
}

/*
	������

*/