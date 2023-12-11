#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

struct CubeSet
{
    int red, green, blue;
};

std::vector<CubeSet> parseGameSets(const std::string &line)
{
    std::vector<CubeSet> sets;
    std::istringstream iss(line);
    std::string token;

    while (std::getline(iss, token, ';'))
    {
        CubeSet set{0, 0, 0};
        std::istringstream setStream(token);
        std::string color;
        int count;

        while (setStream >> count >> color)
        {
            if (color.find("red") != std::string::npos)
                set.red = count;
            else if (color.find("green") != std::string::npos)
                set.green = count;
            else if (color.find("blue") != std::string::npos)
                set.blue = count;
        }

        sets.push_back(set);
    }

    return sets;
}

bool isGamePossible(const std::vector<CubeSet> &sets, int maxRed, int maxGreen, int maxBlue)
{
    for (const auto &set : sets)
    {
        if (set.red > maxRed || set.green > maxGreen || set.blue > maxBlue)
        {
            return false;
        }
    }

    return true;
}

CubeSet findMinimumCubes(const std::vector<CubeSet> &sets)
{
    int minRed = 0, minGreen = 0, minBlue = 0;
    for (const auto &set : sets)
    {
        minRed = std::max(minRed, set.red);
        minGreen = std::max(minGreen, set.green);
        minBlue = std::max(minBlue, set.blue);
    }
    return CubeSet{minRed, minGreen, minBlue};
}

long long cubeSetPower(const CubeSet &set)
{
    return static_cast<long long>(set.red) * set.green * set.blue;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    std::string line;
    long long totalPower = 0;
    int totalID = 0;
    const int colWidth = 15;

    std::cout << std::left << std::setw(10) << "Game ID"
              << std::setw(15) << "Possibility"
              << std::setw(colWidth) << "Min Red"
              << std::setw(colWidth) << "Min Green"
              << std::setw(colWidth) << "Min Blue"
              << std::setw(20) << "Power" << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    while (std::getline(inputFile, line))
    {
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));

        std::istringstream iss(line);
        int gameID;
        iss >> std::skipws >> line >> gameID;
        iss.ignore(2); // Ignore ": "
        std::string setsData;
        std::getline(iss, setsData);

        auto sets = parseGameSets(setsData);
        auto minCubes = findMinimumCubes(sets);
        long long power = cubeSetPower(minCubes);
        bool possible = isGamePossible(sets, 12, 13, 14);

        std::cout << std::left << std::setw(10) << gameID
                  << std::setw(15) << (possible ? "Possible" : "Impossible")
                  << std::setw(colWidth) << minCubes.red
                  << std::setw(colWidth) << minCubes.green
                  << std::setw(colWidth) << minCubes.blue
                  << std::setw(20) << power << std::endl;

        if (possible)
        {
            totalID += gameID;
        }
        totalPower += power;
    }

    std::cout << std::string(85, '-') << std::endl;
    std::cout << "Sum of IDs of possible games: " << totalID << std::endl;
    std::cout << "Total power of minimum sets of cubes that must have been present in each game: " << totalPower << std::endl;

    inputFile.close();
    return 0;
}
