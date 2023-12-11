#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

struct Position
{
    int row, col;
};

// Overload the equality operator for Position.
bool operator==(const Position &lhs, const Position &rhs)
{
    return lhs.row == rhs.row && lhs.col == rhs.col;
}

template <>
struct std::hash<Position>
{
    size_t operator()(const Position &pos) const
    {
        return hash<int>()(pos.row) ^ hash<int>()(pos.col);
    }
};

std::vector<std::vector<char>> readGridFromFile(const std::string &filePath)
{
    std::vector<std::vector<char>> grid;
    std::ifstream file(filePath);
    std::string line;
    while (getline(file, line))
    {
        grid.push_back(std::vector<char>(line.begin(), line.end()));
    }
    return grid;
}

std::pair<long long, long long> solvePuzzle(const std::vector<std::vector<char>> &grid)
{
    const int rows = grid.size();
    const int cols = grid[0].size();
    long long part1Sum = 0, part2Sum = 0;
    std::unordered_map<Position, std::vector<int>> gearToNumbers;
    const std::string symbols = "*+-/#$%=&@";

    for (int row = 0; row < rows; ++row)
    {
        std::unordered_set<Position> gears;
        int currentNumber = 0;
        bool isPartNumber = false;

        for (int col = 0; col <= cols; ++col)
        {
            if (col < cols && isdigit(grid[row][col]))
            {
                currentNumber = currentNumber * 10 + (grid[row][col] - '0');
                for (int dr = -1; dr <= 1; ++dr)
                {
                    for (int dc = -1; dc <= 1; ++dc)
                    {
                        if (0 <= row + dr && row + dr < rows && 0 <= col + dc && col + dc < cols)
                        {
                            char ch = grid[row + dr][col + dc];
                            if (symbols.find(ch) != std::string::npos)
                            {
                                isPartNumber = true;
                            }
                            if (ch == '*')
                            {
                                gears.insert({row + dr, col + dc});
                            }
                        }
                    }
                }
            }
            else if (currentNumber > 0)
            {
                if (isPartNumber)
                {
                    part1Sum += currentNumber;
                }
                for (const Position &gear : gears)
                {
                    gearToNumbers[gear].push_back(currentNumber);
                }
                currentNumber = 0;
                isPartNumber = false;
                gears.clear();
            }
        }
    }

    for (const auto &item : gearToNumbers)
    {
        if (item.second.size() == 2)
        {
            part2Sum += item.second[0] * item.second[1];
        }
    }

    return {part1Sum, part2Sum};
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    auto grid = readGridFromFile(filePath);
    auto [part1Sum, part2Sum] = solvePuzzle(grid);

    std::cout << "Part 1 Sum: " << part1Sum << std::endl;
    std::cout << "Part 2 Sum: " << part2Sum << std::endl;

    return 0;
}
