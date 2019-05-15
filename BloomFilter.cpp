#include "BloomFilter.h"

void BloomFilter::add(std::string &key) {
    for(int i = 0; i < countOfHashFunction; i++)
        table.set(getHash(key, i));
}

bool BloomFilter::count(std::string &key) {
    for(int i = 0; i < countOfHashFunction; i++)
        if(!table.test(getHash(key, i)))
            return false;
    return true;
}

int BloomFilter::getHash(std::string &key, int hashNumber) {
    int result = 0, f = 1;
    for(char c : key) {
        if(isdigit(c)) {
            result += ((c - '0' + 26) * f) % mod[hashNumber];
            result %= mod[hashNumber];
        }
        else {
            result += ((c - 'a') * f) % mod[hashNumber];
            result %= mod[hashNumber];
        }
        f *= p[hashNumber];
        f %= mod[hashNumber];
    }
    return result;
}
