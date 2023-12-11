#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cmath>
#include <climits>
#include <numeric>

using Grid = std::vector<std::vector<char>>;
using Position = std::pair<int, int>;

std::vector<Position> findHashPositions(const Grid& grid) {
    std::vector<Position> positions;
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == '#') {
                positions.emplace_back(i, j);
            }
        }
    }
    return positions;
}

long long calculateModifiedManhattanDistance(const Position& a, const Position& b, 
                                       const std::vector<size_t>& emptyRows, 
                                       const std::vector<size_t>& emptyCols,
                                       long long factor) {
    long long distance = 0;

    for (int i = std::min(a.first, b.first); i <= std::max(a.first, b.first); ++i) {
        distance += (std::find(emptyRows.begin(), emptyRows.end(), i) != emptyRows.end()) ? factor : 1;
    }

    for (int j = std::min(a.second, b.second); j <= std::max(a.second, b.second); ++j) {
        distance += (std::find(emptyCols.begin(), emptyCols.end(), j) != emptyCols.end()) ? factor : 1;
    }

    return distance - 2;
}

std::vector<long long> findAllPairwiseDistances(const std::vector<Position>& positions,
                                          const std::vector<size_t>& emptyRows,
                                          const std::vector<size_t>& emptyCols,
                                          long long factor) {
    std::vector<long long> allDistances;

    for (size_t i = 0; i < positions.size(); ++i) {
        for (size_t j = i + 1; j < positions.size(); ++j) {
            long long distance = calculateModifiedManhattanDistance(positions[i], positions[j], emptyRows, emptyCols, factor);
            allDistances.push_back(distance);
        }
    }

    return allDistances;
}


Grid readGridFromFile(const std::string &filePath) {
    Grid grid;
    std::ifstream file(filePath);
    std::string line;
    while (getline(file, line))
    {
        grid.push_back(std::vector<char>(line.begin(), line.end()));
    }
    return grid;
}

std::vector<size_t> emptyRows(const Grid& grid) {
    std::vector<size_t> emptyRows;
    for (size_t i = 0; i < grid.size(); ++i) {
        if (std::none_of(grid[i].begin(), grid[i].end(), [](char c) { return c == '#'; })) {
            emptyRows.push_back(i);
        }
    }
    return emptyRows;
}

std::vector<size_t> emptyColumns(const Grid& grid) {
    std::vector<size_t> emptyCols;

    size_t colCount = grid.empty() ? 0 : grid[0].size();
    for (size_t j = 0; j < colCount; ++j) {
        bool containsHash = false;
        for (size_t i = 0; i < grid.size(); ++i) {
            if (grid[i][j] == '#') {
                containsHash = true;
                break;
            }
        }
        if (!containsHash) {
            emptyCols.push_back(j);
        }
    }

    return emptyCols;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    Grid grid = readGridFromFile(filePath);

    std::vector<size_t> empty_rows = emptyRows(grid);
    std::vector<size_t> empty_columns = emptyColumns(grid);

    std::vector<Position> positions = findHashPositions(grid);
    std::vector<long long> distnaces = findAllPairwiseDistances(positions, empty_rows, empty_columns, 2);
    for (size_t i = 0; i < distnaces.size(); i++) {
        std::cout << "Distance " << i << ": " << distnaces[i] << std::endl;
    }

    //long long sum_of_distances = std::accumulate(distnaces.begin(), distnaces.end(), 0);
    
    //std::cout << "Sum of distances: " << sum_of_distances << std::endl;

    return 0;
}