#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

int verifyGroups(const std::string &row, const std::vector<int> &groupSizes)
{
    int groupIndex = 0, count = 0;

    for (char ch : row)
    {
        if (ch == '#')
        {
            count++;
        }
        else if (ch == '.' && count > 0)
        {
            if (groupIndex >= groupSizes.size() || count != groupSizes[groupIndex])
                return 0;
            groupIndex++;
            count = 0;
        }
    }

    if (count > 0)
    {
        if (groupIndex >= groupSizes.size() || count != groupSizes[groupIndex])
            return 0;
        groupIndex++;
    }

    return groupIndex == groupSizes.size() ? 1 : 0;
}

int countValidCombinations(const std::string &row, const std::vector<int> &groupSizes, int position)
{
    if (position == row.size())
    {
        return verifyGroups(row, groupSizes);
    }

    int count = 0;
    if (row[position] == '?')
    {
        std::string rowCopy = row;
        rowCopy[position] = '.';
        count += countValidCombinations(rowCopy, groupSizes, position + 1);

        rowCopy[position] = '#';
        count += countValidCombinations(rowCopy, groupSizes, position + 1);
    }
    else
    {
        count += countValidCombinations(row, groupSizes, position + 1);
    }

    return count;
}

int calculateArrangements(const std::string &row, const std::vector<int> &groupSizes)
{
    return countValidCombinations(row, groupSizes, 0);
}

int processRow(const std::string &rowInfo)
{
    auto parts = split(rowInfo, ' ');
    std::string row = parts[0];
    std::vector<int> groupSizes;

    // Extracting group sizes
    auto sizes = split(parts[1], ',');
    for (const auto &size : sizes)
    {
        groupSizes.push_back(std::stoi(size));
    }

    return calculateArrangements(row, groupSizes);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    std::ifstream file(filePath);
    std::string line;
    int totalArrangements = 0;

    while (std::getline(file, line))
    {
        totalArrangements += processRow(line);
    }

    std::cout << "Total number of arrangements: " << totalArrangements << std::endl;

    return 0;
}
