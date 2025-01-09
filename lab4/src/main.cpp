#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

int CalculateSumm(std::vector<int>& array, int idx) {
    int sum = 0;
    for (int i = 0; i < idx; i++) {
        sum += array[i];
    }
    return sum;
}

std::vector<unsigned long long> copy(std::vector<unsigned long long>& text, int idx) {
    std::vector<unsigned long long> newText;
    for (int i = idx; i < text.size(); i++) {
        newText.push_back(text[i]);
    }
    return newText;
}

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

int main() {
    std::vector<unsigned long long> pattern;
    unsigned long long num;

    while (std::cin >> num) {
        pattern.push_back(num);
        char nextchar = std::cin.get();
        while (nextchar == ' ') {
            // Пропускаем символы пробела
            nextchar = std::cin.get();
        }
        if (nextchar == '\n') {
            break;
        }
        else {
            std::cin.unget();
        }
    }

    std::vector<unsigned long long> text;
    std::vector<int> NotEmptyLinesNumbers;
    std::vector<int> SummOfNumbersForEachLine;
    std::vector<int> LineNumbersQuantity;
    std::vector <int> result;
    int numbersOfLine = 0;
    int QuantityOfLines = 0;
    int SummNumbersOfLine = 0;
    std::vector <int> FinalResult;
    std::string line;
    bool not_empty = false;
    int summ = 0;

    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            QuantityOfLines++;
            continue;
        }

        std::stringstream ss(line);
        while (ss >> num) {
            not_empty = true;
            SummNumbersOfLine++;
            numbersOfLine++;
            text.push_back(num);
            if (text.size() >= pattern.size() * 2) {
                result = KMP(pattern, text);
                for (int res = 0; res < result.size(); res++) {
                    result[res] += summ;
                }
                FinalResult.insert(FinalResult.end(), result.begin(), result.end());
                summ += text.size() - pattern.size() + 1;
                text = copy(text, text.size() - pattern.size() + 1);

            }
        }

        QuantityOfLines++;
        NotEmptyLinesNumbers.push_back(QuantityOfLines);
        SummOfNumbersForEachLine.push_back(SummNumbersOfLine);
        LineNumbersQuantity.push_back(numbersOfLine);
        numbersOfLine = 0;
    }
    
    if (pattern.empty() || !(not_empty)) {
        return 0;
    }

    result = KMP(pattern, text);
    for (int i = 0; i < result.size(); i++) {
        result[i] += summ;
    }

    int max = NotEmptyLinesNumbers.back();

    std::vector<int> NumsQuantityOfEveryLines(max, 0);
    std::vector<int> NumbersSumForAllLines(max, 0);
    for (int i = 0; i < NotEmptyLinesNumbers.size(); i++) {
        NumsQuantityOfEveryLines[NotEmptyLinesNumbers[i] - 1] = LineNumbersQuantity[i];
        NumbersSumForAllLines[NotEmptyLinesNumbers[i] - 1] = SummOfNumbersForEachLine[i];
    }
    
    for (int i = 0; i < NotEmptyLinesNumbers.size(); i++) {
        std::cout << NotEmptyLinesNumbers[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < SummOfNumbersForEachLine.size(); i++) {
        std::cout << SummOfNumbersForEachLine[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < LineNumbersQuantity.size(); i++) {
        std::cout << LineNumbersQuantity[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Вспомогательные вектора для корректного вывода:\n";
    for (int i = 0; i < NumsQuantityOfEveryLines.size(); i++) {
        std::cout << NumsQuantityOfEveryLines[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < NumbersSumForAllLines.size(); i++) {
        std::cout << NumbersSumForAllLines[i] << " ";
    }
    std::cout << std::endl;

    FinalResult.insert(FinalResult.end(), result.begin(), result.end());
    for (int k = 0; k < FinalResult.size(); k++) {
        for (int j = 0; j < NumbersSumForAllLines.size(); j++) {
            if (FinalResult[k] <= NumbersSumForAllLines[j]) {
                std::cout << j + 1 << ", " << FinalResult[k] - CalculateSumm(NumsQuantityOfEveryLines, j) << std::endl;
                break;
            }
        }
    }

    return 0;
}
