#include <iostream>
#include <vector>

std::vector<int> KMP(const std::vector<unsigned long long>& pattern, std::vector<unsigned long long>& text) {
    std::vector<int> PI(pattern.size());
    
    PI[0] = 0;
    int j = 0, i = 1;
    while (i < static_cast<int>(pattern.size())) {
        if (pattern[i] != pattern[j]) {
            if (j == 0) {
                PI[i] = 0;
                i++;
            }
            else {
                j = PI[j - 1];
            }
        }
        else {
            PI[i] = j + 1;
            i++;
            j++;
        }
    }
    
    i = 0;
    j = 0;
    std::vector<int> result;
    while (i < static_cast<int>(text.size())) {
        if (text[i] == pattern[j]) {                         
            i++;
            j++;
            if (j == static_cast<int>(pattern.size())) {
                result.push_back(i - j + 1);
                j = PI[j - 1];
            }
        }
        else {
            if (j > 0) {
                j = PI[j - 1];
            }
            else {
                i++;
            }
        }
    }
    return result;
}


std::vector<int> NaiveAlg(const std::vector<unsigned long long>& text, const std::vector<unsigned long long>& pattern) {
    std::vector<int> result;
    int textSize = text.size();
    int patternSize = pattern.size();

    for (int i = 0; i <= textSize - patternSize; i++) {
        bool found = true;
        for (int j = 0; j < patternSize; j++) {
            if (text[i + j] != pattern[j]) {
                found = false;
                break;
            }
        }
        if (found) {
            result.push_back(i + 1);
        }
    }

    return result;
}

