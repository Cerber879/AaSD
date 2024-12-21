#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <conio.h>
#include <random>
#include <windows.h>

using namespace std;

const char ONE = '1';
const char TWO = '2';
const char W = 'w';
const char A = 'a';
const char S = 's';
const char D = 'd';
const char O = 'o';
const char X = 'x';

const char GRID = '#';
const char DOLLAR = '$';
const char ZERO = '0';
const char WHITESPACE = ' ';

const string SCORE_TEXT = "Score: ";
const string WIN_TEXT = "You win";
const string GAMEOVER_TEXT = "Game Over";
const string PAUSE_TEXT = "Paused ";
const string PLAYING_TEXT = "Playing";
const string STATUS_TEXT = "Status: ";
const string MAIN_MENU_TITLE = "Snake Game";
const string START_OPTION = "1. Start";
const string EXIT_OPTION = "2. Exit";

const string RETURN_MAIN_MENU = "Press any key to return to the main menu...";

const enum class Direction { PLAING, PAUSE, GAME_OVER, WIN, LEFT, UP, RIGHT, DOWN };
const enum class Difficulty { EASY, NORMAL, HARD, IMPOSSIBLE };

Direction dir = Direction::PAUSE;
Difficulty diff = Difficulty::HARD;

const char heads[4] = { '<', 'A', '>', 'V' };

struct Snake {
    Snake* next = nullptr;
    Snake* prev = nullptr;
    int x = -1;
    int y = -1;
};
Snake* head = nullptr;
Snake* tail = nullptr;
int Score = 2;

int width = 20;
int height = 10;
int x;
int y;
int fruitx;
int fruity;
int speed = 50;

vector<vector<char>> field(height, vector<char>(width, WHITESPACE));
vector<pair<int, int>> obstacles;
int obstacleCount = 5;

// Инициализация функций
void InitializeRandomGenerator();
void InitializeField();
void GenerateFruit();
bool IsFruitOnObstacle();
bool IsSnakeOnFruit();
void GenerateObstacles();
bool IsSnakeOnObstacle(int x, int y);
bool IsObstacle(int x, int y);
void InputCmd();
void CheckDirection();
void CheckSubscriptOfRangeField();
void DelTail();
void UpdateSnakePosition();
void ProcessGameLogic();
void DrawField();
bool GameOver();
void ResetGame();
void DisplayGameStatus();
void DisplayMainMenu(); 
void RunGame();

// Инициализация генератора случайных чисел
void InitializeRandomGenerator() {
    srand(static_cast<unsigned int>(time(nullptr))); // Используем текущее время в качестве начального значения
}

// Функция инициализации игрового поля
void InitializeField() {
    x = width / 2;
    y = height / 2;

    field.assign(height, vector<char>(width, WHITESPACE));

    head = new Snake;
    head->x = x;
    head->y = y;

    tail = new Snake;
    tail->x = x - 1;
    tail->y = y;

    head->next = tail;
    tail->prev = head;

    if (diff == Difficulty::HARD or diff == Difficulty::IMPOSSIBLE) {
        GenerateObstacles();
    }
    GenerateFruit();
}

// Функция генерации новых координат для фрукта
void GenerateFruit() {
    do {
        fruitx = rand() % width;
        fruity = rand() % height;
    } while (IsSnakeOnFruit() or IsFruitOnObstacle());
}

// Функция проверки, находится ли фрукт на стенке
bool IsFruitOnObstacle() {
    if (diff == Difficulty::HARD or diff == Difficulty::IMPOSSIBLE) {
        for (int i = 0; i < obstacles.size(); i++) {
            if (obstacles[i].first == fruitx and obstacles[i].second == fruity) {
                return true;
            }
        }
    }
    return false;
}

// Функция проверки, находится ли змейка на фрукте
bool IsSnakeOnFruit() {
    Snake* curr = head;
    while (curr != nullptr) {
        if (curr->x == fruitx and curr->y == fruity) {
            return true;
        }
        curr = curr->next;
    }
    return false;
} 

// Функция для генерации преград
void GenerateObstacles() {
    for (int i = 0; i < obstacleCount; i++) {
        int obsX, obsY;
        do {
            obsX = rand() % width;
            obsY = rand() % height;
        } while (IsSnakeOnObstacle(obsX, obsY));
        obstacles.push_back({ obsX, obsY });
    }
}

// Функция для проверки, находится ли змейка на преграде
bool IsSnakeOnObstacle(int x, int y) {
    Snake* curr = head;
    while (curr != nullptr) {
        if (curr->x == x and curr->y == y) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

// Функция для проверки ячейки на преграду
bool IsObstacle(int x, int y) {
    for (int i = 0; i < obstacles.size(); i++) {
        if (obstacles[i].first == x and obstacles[i].second == y) {
            return true;
        }
    }
    return false;
}

// Функция обработки ввода команд
void InputCmd() {
    if (_kbhit()) {
        char ch = _getch();
        if (ch == A and dir == Direction::PAUSE and (head->next->x == head->x - 1 or head->next->x == width - 1)) {}
        else if (ch == D and dir == Direction::PAUSE and (head->next->x == head->x + 1 or head->next->x == 0)) {}
        else if (ch == W and dir == Direction::PAUSE and (head->next->y == head->y - 1 or head->next->y == height - 1)) {}
        else if (ch == S and dir == Direction::PAUSE and(head->next->y == head->y - 1 or head->next->y == 0)) {}
        else if ((ch == W && dir != Direction::DOWN) or
            (ch == A && dir != Direction::RIGHT) or
            (ch == S && dir != Direction::UP) or
            (ch == D && dir != Direction::LEFT)) {
            switch (ch) {
            case W:
                dir = Direction::UP;
                break;
            case A:
                dir = Direction::LEFT;
                break;
            case S:
                dir = Direction::DOWN;
                break;
            case D:
                dir = Direction::RIGHT;
                break;
            }
        }
        else if (ch == WHITESPACE) {
            if (dir == Direction::PAUSE) {
                if (head->next->x == head->x - 1 or head->next->x == width - 1) {
                    dir = Direction::RIGHT;
                }
                else if (head->next->x == head->x + 1 or head->next->x == 0) {
                    dir = Direction::LEFT;
                }
                else if (head->next->y == head->y - 1 or head->next->y == height - 1) {
                    dir = Direction::DOWN;
                }
                else if (head->next->y == head->y - 1 or head->next->y == 0) {
                    dir = Direction::UP;
                }
            }
            else
                dir = Direction::PAUSE;
        }
        else if (ch == X) {
            dir = Direction::GAME_OVER;
        }
    }
}

// Функция обновления направления движения змейки
void CheckDirection() {
    switch (dir) {
    case Direction::UP:
        y--;
        break;
    case Direction::LEFT:
        x--;
        break;
    case Direction::DOWN:
        y++;
        break;
    case Direction::RIGHT:
        x++;
        break;
    }
}

// Функция проверки выхода за поле
void CheckSubscriptOfRangeField() {
    if (diff != Difficulty::HARD and diff != Difficulty::IMPOSSIBLE) {
        if (x >= width)
            x = 0;
        else if (x < 0)
            x = width - 1;
        else if (y >= height)
            y = 0;
        else if (y < 0)
            y = height - 1;
    }
    else if (diff == Difficulty::HARD){
        if (x >= width or x < 0 or y >= height or y < 0)
            dir = Direction::GAME_OVER;
        for (int i = 0; i < obstacles.size(); i++) {
            if (obstacles[i].second == y and obstacles[i].first == x)
                dir = Direction::GAME_OVER;
        }
    }
    else {
        if ((x == width or IsObstacle(x, y)) and head->next->x == head->x - 1) {
            if (Score == 2) dir = Direction::GAME_OVER;
            else {
                if (!IsObstacle(x - 1, y - 1) and y != 0) {
                    x--;
                    y--;
                    dir = Direction::UP;
                }
                else if (!IsObstacle(x - 1, y + 1) and y != height - 1) {
                    x--;
                    y++;
                    dir = Direction::DOWN;
                }
                else dir = Direction::GAME_OVER;
                DelTail();
            }
        }
        else if ((x == -1 or IsObstacle(x, y)) and head->next->x == head->x + 1) {
            if (Score == 2) dir = Direction::GAME_OVER;
            else {
                if (!IsObstacle(x + 1, y + 1) and y != height - 1) {
                    x++;
                    y++;
                    dir = Direction::DOWN;
                }
                else if (!IsObstacle(x + 1, y - 1) and y != 0) {
                    x++;
                    y--;
                    dir = Direction::UP;
                }
                else dir = Direction::GAME_OVER;
                DelTail();
            }
        }
        else if ((y == height or IsObstacle(x, y)) and head->next->y == head->y - 1) {
            if (Score == 2) dir = Direction::GAME_OVER;
            else {
                if (!IsObstacle(x + 1, y - 1) and x != width - 1) {
                    y--;
                    x++;
                    dir = Direction::RIGHT;
                }
                else if (!IsObstacle(x - 1, y - 1) and x != 0) {
                    y--;
                    x--;
                    dir = Direction::LEFT;
                }
                else dir = Direction::GAME_OVER;
                DelTail();
            }
        }
        else if ((y == -1 or IsObstacle(x, y)) and head->next->y == head->y + 1) {
            if (Score == 2) dir = Direction::GAME_OVER;
            else {
                if (!IsObstacle(x - 1, y + 1) and x != 0) {
                    y++;
                    x--;
                    dir = Direction::LEFT;
                }
                else if (!IsObstacle(x + 1, y + 1) and x != width - 1) {
                    y++;
                    x++;
                    dir = Direction::RIGHT;
                }
                else dir = Direction::GAME_OVER;
                DelTail();
            }
        }
    }
}

// Удаление хвоста
void DelTail() {
    Snake* temp = tail;
    tail = tail->prev;
    tail->next = nullptr;
    delete temp;
    Score--;
}

// Функция обновления позиции змейки
void UpdateSnakePosition() {
    if (dir != Direction::PAUSE) {
        Snake* temp = head;
        CheckDirection();
        CheckSubscriptOfRangeField();

        int prevx = temp->x;
        int prevy = temp->y;
        temp->x = x;
        temp->y = y;
        temp = temp->next;
        int prev2x, prev2y;
        while (temp != nullptr) {
            prev2x = temp->x;
            prev2y = temp->y;
            temp->x = prevx;
            temp->y = prevy;
            prevx = prev2x;
            prevy = prev2y;
            temp = temp->next;
        }
    }
}

// Функция обработки игровой логики
void ProcessGameLogic() {
    UpdateSnakePosition();

    if (dir != Direction::PAUSE and diff != Difficulty::EASY) {
        Snake* curr = head->next;
        while (curr != nullptr) {
            if (curr->x == head->x && curr->y == head->y) {
                dir = Direction::GAME_OVER;
                return;
            }
            curr = curr->next;
        }
    }

    // Проверка, находится ли голова на фрукте
    if (head->x == fruitx and head->y == fruity) {
        GenerateFruit();
        Snake* newTail = new Snake;
        newTail->x = tail->x;
        newTail->y = tail->y;
        tail->next = newTail;
        newTail->prev = tail;
        tail = newTail;
        Score++;
    }

    if (Score == width * height) {
        dir = Direction::GAME_OVER;
    }
}

// Функция отрисовки игрового поля
void DrawField() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            field[i][j] = WHITESPACE;
        }
    }

    if (diff == Difficulty::HARD or diff == Difficulty::IMPOSSIBLE) {
        for (int i = 0; i < obstacles.size(); i++) {
            field[obstacles[i].second][obstacles[i].first] = GRID;
        }
    }
        
    char ch = ZERO;
    int index = static_cast<int>(dir);
    if (index == 7 or index == 4 or index == 5 or index == 6)
        ch = heads[index - 4];


    field[head->y][head->x] = ch;
    field[fruity][fruitx] = DOLLAR;
    Snake* curr = head->next;
    while (curr != nullptr) {
        field[curr->y][curr->x] = O;
        curr = curr->next;
    }

    /*system("cls");*/
    // Получение дескриптора стандартного вывода
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Структура для хранения координат курсора
    COORD position;
    position.X = 0; // Установка координаты по оси X в 0
    position.Y = 0; // Установка координаты по оси Y в 0

    // Перемещение курсора в указанную позицию
    SetConsoleCursorPosition(hConsole, position);
    for (int i = 0; i < width + 2; i++)
        cout << GRID;
    cout << endl;

    for (int i = 0; i < height; i++) {
        cout << GRID;
        for (int j = 0; j < width; j++) {
            cout << field[i][j];
        }
        cout << GRID << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << GRID;
    cout << endl;
}

// Функция для отслеживания состояния игры
bool GameOver() {
    return dir == Direction::GAME_OVER;
}

// Функция удаления змейки и сброса игры
void ResetGame() {
    while (head != nullptr) {
        Snake* temp = head;
        head = head->next;
        delete temp;
    }
    obstacles.clear();
    head = nullptr;
    tail = nullptr;
    dir = Direction::PAUSE;
    x = width / 2;
    y = height / 2;
    Score = 2;
}

// Отображение статуса игры
void DisplayGameStatus() {
    cout << SCORE_TEXT << Score - 2 << WHITESPACE;
    string statusText;
    if (dir == Direction::WIN) statusText = WIN_TEXT;
    if (dir == Direction::GAME_OVER) statusText = GAMEOVER_TEXT;
    else if (dir == Direction::PAUSE) statusText = PAUSE_TEXT;
    else statusText = PLAYING_TEXT;
    cout << STATUS_TEXT << statusText << endl;
}

// Меню
void DisplayMainMenu() {
    system("cls");
    cout << MAIN_MENU_TITLE << endl;
    cout << START_OPTION << endl;
    cout << EXIT_OPTION << endl;
}

// Основная функция запуска игры
void RunGame() {
    char option;
    do {
        DisplayMainMenu();
        option = _getch();
        switch (option) {
        case ONE: {
            InitializeRandomGenerator();
            InitializeField();
            while (!GameOver()) {
                DrawField();
                InputCmd();
                ProcessGameLogic();
                DisplayGameStatus();
                Sleep(speed);
            }
            ResetGame();
            cout << endl << RETURN_MAIN_MENU;
            _getch();
            break;
        }
        case 27:
            option = TWO;
            break;
        default:
            break;
        }
    } while (option != TWO and option != WHITESPACE and option != X);

}

int main() {
    setlocale(LC_ALL, "RU");
    RunGame();
    return 0;
}