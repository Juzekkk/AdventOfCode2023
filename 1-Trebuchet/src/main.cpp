#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
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
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <conversion_option> <input_file_path>" << std::endl;
        return 1;
    }

    std::string conversionOption = argv[1];
    std::string inputFilePath = argv[2];

    std::ifstream file(inputFilePath);
    std::string line;
    int sum = 0;

    if (!file)
    {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    while (getline(file, line))
    {
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));

        if (!line.empty())
        {
            if (conversionOption == "one" || conversionOption == "two")
            {
                std::string convertedLine = conversionOption == "two" ? convertSpelledOutDigits(line) : line;
                int result = concatenateFirstAndLastDigit(convertedLine);
                std::cout << line << "\t" << convertedLine << "\t" << result << std::endl;
                sum += result;
            }
            else
            {
                std::cerr << "Invalid conversion option: " << conversionOption << std::endl;
                return 1;
            }
        }
    }

    std::cout << "Sum of all results:\t" << sum << std::endl;

    file.close();
    return 0;
}