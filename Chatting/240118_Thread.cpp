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
		cout << "하세요" << endl;
		sleep_for(milliseconds(500));
	}
	
}
void ByeFunc()
{
	for (int i = 0; i < 10; i++)
	{
		cout << "잘가" << endl;
	}
}

int main()
{
	thread tC([&]()
		{
			for (int i = 0; i < 10; i++)
			{
				cout << "안녕하세요" << endl;
				sleep_for(milliseconds(1000));
			}
		});

	int cnt = 100;
	cout << "입력 : ";
	cin >> cnt;

	cout << "입력한 값 : " << cnt << endl;

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
				cout << "람다 잘가" << endl;
				sleep_for(milliseconds(500));
			}
		});

	for (int i = 0; i < 10; i++)
	{
		cout << "안녕" << endl;
		sleep_for(milliseconds(500));
	}

	t.join();
	t2.join();

	return 0;
}

/*
	스레드

*/