#pragma once

#include <string>
#include <atomic>
#include <thread>
#include <openssl/md5.h>

#pragma comment(lib, "libcrypto.lib")
using ConstUChar = const unsigned char;

constexpr size_t HASH_SIZE = MD5_DIGEST_LENGTH * 2;
constexpr size_t sizePinCode_ = 8;

class HashDecode
{
public:
	HashDecode();
	~HashDecode();

	bool putHash(const std::string hash);
	bool getEnable() { return enable_; };
	bool getActive() { return active_; };
	bool getComplete() { return complete_; };
	size_t getThreadsMaxCount() { return threadsMaxCount_; }
	bool evaluatePinCode();
	bool evaluatePinCode(std::string sample);
	std::string getResult();

private:
	char hexToBin(const unsigned char ch);
	void intToUChar(int value, unsigned char buffer[sizePinCode_]);
	bool isValidHash(const char hash[HASH_SIZE]);
	void packHash(const char hash[HASH_SIZE], unsigned char packedhash[MD5_DIGEST_LENGTH]);
	bool comparePackedHash(const unsigned char hash1[MD5_DIGEST_LENGTH], const unsigned char hash2[MD5_DIGEST_LENGTH]);
	void evaluateRange(size_t start, size_t count);
	
	size_t threadsMaxCount_ = 4;

	unsigned char sample_[MD5_DIGEST_LENGTH];
	bool enable_ = false;
	bool active_ = false;
	std::atomic_bool complete_ = false;
	std::atomic_int  activeThreads_ = 0;
	std::atomic_int  result_ = -1;
};