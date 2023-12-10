#ifndef DIRECTIONS_H
#define DIRECTIONS_H

enum Direction
{
    North,
    East,
    South,
    West
};

const int DX[] = {-1, 0, 1, 0};
const int DY[] = {0, 1, 0, -1};

#endif // DIRECTIONS_H
