#include <iostream>
#include <set>
#include "LsmTree.h"

void checkOperations(LsmTree& db, int CountOfTest) {
    std::string value = "Some text", key = db.insert(value);
    std::vector<std::string> keys;
    keys.push_back(key);
    for(int i = 0; i < CountOfTest; i++) {
        key = db.insert(value);
        keys.push_back(key);
    }
    for(auto& it : keys)
        if(db.get(it).empty()) {
            std::cout << "Fail get " << it << "\n";
            db.get(it);
        }
    value = "other text";
    for(auto& it : keys)
        db.update(it, value);
    for(auto& it : keys)
        if(db.get(it) != value)
            std::cout << "Fail update " << it << "\n";
    for(auto& it : keys)
        db.erase(it);
    for(auto& it : keys)
        if(!db.get(it).empty())
            std::cout << "Fail erase " << it << "\n";
}

void interactive(LsmTree& db) {
    std::cout << "Comands:\n";
    std::cout << "add [enter] value //return key\n";
    std::cout << "get key // return value for key\n";
    std::cout << "update key [enter] value\n";
    std::cout << "erase key\n";
    std::cout << "stop\n";
    while(1) {
        std::string command;
        std::cin >> command;
        if(command == "add") {
            std::string value;
            getline(std::cin, value);
            getline(std::cin, value);
            std::cout << "Key = " << db.insert(value) << "\n";
        }
        else if(command == "get") {
            std::string key;
            std::cin >> key;
            if(db.get(key).empty())
                std::cout << "No " << key << " in tree\n";
            else
                std::cout << "Value = " << db.get(key) << "\n";
        }
        else if(command == "update") {
            std::string key, value;
            std::cin >> key;
            getline(std::cin, value);
            getline(std::cin, value);
            db.update(key, value);
        }
        else if(command == "erase") {
            std::string key;
            std::cin >> key;
            db.erase(key);
        }
        else if(command == "stop")
            break;
        else
            std::cout << "Bad command\n";
    }
}

int main() {
    LsmTree db;
//    checkOperations(db, 1000);
    interactive(db);
    return 0;
}