#include <iostream>
using namespace std;

char board[3][3];

void initBoard() {
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            board[i][j] = ' ';
}

void displayBoard() {
    cout << "\n";
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            cout << " " << board[i][j] << " ";
            if(j<2) cout << "|";
        }
        cout << "\n";
        if(i<2) cout << "---|---|---\n";
    }
    cout << "\n";
}

bool checkWin(char player) {
    for(int i=0;i<3;i++)
        if(board[i][0]==player && board[i][1]==player && board[i][2]==player) return true;
    for(int j=0;j<3;j++)
        if(board[0][j]==player && board[1][j]==player && board[2][j]==player) return true;
    if(board[0][0]==player && board[1][1]==player && board[2][2]==player) return true;
    if(board[0][2]==player && board[1][1]==player && board[2][0]==player) return true;
    return false;
}

bool checkDraw() {
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            if(board[i][j]==' ') return false;
    return true;
}

void playGame() {
    char currentPlayer = 'X';
    int row, col;
    initBoard();
    displayBoard();

    while(true) {
        cout << "Player " << currentPlayer << ", enter row (0-2) and column (0-2): ";
        cin >> row >> col;
        if(row<0 || row>2 || col<0 || col>2 || board[row][col]!=' '){
            cout << "Invalid move. Try again.\n";
            continue;
        }
        board[row][col] = currentPlayer;
        displayBoard();

        if(checkWin(currentPlayer)){
            cout << "Player " << currentPlayer << " wins!\n";
            break;
        }

        if(checkDraw()){
            cout << "It's a draw!\n";
            break;
        }

        currentPlayer = (currentPlayer=='X') ? 'O' : 'X';
    }
}

int main() {
    char choice;
    do{
        playGame();
        cout << "Do you want to play again? (y/n): ";
        cin >> choice;
    } while(choice=='y' || choice=='Y');

    cout << "Thanks for playing!\n";
    return 0;
}
