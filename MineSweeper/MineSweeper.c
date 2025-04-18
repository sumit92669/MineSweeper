#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 5
#define MINES 5

char board[SIZE][SIZE];
char mines[SIZE][SIZE];

void initializeBoard();
void placeMines();
void displayBoard();
void revealCell(int row, int col);
int countAdjacentMines(int row, int col);
void gameOver();
int checkWin();
void playGame();

int main() {
    srand(time(NULL));
    initializeBoard();
    placeMines();
    playGame();
    return 0;
}

// Initializes board with hidden cells
void initializeBoard() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = '-';
            mines[i][j] = '0';
        }
    }
}

// Places random mines
void placeMines() {
    int placed = 0;
    while (placed < MINES) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        if (mines[row][col] != 'X') {
            mines[row][col] = 'X';
            placed++;
        }
    }
}

// Displays the board properly aligned
void displayBoard() {
    printf("\n   "); // Extra space to align the column numbers
    for (int col = 0; col < SIZE; col++) {
        printf(" %d ", col); // 1 space before and after the number
    }
    printf("\n  "); // Adjusted to align the horizontal line correctly

    // Print the top border
    for (int i = 0; i < SIZE * 3 + 1; i++) {
        printf("-");
    }
    printf("\n");

    // Print rows
    for (int row = 0; row < SIZE; row++) {
        printf("%d |", row); // Print row number with a side border
        for (int col = 0; col < SIZE; col++) {
            printf(" %c |", board[row][col]); // Ensure spacing inside the cells
        }
        printf("\n");
    }

    // Print bottom border
    printf("  ");
    for (int i = 0; i < SIZE * 3 + 1; i++) {
        printf("-");
    }
    printf("\n");
}

// Reveals a cell
void revealCell(int row, int col) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || board[row][col] != '-') 
        return;

    if (mines[row][col] == 'X') {
        displayBoard();
        gameOver();  // Call clean Game Over function
    } else {
        int adjacent = countAdjacentMines(row, col);
        board[row][col] = '0' + adjacent;
        if (adjacent == 0) {
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    revealCell(row + i, col + j);
                }
            }
        }
    }
}

// Counts adjacent mines
int countAdjacentMines(int row, int col) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int newRow = row + i;
            int newCol = col + j;
            if (newRow >= 0 && newRow < SIZE && newCol >= 0 && newCol < SIZE) {
                if (mines[newRow][newCol] == 'X') {
                    count++;
                }
            }
        }
    }
    return count;
}

// Checks if player won
int checkWin() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == '-' && mines[i][j] != 'X') {
                return 0; // Game is still on
            }
        }
    }
    return 1; // Win condition
}

// Handles losing the game
void gameOver() {
    printf("\n====================================\n");
    printf("           GAME OVER!              \n");
    printf("       You hit a mine! X           \n");
    printf("====================================\n\n");
    fflush(stdout);
    exit(0);
}

// Main game loop
void playGame() {
    int row, col;
    char action;

    while (1) {
        displayBoard();
        printf("\nEnter row (0-%d), column (0-%d), and action (r=Reveal, f=Flag): ", SIZE - 1, SIZE - 1);

        // Handle invalid input
        if (scanf("%d %d %c", &row, &col, &action) != 3) {
            printf("Invalid input! Please enter row, column, and action correctly.\n");
            while (getchar() != '\n');  // Clear input buffer
            continue;
        }

        // Validate row and column range
        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
            printf("Invalid row/column! Try again.\n");
            continue;
        }

        // Validate action
        if (action != 'r' && action != 'f') {
            printf("Invalid action! Use 'r' for Reveal or 'f' for Flag.\n");
            continue;
        }

        // Handle user action
        if (action == 'r') {
            revealCell(row, col);
        } else if (action == 'f') {
            // Toggle flagging
            if (board[row][col] == '-') {
                board[row][col] = 'F';
            } else if (board[row][col] == 'F') {
                board[row][col] = '-';
            } else {
                printf("Cannot flag a revealed cell!\n");
            }
        }

        // Check for win
        if (checkWin()) {
            displayBoard();
            printf("\nCongratulations! You won the game!\n");
            break;
        }
    }
}
