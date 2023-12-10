#include "PipeGrid.h"
#include <fstream>
#include <iostream>

bool PipeGrid::isPipeConnectable(char pipe, Direction dir) const
{
    switch (pipe)
    {
    case '|':
        return dir == 0 || dir == 2; // North or South
    case '-':
        return dir == 1 || dir == 3; // East or West
    case 'L':
        return dir == 0 || dir == 1; // North or East
    case 'J':
        return dir == 0 || dir == 3; // North or West
    case '7':
        return dir == 2 || dir == 3; // South or West
    case 'F':
        return dir == 2 || dir == 1; // South or East
    case 'S':
        return true;
    default:
        return false;
    }
}

bool PipeGrid::isMoveValid(int x, int y, Direction dir) const
{
    if (x < 0 || x >= gridHeight || y < 0 || y >= gridWidth || grid[x][y] == '.')
        return false;
    return isPipeConnectable(grid[x][y], dir);
}

void PipeGrid::depthFirstSearch(int x, int y, int &visitedCount)
{
    if (!visitedGrid[x][y])
    {
        visitedGrid[x][y] = true;
        visitedCount++;
        for (int i = 0; i < 4; ++i)
        {
            int newX = x + DX[i];
            int newY = y + DY[i];
            Direction fromDir = static_cast<Direction>((i + 2) % 4);
            if (isMoveValid(newX, newY, fromDir) && isMoveValid(x, y, static_cast<Direction>(i)))
            {
                depthFirstSearch(newX, newY, visitedCount);
            }
        }
    }
}

std::pair<int, int> PipeGrid::findStartLocation() const
{
    for (int i = 0; i < gridHeight; ++i)
    {
        for (int j = 0; j < gridWidth; ++j)
        {
            if (grid[i][j] == 'S')
            {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

PipeGrid::PipeGrid(const std::vector<std::string> &inputGrid) : grid(inputGrid)
{
    gridHeight = grid.size();
    gridWidth = grid.empty() ? 0 : grid[0].size();
    visitedGrid.assign(gridHeight, std::vector<bool>(gridWidth, false));
}

int PipeGrid::countVisitedPipes()
{
    auto [startX, startY] = findStartLocation();
    int visitedCount = 0;
    depthFirstSearch(startX, startY, visitedCount);
    return visitedCount;
}

void PipeGrid::outputGridToConsole()
{
    for (int i = 0; i < gridHeight; ++i)
    {
        for (int j = 0; j < gridWidth; ++j)
        {
            char cellValue = grid[i][j];
            std::cout << cellValue << " ";
        }
        std::cout << std::endl;
    }
}

void PipeGrid::cleanUpGrid()
{
    for (int i = 0; i < gridHeight; ++i)
    {
        for (int j = 0; j < gridWidth; ++j)
        {
            if (!visitedGrid[i][j] && grid[i][j] != '.' && grid[i][j] != 'S')
            {
                grid[i][j] = '.';
            }
        }
    }
}