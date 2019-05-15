#include "LsmTree.h"

std::string LsmTree::insert(std::string &value) {
    std::string key = getNewKey();
    logFile << "insert " << key << " " << value << std::endl;
    bloomFilter -> add(key);
    dataMemoryUsed += (key.length() + value.length());
    data[key] = value;
    if(dataMemoryUsed > bufferSizeForUpdate) {
        logFile << "update sortedStringTable" << std::endl;
        sortetStringTable -> update(data, updates, deleted);
        dataMemoryUsed = 0;
    }
    return key;
}

std::string LsmTree::get(std::string &key) {
    std::string result = "";
    logFile << "get " << key << std::endl;
    if(!bloomFilter -> count(key) || deleted.count(key))
        return "";
    if(data.count(key))
       return data[key];
    if(updates.count(key))
        return updates[key];
    return sortetStringTable -> get(key);
}

void LsmTree::update(std::string &key, std::string &value) {
    logFile << "update " << key << " " << value << "\n";
    if(!bloomFilter -> count(key))
        return;
    if(data.count(key))
        data[key] = value;
    else {
        if(sortetStringTable -> get(key).empty())
            return;
        updates[key] = value;
        dataMemoryUsed += (key.length() + value.length());
        if(dataMemoryUsed > bufferSizeForUpdate) {
            logFile << "update sortedStringTable" << std::endl;
            sortetStringTable -> update(data, updates, deleted);
            dataMemoryUsed = 0;
        }
    }
}

std::string LsmTree::getNewKey() {
    std::string result = "";
    for(int i = 0; i < keyLength; i++)
    {
        int next = rand() % 36;
        if(next < 26)
            result += ('a' + next);
        else
            result += ('0' + next - 26);
    }
    if(bloomFilter -> count(result))
        return getNewKey();
    return result;
}

void LsmTree::erase(std::string &key) {
    logFile << "erase " << key << std::endl;
    if(!bloomFilter -> count(key))
        return;
    if(data.count(key))
        data.erase(key);
    else {
        deleted.insert(key);
        dataMemoryUsed += (key.length());
        if(dataMemoryUsed > bufferSizeForUpdate) {
            logFile << "update sortedStringTable" << std::endl;
            sortetStringTable -> update(data, updates, deleted);
            dataMemoryUsed = 0;
        }
    }
}
