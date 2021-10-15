#include <iostream>
#include <math.h>
#include <stack>
#include <vector>
#include <array>
#include <assert.h>
#include <algorithm>
#define pii(a, b) std::pair<int, int>(a, b)

void displayBoard(std::array<std::pair<int, int>, 20> position) {
    std::cout << std::endl;
    for (int j = 0; j < 20; j++) {
        if (j % 4 == 0) 
            std::cout << std::endl;
        if (position[j].second >= 0)
            std::cout << "[" << position[j].first << "  " << position[j].second << "] ";
        else
            std::cout << "[" << position[j].first << " " << position[j].second << "] ";
    }
    std::cout << std::endl;
}

bool possible(std::array<std::pair<int, int>, 20> board, int s, int e) {
    // sx sy is starting position, ex ey is the end position
    // Ensure that the spot is available and the moving block is not empty
    if (board[e].first != 0 || board[s].first == 0) 
        return false;
    int d = e-s;
    if (abs(d) != 1 && abs(d) != 4) // Ensure adjacency
        return false;
    if (board[s].second == 0)
        return true;
    int curr = s+board[s].second;
    int a = abs(d) == 4 ? 1 : 4;
    while (curr != s) {
        if (abs(curr-s) == a && board[curr+d].first != 0)
            return false;
        curr += board[curr].second;
    }
    return true;
}

bool equal(std::array<std::pair<int, int>, 20> a, std::array<std::pair<int, int>, 20> b) {
    for (int i = 0; i < 20; i++) {
        if (a[i].first != b[i].first || a[i].second != b[i].second)
            return false;
    }
    return true;
}

std::vector<std::array<std::pair<int, int>, 20>> solve(std::array<std::pair<int, int>, 20> board) {
    // Store a list of possible states that can be referenced
    std::vector<std::array<std::pair<int, int>, 20>> stateArchive;
    // toCheck implements a DFS, root provides links between states for a backwards search, frees stores empty squares 
    std::vector<int> root, frees(2), explored;
    std::stack<int> toCheck;
    // Stores a state to be compared with other states, and added to the state archive if necessary
    std::array<std::pair<int, int>, 20> nextPos;

    stateArchive.push_back(board);
    toCheck.push(0);
    explored.push_back(0);
    root.push_back(-1);
    int adj[4] {1, -1, 4, -4}; // If board[j] == board[i+k] where k is in adj, then i is adjacent to j
    // Current stores the current state index, curr is a temporary variable for in-state searches
    // s is the start position for any move, trace is used to make a backwards search
    int current, curr, s, trace, iter, posIndex, t; 
    iter = 0;
    bool repeated, localRepeat;
    do {
        // std::cout << "Iteration " << iter << std::endl;
        iter++;
        current = toCheck.top(); 
        toCheck.pop();
        frees.clear();
        for (int i = 0; i < 20; i++) {
            if (stateArchive[current][i].first == 0)
                frees.push_back(i);
        }
        assert(("Frees size is not 2", frees.size() == 2));
        for (int e : frees) { 
            for (int j = 0; j < 4; j++) {
                s = e+adj[j];
                if (s < 20 && s >= 0 && possible(stateArchive[current], s, e)) {
                    // std::cout << "Found valid move " << s << " to " << e << std::endl;
                    // Finding the updated board 
                    nextPos = stateArchive[current];
                    if (nextPos[s] == pii(2, adj[j])) {
                        for (int i = 0; i < 3; i++) {
                            s += nextPos[s].second; 
                        }
                    }
                    curr = s;
                    do {
                        nextPos[curr-adj[j]] = nextPos[curr];
                        t = curr;
                        curr += stateArchive[current][curr].second;
                        nextPos[t] = pii(0, 0); 
                    } while (curr != s);
                    // std::cout << "Found resultant state" << std::endl;

                    // Adding the state to the index if it hasn't been checked yet
                    repeated = false;
                    for (int i = 0; i < stateArchive.size(); i++) {
                        if (equal(stateArchive[i], nextPos)) {
                            repeated = true;
                            break;
                        }
                    }
                    if (!repeated) {
                        stateArchive.push_back(nextPos);
                        root.push_back(current); // The root of the new state is this node
                        toCheck.push(stateArchive.size()-1);
                        // std::cout << "Added state to toCheck: ";
                        // displayBoard(nextPos);
                    }
                }
            }
        }
    } while (stateArchive[current][1].first != 2 || stateArchive[current][2].first != 2);

    std::cout << "Reached success position, now backtracking..." << std::endl;
    std::vector<std::array<std::pair<int, int>, 20>> path;
    while (current != 0) {
        path.push_back(stateArchive[current]);
        current = root[current];
    }
    std::reverse(path.begin(), path.end());
    // std::cout << "Returning path" << std::endl;
    return path;
}

int main() { 
    // The value at (x, y) is board[:][4*y+x]
    // board[0] represents existence
    // board[1] represents the necessary shift to reach a block's connection.
    std::array<std::pair<int, int>, 20> board = {
        pii(1, 0), pii(0, 0), pii(0, 0), pii(1, 0),
        pii(1, 4), pii(1, 0), pii(1, 0), pii(1, 4),
        pii(1,-4), pii(1, 1), pii(1,-1), pii(1,-4),
        pii(1, 4), pii(2, 1), pii(2, 4), pii(1, 4),
        pii(1,-4), pii(2,-4), pii(2,-1), pii(1,-4)
    };

    std::vector<std::array<std::pair<int, int>, 20>> path = solve(board);
    for (auto i : path) {
        displayBoard(i);
    }
    return 0;
}