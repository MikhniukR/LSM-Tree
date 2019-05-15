#ifndef LSMTREE_BLOOMFILTER_H
#define LSMTREE_BLOOMFILTER_H

#define filterSize 1024*1024
#define countOfHashFunction 10

#include <string>
#include <bitset>

class BloomFilter {
public:
    void add(std::string &key);
    bool count(std::string &key);
private:
    int getHash(std::string &key, int hashNumber);
    std::bitset<filterSize> table;
    int mod[countOfHashFunction] = {1048573, 1048573, 1048573, 1048573, 1048573,
                   1048571, 1048571, 1048571, 1048571, 1048571};
    int p[countOfHashFunction] = {37, 41, 43, 47, 53, 37, 41, 43, 47, 53};
};


#endif //LSMTREE_BLOOMFILTER_H
