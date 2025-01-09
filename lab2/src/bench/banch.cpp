#include <iostream>
#include <chrono>
#include <map>
#include "patr.hpp"

int main () {
    patr *PATRICIA = new patr();
    std::map<std::string, unsigned long long> container;
    std::chrono::time_point<std::chrono::system_clock> startTs, endTs;
    uint64_t patriciaInsertTime = 0;
    uint64_t containerInsertTime = 0;
    uint64_t patriciaAtTime = 0;
    uint64_t containerAtTime = 0;
    uint64_t patriciaRemoveTime = 0;
    uint64_t containerRemoveTime = 0;
    startTs = std::chrono::system_clock::now();
    try {
        PATRICIA->Add("abcdefgfqifbquifiiqbcqejqocnqocnq", 1100);
    }
    catch (const std::exception &e) {}
    try {
        PATRICIA->Add("afmqodefgcmcmopdpqpqpqpvpqkvmwonofwp", 1100);
    }
    catch (const std::exception &e) {}
    try {
        PATRICIA->Add("avpqpflqfgcmqfoqjjfoqwofjweipjoqwwkognq", 1100);
    }
    catch (const std::exception &e) {}
    try {
        PATRICIA->Add("qqefgqffqfqohihbvwqegihwiqgfhioqwejioqr", 1100);
    }
    catch (const std::exception &e) {}
        try {
        PATRICIA->Add("a", 1100);
    }
    catch (const std::exception &e) {}
        try {
        PATRICIA->Add("b", 1100);
    }
    catch (const std::exception &e) {}
        try {
        PATRICIA->Add("c", 1100);
    }
    catch (const std::exception &e) {}
        try {
        PATRICIA->Add("d", 1100);
    }
    catch (const std::exception &e) {}
        try {
        PATRICIA->Add("e", 1100);
    }
    catch (const std::exception &e) {}
        try {
        PATRICIA->Add("f", 1100);
    }
    catch (const std::exception &e) {}
        try {
        PATRICIA->Add("g", 1100);
    }
    catch (const std::exception &e) {}

    endTs = std::chrono::system_clock::now();
    patriciaInsertTime += std::chrono::duration_cast<std::chrono::microseconds>(endTs - startTs ).count();
    std::cout << "Insert in patricia: " << patriciaInsertTime << "ms" << std::endl;

    startTs = std::chrono::system_clock::now();
    container.insert(std::make_pair("abcdefgfqifbquifiiqbcqejqocnqocnq", 1100));
    container.insert(std::make_pair("afmqodefgcmcmopdpqpqpqpvpqkvmwonofwp", 1100));
    container.insert(std::make_pair("avpqpflqfgcmqfoqjjfoqwofjweipjoqwwkognq", 1100));
    container.insert(std::make_pair("qqefgqffqfqohihbvwqegihwiqgfhioqwejioqr", 1100));
    container.insert(std::make_pair("a", 1100));
    container.insert(std::make_pair("b", 1100));
    container.insert(std::make_pair("c", 1100));
    container.insert(std::make_pair("d", 1100));
    container.insert(std::make_pair("e", 1100));
    container.insert(std::make_pair("f", 1100));
    container.insert(std::make_pair("g", 1100));
    endTs = std::chrono::system_clock::now();
    containerInsertTime += std::chrono::duration_cast<std::chrono::microseconds>(endTs - startTs ).count();
    std::cout << "Insert in map container: " << containerInsertTime << "ms" << std::endl;
    
    std::cout << std::endl;

    startTs = std::chrono::system_clock::now();
    try {
        PATRICIA->Get_Value("avpqpflqfgcmqfoqjjfoqwofjweipjoqwwkognq");
    }
    catch (const std::exception &e) {}
    endTs = std::chrono::system_clock::now();
    patriciaAtTime += std::chrono::duration_cast<std::chrono::microseconds>(endTs - startTs ).count();
    std::cout << "Search in patricia: " << patriciaAtTime << "ms" << std::endl;

    startTs = std::chrono::system_clock::now();
    container.at("avpqpflqfgcmqfoqjjfoqwofjweipjoqwwkognq");
    endTs = std::chrono::system_clock::now();
    containerAtTime += std::chrono::duration_cast<std::chrono::microseconds>(endTs - startTs ).count();
    std::cout << "Search in map container: " << containerAtTime << "ms" << std::endl;

    std::cout << std::endl;

    startTs = std::chrono::system_clock::now();
    try {
        PATRICIA->Remove("avpqpflqfgcmqfoqjjfoqwofjweipjoqwwkognq");
    }
    catch (std::exception &e) {}
    endTs = std::chrono::system_clock::now();
    patriciaRemoveTime += std::chrono::duration_cast<std::chrono::microseconds>(endTs - startTs ).count();
    std::cout << "Remove in patricia: " << patriciaRemoveTime << "ms" << std::endl;

    startTs = std::chrono::system_clock::now();
    container.erase("avpqpflqfgcmqfoqjjfoqwofjweipjoqwwkognq");
    endTs = std::chrono::system_clock::now();
    containerRemoveTime += std::chrono::duration_cast<std::chrono::microseconds>(endTs - startTs ).count();
    std::cout << "Remove in map container: " << containerRemoveTime << "ms" << std::endl;






}