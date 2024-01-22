#include <iostream>
#include <thread>
#include <mutex>
using namespace std;
using namespace this_thread;
using namespace chrono;

int main()
{
	int rate = 0;

	thread t([&]() 
		{
			while (true)
			{
				rate++;
				sleep_for(milliseconds(50));

				if (rate > 100)
					break;
			}
		});
	////////////////////////////////////////////////////////
	while (true)
	{
		system("cls");

		for (int i = 0; i < 10; i++)
		{
			if (i < rate / 10)
				cout << "■";
			else
				cout << "□";
		}
		cout << endl;

		cout << "Loading : " << rate << "%" << endl;
		
		if (rate >= 100)
		{
			break;
		}

		sleep_for(milliseconds(50));
	}
	

	t.join();

	system("cls");
	cout << "로딩완료" << endl;

	return 1;
}