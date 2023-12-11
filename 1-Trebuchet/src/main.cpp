#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <iomanip>
#include <map>

std::string convertSpelledOutDigits(const std::string &line)
{
    std::map<std::string, std::string> spelledOutToNumbers = {
        {"one", "o1e"},
        {"two", "t2o"},
        {"three", "t3e"},
        {"four", "f4r"},
        {"five", "f5e"},
        {"six", "s6x"},
        {"seven", "s7n"},
        {"eight", "e8t"},
        {"nine", "n9e"}};

    std::string convertedLine = line;

    for (const auto &entry : spelledOutToNumbers)
    {
        size_t pos = convertedLine.find(entry.first);
        while (pos != std::string::npos)
        {
            convertedLine.replace(pos, entry.first.length(), entry.second);
            pos = convertedLine.find(entry.first, pos + entry.second.length());
        }
    }

    return convertedLine;
}

int concatenateFirstAndLastDigit(const std::string &line)
{
    char firstDigit = '\0', lastDigit = '\0';

    for (char ch : line)
    {
        if (isdigit(ch))
        {
            if (firstDigit == '\0')
            {
                firstDigit = ch;
            }
            lastDigit = ch;
        }
    }

    if (firstDigit != '\0')
    {
        std::string concatenatedNumber;
        concatenatedNumber += firstDigit;
        concatenatedNumber += (lastDigit == '\0') ? firstDigit : lastDigit;

        return std::stoi(concatenatedNumber);
    }

    return 0;
}

int sumVector(const std::vector<int> &numbers)
{
    int sum = 0;
    for (int number : numbers)
    {
        sum += number;
    }
    return sum;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file_path>" << std::endl;
        return 1;
    }

    std::string inputFilePath = argv[1];
    std::ifstream file(inputFilePath);
    std::string line;
    int sum1 = 0, sum2 = 0;

    if (!file)
    {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    const int colWidth = 50;

    // Print table for better representation of data
    std::cout << std::left << std::setw(colWidth) << "Original Line"
              << std::setw(colWidth) << "Converted Line"
              << std::setw(colWidth) << "Result 1"
              << std::setw(colWidth) << "Result 2" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    while (getline(file, line))
    {
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));

        if (!line.empty())
        {
            std::string convertedLine = convertSpelledOutDigits(line);
            int result1 = concatenateFirstAndLastDigit(line);
            int result2 = concatenateFirstAndLastDigit(convertedLine);

            std::cout << std::left << std::setw(colWidth) << line
                      << std::setw(colWidth) << convertedLine
                      << std::setw(colWidth) << result1
                      << std::setw(colWidth) << result2 << std::endl;

            sum1 += result1;
            sum2 += result2;
        }
    }

    std::cout << std::string(80, '-') << std::endl;
    std::cout << std::left << std::setw(colWidth) << "Sum of all results1:"
              << std::setw(colWidth) << sum1 << std::endl;
    std::cout << std::left << std::setw(colWidth) << "Sum of all results2:"
              << std::setw(colWidth) << sum2 << std::endl;

    file.close();
    return 0;
}