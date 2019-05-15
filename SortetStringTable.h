#ifndef LSMTREE_SORTETSTRINGTABLE_H
#define LSMTREE_SORTETSTRINGTABLE_H

#include <queue>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "BloomFilter.h"

#define keyLength 10
#define bufferSizeForUpdate 1024 //only for testing, should be 1-2mb

class SortetStringTable {
public:
    SortetStringTable(BloomFilter& bloomFilter) {
        int cntOfFiles = 0;
        std::ifstream fin;
        std::string tmp;
        while(1) {
            fin.open("../data/" + std::to_string(cntOfFiles), std::ios_base::in);
            if(!fin.is_open())
                break;
            if(!getline(fin, tmp))
                break;
            cntOfFiles++;
            index.push_back(tmp.substr(0,keyLength));
            std::string key = tmp.substr(0,keyLength);
            bloomFilter.add(key);
            while(getline(fin, tmp)) {
                key = tmp.substr(0,keyLength);
                bloomFilter.add(key);
            }
            fin.close();
        }
    }
    std::string get(std::string &key);
    void update(std::map<std::string, std::string> &data, std::map<std::string, std::string> &update, std::set<std::string> &deleted);
private:
    void writeDataToFile(std::queue<std::string> &lines, int cntOfFiles, int &bufferSize);
    std::vector<std::string> index;
};


#endif //LSMTREE_SORTETSTRINGTABLE_H
