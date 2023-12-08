#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

using namespace std;

// Represents a position on the grid.
struct Position
{
    int row, col;
};

// Overload the equality operator for Position.
bool operator==(const Position &lhs, const Position &rhs)
{
    return lhs.row == rhs.row && lhs.col == rhs.col;
}

// Custom hash function for Position to be used in unordered_map and unordered_set.
namespace std
{
    template <>
    struct hash<Position>
    {
        size_t operator()(const Position &pos) const
        {
            return hash<int>()(pos.row) ^ hash<int>()(pos.col);
        }
    };
}

// Reads the puzzle grid from a file.
vector<vector<char>> readGridFromFile(const string &filePath)
{
    vector<vector<char>> grid;
    ifstream file(filePath);
    string line;
    while (getline(file, line))
    {
        grid.push_back(vector<char>(line.begin(), line.end()));
    }
    return grid;
}

// Solves the puzzle by calculating the sums for both parts.
pair<long long, long long> solvePuzzle(const vector<vector<char>> &grid)
{
    const int rows = grid.size();
    const int cols = grid[0].size();
    long long part1Sum = 0, part2Sum = 0;
    unordered_map<Position, vector<int>> gearToNumbers;
    const string symbols = "*+-/#$%=&@";

    // Iterate through each row and column of the grid.
    for (int row = 0; row < rows; ++row)
    {
        unordered_set<Position> gears;
        int currentNumber = 0;
        bool isPartNumber = false;

        for (int col = 0; col <= cols; ++col)
        {
            if (col < cols && isdigit(grid[row][col]))
            {
                // Accumulate the number.
                currentNumber = currentNumber * 10 + (grid[row][col] - '0');
                // Check for adjacent symbols and gears.
                for (int dr = -1; dr <= 1; ++dr)
                {
                    for (int dc = -1; dc <= 1; ++dc)
                    {
                        if (0 <= row + dr && row + dr < rows && 0 <= col + dc && col + dc < cols)
                        {
                            char ch = grid[row + dr][col + dc];
                            if (symbols.find(ch) != string::npos)
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
                // If the number is a part number, add it to part1Sum.
                if (isPartNumber)
                {
                    part1Sum += currentNumber;
                }
                // Associate the number with gears for part 2 calculation.
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

    // Calculate part 2 sum based on gear ratios.
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
        cout << "Usage: " << argv[0] << " <file_path>" << endl;
        return 1;
    }

    string filePath = argv[1];
    auto grid = readGridFromFile(filePath);
    auto [part1Sum, part2Sum] = solvePuzzle(grid);

    cout << "Part 1 Sum: " << part1Sum << endl;
    cout << "Part 2 Sum: " << part2Sum << endl;

    return 0;
}
