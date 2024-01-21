/**
*
* Solution to course project # 5
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Bozhidar Veselinov
* @idnumber 1MI0600393
* @compiler VC
*
* Minesweeper
*
*/


#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime> 

using namespace std;

char const EMPTY_CELL = '~';
char const HIDDEN_CELL = '?';
char const FLAG = 'F';
char const BOMB = 'B';

const vector<string> ACTIONS = { "open", "mark", "unmark" };


bool isValid(int n, int x, int y) {
    return (0 <= x && x < n) && (0 <= y && y < n);
}

bool isVisited(const vector<vector<bool>> visited, int x, int y) {
    
    return visited[x][y];
}

bool isMarked(const vector<vector<bool>> marked, int x, int y) {
    return marked[x][y];
}

bool isBomb(const vector<vector<int>> field, int x, int y) {
    
    return field[x][y] == -1;
}

bool isActionValid(string action) {
    for (string act : ACTIONS) {
        if (act == action) return true;
    }
    cout << "Wrong command\n";
    return false;

}
char calculateFunalFieldSymbolToPrint(const vector<vector<int>>& field,
                                      int i, int j, int currEl,
                                      const vector<vector<bool>>& marked,
                                      const vector<vector<bool>>& visited,
                                      bool hasWon)
{
    if (hasWon)
    {
        if (currEl == 0) return EMPTY_CELL;

        if (field[i][j] == -1) return FLAG;

        return currEl + '0';
    }

    if (isMarked(marked, i, j)) return FLAG;

    if (field[i][j] == -1) return BOMB;

    if (!isVisited(visited, i, j)) return HIDDEN_CELL;

    if (currEl == 0) return EMPTY_CELL;


    return currEl + '0';
}

void printFieldYCoordinatesNumber(int fieldSize) {

    std::cout << '\n';
    std::cout << '\n';

    for (int i = 0; i < fieldSize; i++)
    {
        cout << " " << i << " ";
    }
    std::cout << '\n';

    for (int i = 0; i < fieldSize * 3; i++)
    {
        cout << "_";
    }
    std::cout << '\n';
}


void printFinalField(const vector<vector<int>>& field,
                     const vector<vector<bool>>& marked,
                     const vector<vector<bool>>& visited,
                     bool hasWon) {

    int fieldSize = field.size();

    printFieldYCoordinatesNumber(fieldSize);

    for (size_t i = 0; i < fieldSize; i++)
    {
        for (size_t j = 0; j < fieldSize; j++)
        {
            int currEl = field[i][j];

            char currSympol = calculateFunalFieldSymbolToPrint(field, i, j, currEl, marked, visited, hasWon);

            cout << ' ' << currSympol << ' ';
        }
        cout << " | " << i << '\n';
    }
}

char calculateSymbolToPrint(const vector<vector<int>>& field,
                            int i, int j, int currEl,
                            const vector<vector<bool>>& marked,
                            const vector<vector<bool>>& visited) 
{

    if (isMarked(marked, i, j)) return FLAG;

    if (!isVisited(visited, i, j)) return HIDDEN_CELL;

    if (currEl == 0) return EMPTY_CELL;

    if (field[i][j] == -1) return BOMB;

    return currEl + '0';
}

void printField(const vector<vector<int>>& field, 
                const vector<vector<bool>>& marked,
                const vector<vector<bool>>& visited) 
{

    int fieldSize = field.size();
    
    printFieldYCoordinatesNumber(fieldSize);

    for (size_t i = 0; i < fieldSize; i++)
    {
        for (size_t j = 0; j < fieldSize; j++)
        {
            int currEl = field[i][j];

            char currSympol = calculateSymbolToPrint(field, i,j,currEl, marked, visited);

            cout << ' ' << currSympol << ' ';
        }
        cout<<" | " << i << '\n';
    }
}


void markCell(const vector<vector<int>>& field, int x, int y, int& markedBombs, int& flagCount,
                    vector<vector<bool>>& marked,
              const vector<vector<bool>>& visited) {
    int n = field.size();

    if (!isValid(n, x, y))return;
    if (isVisited(visited, x, y)) {
        cout << "You cannot mark cells that are revieled\n";
        return;
    }

    marked[x][y] = true;
    flagCount--;

    if (field[x][y]==-1)
    {
        markedBombs++;
    }
}

void unmarkCell(const vector<vector<int>> field, int x, int y, int& markedBombs, int& flagCount, 
                      vector<vector<bool>>& marked) {
    int n = field.size();
    if (!isValid(n, x, y)|| !isMarked(marked, x,y))return;

    marked[x][y] = false;
    flagCount++;

    if (isBomb(field, x, y))
    {
        markedBombs--;
    }
}


void openSurroundinCells(vector<vector<int>> field, int x, int y, 
                         vector<vector<bool>>& visited, int& openedCellsCount) {
    int n = field.size();
    if (!isValid(n, x, y))
    {
        return;
    }
    if (isVisited(visited, x, y))
    {
        return;
    }
    if (field[x][y] == -1)
    {
        return;
    }
    visited[x][y] = true;
    openedCellsCount++;

    if (field[x][y] == 0)
    {
        openSurroundinCells(field, x, y + 1, visited, openedCellsCount);
        openSurroundinCells(field, x, y - 1, visited, openedCellsCount);

        openSurroundinCells(field, x + 1, y, visited, openedCellsCount);
        openSurroundinCells(field, x - 1, y, visited, openedCellsCount);

        openSurroundinCells(field, x + 1, y + 1, visited, openedCellsCount);
        openSurroundinCells(field, x - 1, y - 1, visited, openedCellsCount);
        openSurroundinCells(field, x + 1, y - 1, visited, openedCellsCount);
        openSurroundinCells(field, x - 1, y + 1, visited, openedCellsCount);
    }

}

void openCell(      vector<vector<int>>& field, int x, int y,
                    vector<vector<bool>>& visited, int& openedCellsCount,
              const vector<vector<bool>>& marked) {
    int n = field.size();
    if (!isValid(n, x, y))
    {
        cout << "invalid";
        return;
    }
    if (isMarked(marked,x,y))
    {
        cout<<"This cell is marked! You must ummark it in order to open it";
        return;
    }
    if (isVisited(visited, x, y))
    {
        cout << "This cell has already been opened";
        return;
    }


    if (field[x][y] == 0)
    {
        openSurroundinCells(field, x, y, visited, openedCellsCount);
    }
    if (field[x][y]>=1)
    {
        visited[x][y] = true;
        openedCellsCount++;
    }
    

}
bool isNumber(int x, int y) {
    return true;
}

bool checkCoordinatInput(int n, int x, int y) {
    
    if ( !isValid(n, x, y))
    {
        cout << "Invalid coordinates\n";
        return false;
    }
    return true;

    
}

bool readCommand(string &action, int& x, int& y, int n) {
   
    do {
        cout << "Enter a command: ";
        cin >> action >> x >> y;
        cout << "\n";
        cin.clear();
        cin.ignore();

    } while (!isActionValid(action) || !checkCoordinatInput(n, x, y));
   
    return true;


}

void executeCommand(vector<vector<int>>& field ,string action, int x, int y,
                    vector<vector<bool>>& marked, int& markedBombs,int& flagCount, int& openedCellsCount,
                    vector<vector<bool>>& visited) {
    if (action=="mark")
    {
        markCell(field, x, y, markedBombs, flagCount, marked, visited);
        return;
    }

    if (action =="unmark")
    {
        unmarkCell(field, x, y, markedBombs, flagCount, marked);
        return;
    }
    openCell(field, x, y, visited, openedCellsCount, marked);
}



bool isOver(const vector<vector<int >>& field, 
                  vector<vector<bool>>& visited,
            const vector<vector<bool>>& marked,
            int x, int y, bool& hasWon, string currAction,
            int bombsCount, int flagCount, int openedCellsCount, int markedBombs)
{
    int allCells = field.size() * field.size();
    
    if (markedBombs == bombsCount && allCells - openedCellsCount == bombsCount && flagCount==0)
    {
        hasWon = true;
        return true;
    }
    if (field[x][y]==-1 && currAction=="open" && marked[x][y]==false)
    {
        visited[x][y] = true;

        hasWon = false;
        return true;
    }
    return false;
}



void updateCell(vector<vector<int>>& field, int x, int y) {
    int n = field.size();
    if (!isValid(n, x, y) || isBomb(field, x, y)) return;

    field[x][y] += 1;
}

void updateSurroundingCells(vector<vector<int>>& field, int x, int y) {

    updateCell(field, x, y + 1);
    updateCell(field, x, y - 1);

    updateCell(field, x + 1, y);
    updateCell(field, x - 1, y);

    updateCell(field, x + 1, y + 1);
    updateCell(field, x - 1, y - 1);
    updateCell(field, x + 1, y - 1);
    updateCell(field, x - 1, y + 1);

}

int generateRandomNumber(int n) {

    return rand() % n;
}

void generateBombs(vector<vector<int>>& field,int bombCount, int n) {

    srand((time(0)));
    for (int i = 0; i < bombCount; i++)
    {
        int x = generateRandomNumber(n);
        int y = generateRandomNumber(n);


        if (isBomb(field, x, y))
        {
            i--;
            continue;
        }
        field[x][y] = -1;

        updateSurroundingCells(field, x, y);
    }

}


vector<vector<int>> createField(int n) {

    vector<vector<int>> field(n, vector<int>(n, 0));
    return field;
}

vector<vector<bool>> createBoolMatrix(int n) {

    vector<vector<bool>> matrix(n, vector<bool>(n, false));
    return matrix;
}


void initializeGameField(vector<vector<int>>& field ,int n, int bombsCount) {
    
    field = createField(n);
    generateBombs(field, bombsCount, n);
}

void readInput(int& n, int& bombsCount) {

    cout << "\nEnter the size of the field[number between 3 and 10]: ";
    cin >> n;
    cin.clear();
    cin.ignore();

    while (!(n >= 3 && n <= 10))
    {
        cout << "\nIncorect input!\nEnter the size of the field[number between 3 and 10]: ";
        cin >> n;
        cin.clear();
        cin.ignore();
    }

    cout << "\nEnter the number of bombs[number between 1 and " << n * 3 << "]: ";
    cin >> bombsCount;
    cin.clear();
    cin.ignore();

    while (!(bombsCount >= 1 && bombsCount <= 3 * n))
    {
        cout << "\nIncorect input!\nEnter the number of bombs[number between 1 and " << n * 3 << "]: ";
        cin >> bombsCount;
        cin.clear();
        cin.clear();
        cin.ignore();

    }
}



void playMineSweeper() {
    int n = 0;
    int bombsCount = 0;
    int markedBombs = 0;

    readInput(n, bombsCount);
    vector<vector<int>> field; 
    initializeGameField(field, n, bombsCount);


    int x; 
    int y;
    string action;
    bool hasWon = false;
    int flagCount = bombsCount;
    int openedCellsCount = 0;

    vector<vector<bool>> visited = createBoolMatrix(n);
    vector<vector<bool>> marked = createBoolMatrix(n);

    printField(field, marked, visited);


    while (true)
    {
        readCommand(action, x, y, n);

        executeCommand(field, action, x, y, marked, markedBombs, flagCount, openedCellsCount, visited);

        if (isOver(field, visited, marked, x, y,hasWon, action, bombsCount, flagCount, openedCellsCount, markedBombs))
        {
            cout << "The game is over";
            break;
        }
        printField(field, marked, visited);


    }
    printFinalField(field, marked, visited, hasWon);
    if (hasWon)
    {
        cout << "You win";
        return;
    }
    cout << "You lose";
}


int main()
{
    playMineSweeper();
}
