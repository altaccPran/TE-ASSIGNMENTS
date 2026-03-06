#include<bits/stdc++.h>
using namespace std;

vector<vector<char>> board;
int N;

bool isSafe(int row, int col){
    // check column above
    for (int i = 0; i < row; i++){
        if (board[i][col] == 'Q') return false;
    }

    // check upper-left diagonal
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--){
        if (board[i][j] == 'Q') return false;
    }

    // check upper-right diagonal
    for (int i = row - 1, j = col + 1; i >= 0 && j < N; i--, j++){
        if (board[i][j] == 'Q') return false;
    }

    return true;
}

void printBoard(){
    cout << "\nFirst Solution:\n";

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++) cout << board[i][j] << " ";
        cout << endl;
    }
}

bool placeQueens(int row){
    if (row == N){
        printBoard();
        return true;
    }

    for (int col = 0; col < N; col++){
        if (isSafe(row, col)){
            board[row][col] = 'Q';
            if (placeQueens(row+1)) return true;
            board[row][col] = '.';  // backtrack
        }
    }

    return false;
}

int main(){
    int choice;
    while (true){
        cout << "\n===== N Queens Menu =====\n";
        cout << "1. Enter value of N\n";
        cout << "2. Show first solution\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (choice == 1){
            cout << "Enter N: ";
            cin >> N;
            board.resize(N, vector<char>(N, '.'));
        }
        else if (choice == 2){
            if (N <= 0){
                cout << "Please enter N first\n";
                continue;
            }

            // reset board
            board.assign(N, vector<char>(N, '.'));

            if (!placeQueens(0)) cout << "No solution exists\n";
        }
        else if (choice == 3){
            break;
        }
        else{
            cout << "Invalid choice\n";
        }
    }
    return 0;
}