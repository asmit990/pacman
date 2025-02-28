#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <thread>

using namespace std;

const int GRID_W = 28;
const int GRID_H = 9;
int points = 0;
bool isOver = false;

vector<string> layout = {
    "############################",
    "#         #    .     #     #",
    "# ####### # #######  # ### #",
    "# #     # #       #  # # . #",
    "# # ### # ####### #### ### #",
    "# #   #       #      #   # #",
    "# ###################### # #",
    "#            .        X    #",
    "############################"
};

struct Node {
    int px, py;
    Node* nxt;
};

int playerX = 1, playerY = 1;
int enemyX = 24, enemyY = 7;

void renderGame() {
    system("cls");
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            if (x == playerX && y == playerY) {
                cout << "🟡"; // Player
            } else if (x == enemyX && y == enemyY) {
                cout << "👻"; // Enemy
            } else {
                cout << layout[y][x];
            }
        }
        cout << endl;
    }
    cout << "Score: " << points << endl;
}

void movePlayer(char key) {
    int newX = playerX, newY = playerY;
    if (key == 'w') newY--;
    else if (key == 's') newY++;
    else if (key == 'a') newX--;
    else if (key == 'd') newX++;
    
    if (layout[newY][newX] != '#') {
        if (layout[newY][newX] == '.') {
            points += 10;
            layout[newY][newX] = ' ';
        }
        playerX = newX;
        playerY = newY;
    }
    renderGame();
}

void moveEnemy() {
    int pathGrid[GRID_H][GRID_W] = {0};
    Node* start = new Node{enemyX, enemyY, nullptr};
    Node* end = start;
    pathGrid[enemyY][enemyX] = 1;

    int moves[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    while (start) {
        if (start->px == playerX && start->py == playerY) {
            enemyX = start->px;
            enemyY = start->py;
            delete start;
            break;
        }
        for (int i = 0; i < 4; i++) {
            int nx = start->px + moves[i][0];
            int ny = start->py + moves[i][1];
            if (layout[ny][nx] != '#' && !pathGrid[ny][nx]) {
                pathGrid[ny][nx] = 1;
                Node* nextPos = new Node{nx, ny, nullptr};
                end->nxt = nextPos;
                end = nextPos;
            }
        }
        Node* temp = start;
        start = start->nxt;
        delete temp;
    }
}

void gameCycle() {
    while (!isOver) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'q') {
                isOver = true;
                break;
            }
            movePlayer(key);
        }
        moveEnemy();
        renderGame();
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

int main() {
    renderGame();
    thread cycle(gameCycle);
    cycle.join();
    cout << "Game Over! Final Score: " << points << endl;
    return 0;
}
