#include <iostream>
#include <climits>
#include <cstring>
#include <vector>
#include <unordered_map>
using namespace std;

bool isSafe(int node, int color[], bool graph[101][101], int n, int col) {
    for (int k = 0; k < n; k++) {
        if (k != node && graph[k][node] == 1 && color[k] == col) {
            return false;
        }
    }
    return true;
}

bool forwardChecking(int node, int color[], bool graph[101][101], int n, vector<vector<int>>& remainingValues) {
    for (int i = 0; i < n; i++) {
        if (graph[node][i] && color[i] == 0) {
            for (int j = 0; j < remainingValues[node].size(); j++) {
                if (remainingValues[node][j] == 1) {
                    remainingValues[i][j] = 0; // Remove the color from the neighbor's remaining choices
                    if (remainingValues[i].empty()) {
                        return false; // No valid colors left for the neighbor
                    }
                }
            }
        }
    }
    return true;
}

int findMRV(int color[], bool assigned[], bool graph[101][101], int n, int m, vector<vector<int>>& remainingValues) {
    int minRemainingValues = INT_MAX;
    int minRemainingVariable = -1;

    for (int i = 0; i < n; i++) {
        if (!assigned[i] && color[i] == 0) {
            int remainingValuesCount = 0;
            for (int j = 0; j < m; j++) {
                remainingValuesCount += remainingValues[i][j];
            }

            if (remainingValuesCount < minRemainingValues) {
                minRemainingValues = remainingValuesCount;
                minRemainingVariable = i;
            }
        }
    }

    return minRemainingVariable;
}

bool solve(int node, int color[], int m, int N, bool graph[101][101], bool colorAssigned[], vector<vector<int>>& remainingValues,unordered_map<int,string> states,unordered_map<int,string> bor) {
    if (node == N) {
        // Print the solution
        for (int i = 0; i < N; i++) {
           cout << "Node " << states[i] << " colored with " << bor[color[i]]   << endl;
        }
        return true;
    }

    int nextVariable = findMRV(color, colorAssigned, graph, N, m, remainingValues);

    for (int i = 1; i <= m; i++) {
        if (isSafe(nextVariable, color, graph, N, i)) {
            color[nextVariable] = i;

            // Forward checking
            if (forwardChecking(nextVariable, color, graph, N, remainingValues)) {
                if (solve(node + 1, color, m, N, graph, colorAssigned, remainingValues,states,bor)) {
                    return true;
                }
            }

            // Backtrack: Undo the assignment and restore the original state
            color[nextVariable] = 0;
            for (int k = 0; k < N; k++) {
                if (graph[k][nextVariable] && color[k] != 0) {
                    forwardChecking(k, color, graph, N, remainingValues); // Reapply forward checking for already assigned nodes
                }
            }
        }
    }

    return false;
}

bool graphColoring(bool graph[101][101], int m, int N,unordered_map<int,string> states,unordered_map<int,string> bor) {
    int color[N] = {0};
    bool colorAssigned[N] = {false};
    vector<vector<int>> remainingValues(N, vector<int>(m, 1)); // Initialize all values as valid
    return solve(0, color, m, N, graph, colorAssigned, remainingValues,states,bor);
}

int main() {
    unordered_map<int,string> states;
    unordered_map<int,string> bor;
    states[0]="WA";
    states[1]="NT";
    states[2]="Q";
    states[3]="NSW";
    states[4]="SA";
    states[5]="V";
    states[6]="T";
    bor[1]="red";
    bor[2]="green";
    bor[3]="blue";
    int N = 7;
    int m = 3;

    bool graph[101][101] = {false};

    // Edges are (0, 1), (1, 2), (2, 3), (3, 0), (0, 2)
    graph[0][1] = 1; graph[1][0] = 1;
    graph[0][4] = 1; graph[4][0] = 1;
    graph[4][1] = 1; graph[1][4] = 1;
    graph[2][1] = 1; graph[1][2] = 1;
    graph[2][4] = 1; graph[4][2] = 1;
    graph[3][4] = 1; graph[4][3] = 1;
    graph[5][4] = 1; graph[4][5] = 1;
    graph[2][3] = 1; graph[3][2] = 1;
    graph[3][5] = 1; graph[5][3] = 1;


    if (!graphColoring(graph, m, N,states,bor)) {
        cout << "No solution found." << endl;
    }

    return 0;
}
