#ifndef LSMTREE_LSMTREE_H
#define LSMTREE_LSMTREE_H

#include <string>
#include "BloomFilter.h"
#include "SortetStringTable.h"
#include <map>
#include <set>

class LsmTree {
public:
    LsmTree() {
        bloomFilter = new BloomFilter();
        sortetStringTable = new SortetStringTable(*bloomFilter);
        dataMemoryUsed = 0;
        srand(time(nullptr));
        logFile.open("../log", std::ios_base::out);
    }
    ~LsmTree() {
        sortetStringTable -> update(data, updates, deleted);
        delete bloomFilter;
        delete sortetStringTable;
        logFile.close();
    }
    std::string insert(std::string &value);
    std::string get(std::string &key);
    void update(std::string &key, std::string &value);
    void erase(std::string &key);
private:
    BloomFilter* bloomFilter;
    SortetStringTable* sortetStringTable;
    std::string getNewKey();
    std::map<std::string, std::string> data, updates;
    std::set<std::string> deleted;
    std::ofstream logFile;
    int dataMemoryUsed;
};


#endif //LSMTREE_LSMTREE_H
