#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using namespace std;

vector<string> grid;
vector<vector<int>> distanceGrid;
int n, m; // grid dimensions

// Directions: North, East, South, West
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

bool canConnect(char pipe, int dir)
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

bool isValid(int x, int y, int fromDir)
{
    if (x < 0 || x >= n || y < 0 || y >= m || grid[x][y] == '.')
        return false;

    return canConnect(grid[x][y], fromDir);
}

void dfs(int x, int y, int steps, int fromDir)
{
    if (steps < distanceGrid[x][y])
    {
        distanceGrid[x][y] = steps;

        for (int i = 0; i < 4; ++i)
        {
            int newX = x + dx[i];
            int newY = y + dy[i];
            int toDir = (i + 2) % 4; // Opposite direction

            if (isValid(newX, newY, toDir))
            {
                dfs(newX, newY, steps + 1, i);
            }
        }
    }
}

int findLongestPath()
{
    // Locate the starting position 'S'
    int startX = -1, startY = -1;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (grid[i][j] == 'S')
            {
                startX = i;
                startY = j;
                break;
            }
        }
        if (startX != -1)
            break;
    }

    if (startX == -1)
        return -1; // Starting position not found

    distanceGrid.assign(n, vector<int>(m, INT_MAX));
    dfs(startX, startY, 0, -1);

    int maxDist = 0;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (distanceGrid[i][j] != INT_MAX)
            {
                maxDist = max(maxDist, distanceGrid[i][j]);
            }
        }
    }

    return maxDist;
}

void outputToHtml(const string &filename)
{
    ofstream htmlFile(filename);
    if (!htmlFile.is_open())
    {
        cerr << "Error: Could not open the HTML file for writing." << endl;
        return;
    }

    // HTML header
    htmlFile << "<!DOCTYPE html><html><head><style>"
             << "table {border-collapse: collapse;}"
             << "td {width: 30px; height: 30px; text-align: center; color: white;}"
             << "</style></head><body><table>";

    // Find max distance for scaling colors
    int maxDist = 0;
    int maxI = 0, maxJ = 0;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (distanceGrid[i][j] != INT_MAX && distanceGrid[i][j] > maxDist)
            {
                maxDist = distanceGrid[i][j];
                maxI = i;
                maxJ = j;
            }
        }
    }

    for (int i = 0; i < n; ++i)
    {
        htmlFile << "<tr>";
        for (int j = 0; j < m; ++j)
        {
            if (grid[i][j] == 'S')
            {
                htmlFile << "<td style='background-color: green; border: 1px solid black;'>S</td>";
            }
            else if (i == maxI && j == maxJ)
            {
                htmlFile << "<td style='background-color: red; border: 1px solid black;'>" << distanceGrid[i][j] << "</td>";
            }
            else if (distanceGrid[i][j] != INT_MAX)
            {
                int intensity = static_cast<int>((1.0 - (double)distanceGrid[i][j] / maxDist) * 255);
                htmlFile << "<td style='background-color: rgb(0, 0, "
                         << intensity << "); border: 1px solid black;'>" << distanceGrid[i][j] << "</td>";
            }
            else
            {
                htmlFile << "<td style='color: black;'>.</td>";
            }
        }
        htmlFile << "</tr>";
    }

    // HTML footer
    htmlFile << "</table></body></html>";
    htmlFile.close();
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <input_file_path>" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        cerr << "Error: Could not open the input file." << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line))
    {
        grid.push_back(line);
    }
    inputFile.close();

    n = grid.size();
    m = grid[0].size();

    // Find the longest path in the loop
    int farthestDistance = findLongestPath();
    cout << "Farthest distance from start: " << farthestDistance << endl;

    outputToHtml("output.html");

    return 0;
}
