#include <queue>
#include <sys/stat.h>
#include <cstring>
#include "SortetStringTable.h"

std::string SortetStringTable::get(std::string &key) {
    std::string result;
    if(index.empty())
        return result;
    int left = 0, right = index.size() - 1, middle;
    while(left < right) {
        middle = (left + right + 1) / 2;
        if(index[middle] > key)
            right = middle - 1;
        else
            left = middle;
    }
    std::ifstream fin("../data/" + std::to_string(left));
    std::string tmp;
    while(getline(fin, tmp)) {
        bool find = true;
        for(int i = 0; i < keyLength; i++)
            if(tmp[i] != key[i]) {
                find = false;
                break;
            }
        if(find) {
            for(int i = keyLength; i < tmp.size(); i++)
                result += tmp[i];
        }
    }
    return result;
}

void SortetStringTable::update(std::map<std::string, std::string> &data, std::map<std::string, std::string> &update, std::set<std::string> &deleted) {
    if(index.empty()) {
        deleted.clear();
        int cntFileName = 0;
        std::ofstream fout("../data/" + std::to_string(cntFileName));
        if(!fout.is_open()) {
            if(mkdir("../data/", 0777) == -1)
                std::cerr << "Error :  " << std::strerror(errno) << std::endl;
            fout.close();
            fout.open("../data/" + std::to_string(cntFileName), std::ios_base::out);
        }
        index.push_back(data.begin()->first);
        int bufferSize = 0;
        for (auto &it : data) {
            if(bufferSize > bufferSizeForUpdate) {
                bufferSize = 0;
                fout.close();
                cntFileName++;
                fout.open("../data/" + std::to_string(cntFileName), std::ios_base::out);
                index.push_back(it.first);
            }
            fout << (it.first) << it.second << "\n";
            bufferSize += (it.first.size() + it.second.size());
        }
        fout.close();
    }
    else {
        std::queue<std::string> lines;
        int cntReadFile = 0, cntWriteFile = 0, cntOfFiles = index.size(), bufferSize = 0;
        index.clear();
        std::ifstream fin("../data/" + std::to_string(cntReadFile));
        auto dataIterator = data.begin();
        auto updateIterator = update.begin();
        std::string tmp;
        while(cntReadFile != cntOfFiles) {
            if(getline(fin, tmp)) {
                std::string key = tmp.substr(0, keyLength);
                if(deleted.count(key))
                    continue;
                if(updateIterator != update.end() && updateIterator->first == key) {
                    tmp = updateIterator->first + updateIterator->second;
                    key = tmp.substr(0, keyLength);
                    updateIterator++;
                }
                while(dataIterator != data.end() && dataIterator -> first < key) {
                    lines.push(dataIterator->first + dataIterator->second);
                    bufferSize += lines.back().size();
                    while(bufferSize >= bufferSizeForUpdate && cntWriteFile < cntReadFile)
                        writeDataToFile(lines, cntWriteFile++, bufferSize);
                    dataIterator++;
                }
                lines.push(tmp);
                bufferSize += lines.back().size();
                while(bufferSize >= bufferSizeForUpdate && cntWriteFile < cntReadFile)
                    writeDataToFile(lines, cntWriteFile++, bufferSize);
            }
            else {
                cntReadFile++;
                if(cntReadFile != cntOfFiles) {
                    fin.close();
                    fin.open("../data/" + std::to_string(cntReadFile), std::ios_base::in);
                }
                else {
                    while(dataIterator != data.end()) {
                        lines.push(dataIterator->first + dataIterator->second);
                        dataIterator++;
                    }
                }
            }
        }
        while(!lines.empty())
            writeDataToFile(lines, cntWriteFile++, bufferSize);
        std::ofstream fout;
        while(cntWriteFile <= cntOfFiles) {
            fout.open("../data/" + std::to_string(cntWriteFile), std::ios_base::out);
            fout.close();
            cntWriteFile++;
        }
    }
    data.clear();
    update.clear();
    deleted.clear();
}

void SortetStringTable::writeDataToFile(std::queue<std::string> &lines, int cntOfFiles, int &bufferSize) {
    std::ofstream fout("../data/" + std::to_string(cntOfFiles));
    index.push_back(lines.front().substr(0, keyLength));
    int tmpBufferSize = 0;
    while(!lines.empty() && tmpBufferSize < bufferSizeForUpdate) {
        fout << lines.front() << "\n";
        bufferSize -= lines.front().size();
        tmpBufferSize += lines.front().size();
        lines.pop();
    }
    fout.close();
}
