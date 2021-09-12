#include "HashDecode.h"
#include <iostream>

constexpr int pow(int base, int ехр) noexcept
{
    auto result = 1;
    for (int i = 0; i < ехр; ++i) result *= base;
    return result;
} 

HashDecode::HashDecode()
{
    auto i = std::thread::hardware_concurrency();
    if (i > 0) threadsMaxCount_ = i;
}

HashDecode::~HashDecode()
{
    complete_ = true;
    while (activeThreads_ > 0)
    {
        ;
    }
}

bool HashDecode::putHash(const std::string hash)
{
    if (!isValidHash(hash.c_str()))
    {
        enable_ = false;
    }
    else
    {
        packHash(hash.c_str(), sample_);
        enable_ = true;
    }
    return enable_;
}

bool HashDecode::evaluatePinCode()
{
    if (!enable_)
    {
        return false;
    }
    complete_ = false;    
    activeThreads_ = 0;
    result_ = -1;

    size_t step = static_cast<size_t>(pow(10, sizePinCode_ ) / (threadsMaxCount_ ));
    active_ = true;
    for (size_t i = 0; i < threadsMaxCount_; ++i)
    {
        std::thread th(&HashDecode::evaluateRange,this, i * step, (i + 1) * step - 1);
        th.detach();
    }
}

bool HashDecode::evaluatePinCode(std::string sample)
{    
    if (!putHash(sample))
    {
        return false;
    }
    enable_ = true;
    evaluatePinCode();
}

bool HashDecode::isValidHash(const char hash[HASH_SIZE])
{
    for (size_t i = 0; i < HASH_SIZE; ++i)
    {
        if (hexToBin(hash[i]) == -1) return false;
    }
    return true;
}

char HashDecode::hexToBin(const unsigned char ch)
{
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    return -1;
}

void HashDecode::intToUChar(int value, unsigned char buffer[sizePinCode_])
{
    try
    {
        auto exponent = static_cast<size_t>(pow(10, sizePinCode_ - 1));
        size_t i = 0;
        while (exponent > 0)
        {
            short int digit = (short int)(value / exponent);
            buffer[i] = (unsigned char)digit + '0';
            value -= digit * exponent;
            exponent /= 10;
            ++i;
        } 
    }
    catch (std::exception e)
    {
        std::cout << "HashDecode::intToUChar - exception. Value - " << value << "/ Message - " << e.what() << "\n";
        throw;
    }
}

void HashDecode::packHash(const char hash[HASH_SIZE], unsigned char packedhash[MD5_DIGEST_LENGTH])
{
    for (size_t i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        packedhash[i] = ((hexToBin(hash[i << 1])) << 4) + hexToBin(hash[(i << 1) + 1]);
    }
}

bool HashDecode::comparePackedHash(const unsigned char hash1[MD5_DIGEST_LENGTH], const unsigned char hash2[MD5_DIGEST_LENGTH])
{
    return memcmp(hash1, hash2, MD5_DIGEST_LENGTH) == 0;
}

void HashDecode::evaluateRange(size_t start, size_t count)
{  
    size_t i = 0;
    try
    {
        ++activeThreads_;
        unsigned char buffer[sizePinCode_];
        unsigned char md5digest[MD5_DIGEST_LENGTH];
        for (i = start; i < count && !complete_; ++i)
        {
            intToUChar(i, buffer);
            MD5(buffer, sizePinCode_, md5digest);
            if (comparePackedHash(md5digest, sample_))
            {
                complete_ = true;
                result_ = i;
            }
        }        
        if (--activeThreads_ == 0) active_ = false;
    }
    catch (std::exception e)
    {
        std::cout << "HashDecode::evaluateRange - exception. Index = " << i << ". Message - " << e.what() << "\n";
        if (--activeThreads_ == 0) active_ = false;
    }
}

std::string HashDecode::getResult()
{    
    if (complete_)
    {
        if (result_ > 0)
        {
            unsigned char buffer[sizePinCode_ + 1];
            intToUChar(result_, buffer);
            buffer[sizePinCode_] = 0;
            return std::string(reinterpret_cast<char*>(buffer));
        }
        else
        {
            return std::string("Pin code is not find.");
        }
    }
    else
    {
        return std::string("Evaluation is not complete.");
    }
}