#include <iostream>
#include <functional>
using namespace std;

enum 
{
    CALC_ADD = 0,
    CALC_SUB
};

int Calc(int _numA, int _numB, int _Opt)
{
    int result = 0;

    switch (_Opt)
    {
    case 0:
        result =  _numA + _numB;
        break;
    case 1:
        result = _numA - _numB;
        break;
    default:
        break;
    }

    return result;
}

int CalcLam(int _numA, int _numB, function<int(int, int)> Func)
{
    return Func(_numA, _numB);
}

int Add(int _a, int _b)
{
    return _a + _b;
}
int MyCalc(int _a, int _b)
{
    return 2*_a + _b;
}

int main()
{

    cout << CalcLam(10, 20, [](int a, int b) {return a + b; })<< endl;

    int cnt = 0;

    auto lamFunc = [=]() 
        {
            cout << "람다" << cnt << endl;
        };

    lamFunc();

    std::cout << "Hello World!\n";

    //  게임 : 플레이어(공격력) 몬스터(체력)
    /*
        void Offset(int _atk)
        {
            hp -= _atk;
        }
    
    */
    auto Func = [&](int a, int b)->int
        {
            cnt++;
            cout << "cnt : " << cnt << endl;
            return a + b;
        };

    CalcLam(10, 10, Func);
}

/*
    람다 식 

    무명함수

    일반함수 vs 람다

        함수 실행 위치 밖의 변수를 접근할 수 있는가?

    함수 밖에 있는 데이터를 어떻게 접근하는가?
        
        1. 매개변수로 전달한다.
        2. 전역변수로 사용한다.

        3. 

    함수명 => 함수포인터


    functional

    함수포인터, 람다를 편하게 사용하기 위해서 제공되는 라이브러리

    Func
    ex) function<int(int, int)>

    반환타입, 매개변수 타입을 넣어서 함수포인터 자료형처럼 만든다.
    람다식을 저장할 수 있다.
*/
