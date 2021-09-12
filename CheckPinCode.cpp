// CheckPinCode.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <chrono>
#include "HashDecode.h"

using std::cout, std::cin;

void printInterface()
{
    cout << "**************************************************************************\n";
    cout << "*                           Pin Code Hash Decode                         *\n";
    cout << "**************************************************************************\n";
}

auto inputHash()
{
    cout << "Please enter valid hash code.\n";
    cout << "hash>";
    std::string input;
    cin >> input;
    return input;
}

bool doWork()
{
    printInterface();
    std::string hash;
    HashDecode hashDecode;
    do
    {
        hash = inputHash();
    } while (!hashDecode.putHash(hash));

    auto start = std::chrono::high_resolution_clock::now();
    if (hashDecode.getEnable())
    {
        hashDecode.evaluatePinCode();
    }
    cout << "Processing: ";
    while (hashDecode.getActive())
    {
        cout << ".";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    cout << "Evaluation complete. \n";
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;
    cout << "Evaluation time: " << duration.count() << "\n";
    cout << "Using " << hashDecode.getThreadsMaxCount() << " threads.\n";

    if (hashDecode.getComplete())
    {
        cout << "Evaluated Pin Code is ";
        cout << hashDecode.getResult() << "\n";
    }
    else
    {
        cout << "Pin code is not find.\n";
    }

    cout << "Do you want to repeat evaluation?[y/n] ";
    char c = 0;
    cin >> c;

    return c == 'y' || c == 'Y';
}

int main()
{
    while (doWork())
    {
        system("cls");
    }
   return 0;
}