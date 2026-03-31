#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MOVES 4  // Maximum number of moves to store (for 3x3 board)

// Struct to represent the Queue
typedef struct {
    int row[MAX_MOVES];
    int col[MAX_MOVES];
    int front;
    int rear;
} MoveQueue;

// Initialize the queue
void initQueue(MoveQueue *q) {
    q->front = 0;
    q->rear = 0;
}

// Enqueue a new move (FIFO principle)
void enqueue(MoveQueue *q, int row, int col) {
    // If the queue is full, move the front forward
    if ((q->rear + 1) % MAX_MOVES == q->front) {
        q->front = (q->front + 1) % MAX_MOVES;  // Remove the oldest element
    }

    // Add the new move to the queue
    q->row[q->rear] = row;
    q->col[q->rear] = col;
    q->rear = (q->rear + 1) % MAX_MOVES;  // Update the rear pointer
}

// Get the size of the board
int getsize() {
    int size;
    char input[10];
    printf("Enter the size of the board (3, 4, or 5): ");
    while (1) {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;  // Remove newline character
        size = atoi(input);
        if (size == 3 || size == 4 || size == 5) {
            break;
        } else {
            printf("Size should be 3, 4, or 5!\n");
        }
    }
    return size;  // Return the validated size
}

// Reset the board
void resetboard(char **board, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j] = ' ';
        }
    }
}

// Print the board
void printboard(char **board, int size) {
    printf("\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf(" %c ", board[i][j]);
            if (j < size - 1) {
                printf("|");
            }
        }
        printf("\n");
        if (i < size - 1) {
            for (int u = 0; u < size; u++) {
                printf("---");
                if (u < size - 1) {
                    printf("+");
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Check if the board is full
int isboardfull(char **board, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == ' ') {
                return 0; // Board is not full
            }
        }
    }
    return 1; // Board is full
}

// Check for a winner
char checkwinner(char **board, int size) {
    // Check rows
    for (int i = 0; i < size; i++) {
        int rowWin = 1;
        for (int j = 1; j < size; j++) {
            if (board[i][j] != board[i][0] || board[i][j] == ' ') {
                rowWin = 0;
                break;
            }
        }
        if (rowWin) {
            return board[i][0];
        }
    }

    // Check columns
    for (int j = 0; j < size; j++) {
        int colWin = 1;
        for (int i = 1; i < size; i++) {
            if (board[i][j] != board[0][j] || board[i][j] == ' ') {
                colWin = 0;
                break;
            }
        }
        if (colWin) {
            return board[0][j];
        }
    }

    // Check main diagonal
    int diag1Win = 1;
    for (int i = 1; i < size; i++) {
        if (board[i][i] != board[0][0] || board[i][i] == ' ') {
            diag1Win = 0;
            break;
        }
    }
    if (diag1Win) {
        return board[0][0];
    }

    // Check reverse diagonal
    int diag2Win = 1;
    for (int i = 1; i < size; i++) {
        if (board[i][size - i - 1] != board[0][size - 1] || board[i][size - i - 1] == ' ') {
            diag2Win = 0;
            break;
        }
    }
    if (diag2Win) {
        return board[0][size - 1];
    }

    return ' '; // No winner
}

// Make a move
void makemove(char **board, char playerSymbol, int size, int playerNumber, MoveQueue *q) {
    printf("Player %d (%c) move. Please enter row and column: ", playerNumber, playerSymbol);
    int row, column;
    while (1) {
        scanf("%d %d", &row, &column);

        if (row >= 0 && row < size && column >= 0 && column < size && board[row][column] == ' ') {
            // If the board is 3x3 and the queue is full, remove the oldest move
            if (size == 3 && (q->rear - q->front + MAX_MOVES) % MAX_MOVES == MAX_MOVES - 1) {
                // Get the oldest element from the queue
                int oldRow = q->row[q->front];
                int oldCol = q->col[q->front];
                board[oldRow][oldCol] = ' ';  // Remove the old move from the board
                q->front = (q->front + 1) % MAX_MOVES;  // Remove the old move from the queue
            }
            
            // Add the new move to the board and queue
            board[row][column] = playerSymbol;
            enqueue(q, row, column);  // Add the move to the queue
            break;
        } else {
            printf("Invalid move! Try again.\n");
        }
    }

    // Log the move
    FILE *file = fopen("game_log.txt", "a");
    time_t t;
    time(&t);
    fprintf(file, "Player %d (%c) --> (%d %d) at %s", playerNumber, playerSymbol, row, column, ctime(&t));
    fclose(file);
}

// Add timestamp to the log when the game starts or ends
void addTimestampToLog(const char* event) {
    FILE *file = fopen("game_log.txt", "a");
    time_t t;
    time(&t);
    fprintf(file, "%s at %s\n", event, ctime(&t));  // Add timestamp
    fclose(file);
}

int main() {
    int gamenumber = 1;
    FILE *file = fopen("game_log.txt", "w");  // Reset log file
    fclose(file);

    while (1) {
        int size = getsize();  // Get board size
        char **board = (char **)malloc(size * sizeof(char *));  // Allocate memory for the board
        for (int i = 0; i < size; i++) {
            board[i] = (char *)malloc(size * sizeof(char));
        }

        char pl1symbol, pl2symbol;

        resetboard(board, size);  // Reset the board
        printboard(board, size);  // Print the empty board

        file = fopen("game_log.txt", "a");
        fprintf(file, "        GAME %d\n", gamenumber);
        fclose(file);

        addTimestampToLog("Game Started");  // Add timestamp when game starts

        // Symbol selection
        while (1) {
            printf("Player 1, please select your symbol (X or O): ");
            scanf(" %c", &pl1symbol);
            if (pl1symbol == 'O') {
                pl2symbol = 'X';
                break;
            } else if (pl1symbol == 'X') {
                pl2symbol = 'O';
                break;
            } else {
                printf("Invalid symbol! You should select X or O.\n");
            }
        }

        MoveQueue q1, q2;
        initQueue(&q1);
        initQueue(&q2);

        int turn = 1; // Player 1 starts
        while (1) {
            if (turn) {
                makemove(board, pl1symbol, size, 1, &q1);
                char winner = checkwinner(board, size);
                printboard(board, size);
                if (winner == pl1symbol) {
                    printf("Congrats! Player 1 won.\n");
                    addTimestampToLog("Player 1 won the game");
                    break;
                }
                turn = 0;
            } else {
                makemove(board, pl2symbol, size, 2, &q2);
                char winner = checkwinner(board, size);
                printboard(board, size);
                if (winner == pl2symbol) {
                    printf("Congrats! Player 2 won.\n");
                    addTimestampToLog("Player 2 won the game");
                
                    break;
                }
                turn = 1;
            }

            if (isboardfull(board, size)) {
                printboard(board, size);
                printf("The game is a draw!\n");
                addTimestampToLog("Game ended in draw");
                break;
            }
        }

        printf("Do you want to play again? (yes/no): ");
        char choice[10];
        scanf("%s", choice);
        if (choice[0] == 'n' || choice[0] == 'N') {
            break;
        }
        gamenumber++;
        // Free memory
        for (int i = 0; i < size; i++) {
            free(board[i]);
        }
        free(board);
    }
    return 0;
}
