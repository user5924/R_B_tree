// R_B_tree.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#include "strings.h"
#include "RB_tree.h"

void sleep(_int64 time)
{
    _int64 start = clock();
    _int64 finish = start + CLOCKS_PER_SEC * time / 1000;

    while (clock() < finish);
}

int main()
{
    R_B_tree<float> test;
    char* arg,* arg_2;
    int i;
    fstream f;
    f.open("out.txt", ios::out | ios::trunc);
    bool res;
    int size = 150000;
    srand(time(NULL));
    vector <char*> buff;

    for (i = size; i < size * 2; i += 1)
    {
        arg = rand_str(7);
        res = test.add(arg, i * 2);
        buff.push_back(copy_str(arg));
        delete[] arg;
    }

    for(i = 0; i < size; i += 1)
    {
        int one, two;
        one = rand() % size;
        two = rand() % size;

        swap(buff[one], buff[two]);
    }

    sleep(10000);
    
    for (i = 0; i < size; i += 1)
    {
        test.del(buff[i]);  
        delete[] buff[i];
    }
    
    sleep(10000);
    //test.post(f);
    f << "\n\n\n\n";
   //test.post_recurs(f);
    
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
