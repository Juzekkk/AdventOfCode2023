#ifndef PIPEGRID_H
#define PIPEGRID_H

#include <vector>
#include <string>
#include "Directions.h"

class PipeGrid
{
public:
    PipeGrid(const std::vector<std::string> &inputGrid);
    int countVisitedPipes();
    int countInsideLoopTiles();
    void outputGridToConsole();

private:
    std::vector<std::string> grid;
    std::vector<std::vector<bool>> visitedGrid;
    int gridHeight, gridWidth;
    int startX, startY;

    void traverseLoop(int x, int y, int &visitedCount);
    void cleanUpGrid();
    void replaceStartSymbol();
    bool isPipeConnectable(char pipe, Direction dir) const;
    bool isMoveValid(int x, int y, Direction dir) const;
    std::pair<int, int> findStartLocation() const;
};

#endif // PIPEGRID_H
