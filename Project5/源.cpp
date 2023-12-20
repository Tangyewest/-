#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define BOARD_SIZE 19
#define WIN_LENGTH 6
#define MAX_MOVES (BOARD_SIZE * BOARD_SIZE / 2) // 最大允许的步数

char board[BOARD_SIZE][BOARD_SIZE];

// 结构体表示落子位置
typedef struct {
    int row;
    int col;
} Move;

// 初始化棋盘
void initializeBoard() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = ' ';
        }
    }
}

// 打印棋盘
void printBoard() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

// 判断是否合法的落子位置
int isMoveValid(int row, int col) {
    return (row >= 0 && row < BOARD_SIZE&& col >= 0 && col < BOARD_SIZE&& board[row][col] == ' ');
}

// 判断是否游戏结束
int isGameOver(int lastRow, int lastCol, char player) {
    int count;

    // 检查横向
    count = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[lastRow][i] == player) {
            count++;
            if (count == WIN_LENGTH) return 1;
        }
        else {
            count = 0;
        }
    }

    // 检查纵向
    count = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[i][lastCol] == player) {
            count++;
            if (count == WIN_LENGTH) return 1;
        }
        else {
            count = 0;
        }
    }

    // 检查主对角线
    count = 0;
    for (int i = -WIN_LENGTH + 1; i < WIN_LENGTH; ++i) {
        int row = lastRow + i;
        int col = lastCol + i;
        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
            if (board[row][col] == player) {
                count++;
                if (count == WIN_LENGTH) return 1;
            }
            else {
                count = 0;
            }
        }
    }

    // 检查副对角线
    count = 0;
    for (int i = -WIN_LENGTH + 1; i < WIN_LENGTH; ++i) {
        int row = lastRow + i;
        int col = lastCol - i;
        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
            if (board[row][col] == player) {
                count++;
                if (count == WIN_LENGTH) return 1;
            }
            else {
                count = 0;
            }
        }
    }

    return 0;
}

// 获取所有合法的落子位置
int getValidMoves(Move validMoves[]) {
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (isMoveValid(i, j)) {
                validMoves[count].row = i;
                validMoves[count].col = j;
                count++;
            }
        }
    }
    return count;
}

// 计算估值函数
int evaluate() {
    // 这里可以根据具体情况设计一个更复杂的评估函数
    return 0;
}

// Mini-Max算法
int minimax(int depth, int isMaximizingPlayer, int alpha, int beta) {
    if (depth == 0 || isGameOver(0, 0, 'X') || isGameOver(0, 0, 'O')) {
        return evaluate();
    }

    Move validMoves[BOARD_SIZE * BOARD_SIZE];
    int numMoves = getValidMoves(validMoves);

    if (isMaximizingPlayer) {
        int maxEval = INT_MIN;
        for (int i = 0; i < numMoves; ++i) {
            int row = validMoves[i].row;
            int col = validMoves[i].col;
            board[row][col] = 'X';
            int eval = minimax(depth - 1, 0, alpha, beta);
            board[row][col] = ' ';
            maxEval = eval > maxEval ? eval : maxEval;
            alpha = eval > alpha ? eval : alpha;
            if (beta <= alpha) break;
        }
        return maxEval;
    }
    else {
        int minEval = INT_MAX;
        for (int i = 0; i < numMoves; ++i) {
            int row = validMoves[i].row;
            int col = validMoves[i].col;
            board[row][col] = 'O';
            int eval = minimax(depth - 1, 1, alpha, beta);
            board[row][col] = ' ';
            minEval = eval < minEval ? eval : minEval;
            beta = eval < beta ? eval : beta;
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

// Alpha-Beta剪枝算法
Move alphaBetaPruning() {
    Move bestMove;
    int maxEval = INT_MIN;
    int alpha = INT_MIN;
    int beta = INT_MAX;

    Move validMoves[BOARD_SIZE * BOARD_SIZE];
    int numMoves = getValidMoves(validMoves);

    for (int i = 0; i < numMoves; ++i) {
        int row = validMoves[i].row;
        int col = validMoves[i].col;
        board[row][col] = 'X';
        int eval = minimax(3, 0, alpha, beta); // 这里的3可以调整为更大的深度
        board[row][col] = ' ';

        if (eval > maxEval) {
            maxEval = eval;
            bestMove = validMoves[i];
        }

        alpha = eval > alpha ? eval : alpha;
        if (beta <= alpha) break;
    }

    return bestMove;
}

// 执行玩家的落子
void playerMove() {
    int row, col;

    do {
        printf("请输入你的落子位置 (行 列): ");
        scanf_s("%d %d", &row, &col);

        if (!isMoveValid(row, col)) {
            printf("无效的落子位置，请重试。\n");
        }
    } while (!isMoveValid(row, col));

    board[row][col] = 'O';
}

// 执行计算机的落子
void computerMove() {
    Move bestMove = alphaBetaPruning();
    board[bestMove.row][bestMove.col] = 'X';
}

// 游戏主循环
void playGame() {
    char currentPlayer = 'O';
    int moves = 0;

    while (moves < MAX_MOVES) {
        printBoard();

        if (currentPlayer == 'O') {
            playerMove();
        }
        else {
            computerMove();
        }

        if (isGameOver(0, 0, currentPlayer)) {
            printBoard();
            printf("%c方获胜！\n", currentPlayer);
            break;
        }

        currentPlayer = (currentPlayer == 'O') ? 'X' : 'O'; // 切换玩家

        moves++;
    }

    if (moves == MAX_MOVES) {
        printBoard();
        printf("和局！\n");
    }
}

int main() {
    srand(time(NULL)); // 初始化随机数种子

    initializeBoard();
    playGame();

    return 0;
}





