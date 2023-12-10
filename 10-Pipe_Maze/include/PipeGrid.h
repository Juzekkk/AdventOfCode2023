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
    void outputGridToConsole();
    void cleanUpGrid();
    int countInsideLoopTiles();
    void replaceStartSymbol();

private:
    std::vector<std::string> grid;
    std::vector<std::vector<bool>> visitedGrid;
    int gridHeight, gridWidth;

    bool isPipeConnectable(char pipe, Direction dir) const;
    bool isMoveValid(int x, int y, Direction dir) const;
    void depthFirstSearch(int x, int y, int &visitedCount);
    std::pair<int, int> findStartLocation() const;
};

#endif // PIPEGRID_H
