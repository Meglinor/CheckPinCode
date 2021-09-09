// CheckPinCode.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <stdio.h>
#include <utility>
#include <openssl/md5.h>
#pragma comment(lib, "libcrypto.lib")

constexpr size_t HASH_SIZE = MD5_DIGEST_LENGTH*2;

char HexToB(const unsigned char ch)
{
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    return 0;
}

void HexToUChar(const char hash[HASH_SIZE], unsigned char val[MD5_DIGEST_LENGTH])
{
    for (size_t i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        val[i] = ((HexToB(hash[i << 1])) << 4) + HexToB(hash[(i << 1) + 1]);
    }
}

bool CompareUChar(const unsigned char val[MD5_DIGEST_LENGTH],const  unsigned char val2[MD5_DIGEST_LENGTH])
{
    for (size_t i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        if (val[i] != val2[i]) return false;
    }
    return true;
}

std::pair<bool, int> GetPinCode(size_t start, size_t count, unsigned char search[MD5_DIGEST_LENGTH])
{
    const size_t size = 9;
    auto result = std::make_pair(false, -1);
    for (size_t i = start; i < count; ++i)
    {
        unsigned char md5digest[MD5_DIGEST_LENGTH];
        unsigned char buffer[size];
        snprintf(reinterpret_cast<char*>(buffer), size, "%08u", i);
        MD5(buffer, size - 1, md5digest);
        if (CompareUChar(md5digest, search))
        {
            result.first = true;
            result.second = i;
            break;
        }
    }
    return result;
}


int main()
{
    auto hash = "25d55ad283aa400af464c76d713c07ad"; // pin 12345678
    unsigned char hashb[MD5_DIGEST_LENGTH];
    HexToUChar(hash, hashb);
    printf("Start search: hash - %s \n", hash);
    auto result = GetPinCode(02345677, 12345679, hashb);
    printf("Result is %08u \n", result.second);
    return 0;
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
