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

int PipeGrid::countInsideLoopTiles()
{
    bool insideLoop = false;
    int tilesEnclosedByLoop = 0;
    bool lookingForJ = false, lookingFor7 = false;

    for (int y = 0; y < gridHeight; ++y)
    {
        for (int x = 0; x < gridWidth; ++x)
        {
            if (visitedGrid[y][x])
            { // on loop
                switch (grid[y][x])
                {
                case '|':
                    insideLoop = !insideLoop;
                    break;
                case 'F':
                    lookingForJ = true;
                    break;
                case 'L':
                    lookingFor7 = true;
                    break;
                case 'J':
                    if (lookingForJ)
                    {
                        insideLoop = !insideLoop;
                        lookingForJ = false;
                    }
                    lookingFor7 = false;
                    break;
                case '7':
                    if (lookingFor7)
                    {
                        insideLoop = !insideLoop;
                        lookingFor7 = false;
                    }
                    lookingForJ = false;
                    break;
                default:
                    break;
                }
            }
            else
            { // not on loop
                if (insideLoop)
                {
                    tilesEnclosedByLoop++;
                    grid[y][x] = 'I';
                }
            }
        }
    }
    return tilesEnclosedByLoop;
}

void PipeGrid::replaceStartSymbol()
{
    auto [startY, startX] = findStartLocation();
    char newSymbol = ' ';

    // Check each direction and determine the correct symbol
    bool canMoveNorth = startY > 0 && isMoveValid(startY - 1, startX, South);
    bool canMoveSouth = startY < gridHeight - 1 && isMoveValid(startY + 1, startX, North);
    bool canMoveEast = startX < gridWidth - 1 && isMoveValid(startY, startX + 1, West);
    bool canMoveWest = startX > 0 && isMoveValid(startY, startX - 1, East);

    // Determine the new symbol based on possible movements
    if (canMoveNorth && canMoveSouth && !canMoveEast && !canMoveWest)
        newSymbol = '|';
    else if (canMoveNorth && !canMoveSouth && canMoveWest && !canMoveEast)
        newSymbol = 'J';
    else if (canMoveNorth && !canMoveSouth && !canMoveWest && canMoveEast)
        newSymbol = 'L';
    else if (!canMoveNorth && canMoveSouth && canMoveWest && !canMoveEast)
        newSymbol = '7';
    else if (!canMoveNorth && canMoveSouth && !canMoveWest && canMoveEast)
        newSymbol = 'F';
    else if (!canMoveNorth && !canMoveSouth && canMoveWest && canMoveEast)
        newSymbol = '-';
    else
        throw std::runtime_error("Incorrect number of connections to start");

    // Replace 'S' with the determined symbol
    grid[startY][startX] = newSymbol;
}
