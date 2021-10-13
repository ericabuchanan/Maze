// Define all basic maze functions
#include "Maze.h"
extern ALLEGRO_DISPLAY *display;

// Basic constructor
Maze::Maze(){
    // Initialize variables
    mazeFileName = "maze1.txt";
    startX = -1;
    startY = -1;
    goalX = -1;
    goalY = -1;
}

// Updated constructor
Maze::Maze(string fileName){
    // Initialize fileName using user input
    mazeFileName = fileName;
    // Initialize other variables
    startX = -1;
    startY = -1;
    goalX = -1;
    goalY = -1;
}

// Deconstructor
Maze::~Maze(){
    // Destroy allegro display here
    al_destroy_display(display);
}

// Read maze from textfile
bool Maze::read_maze(){
    // Open file
    ifstream inFile(mazeFileName);
    // Check if it is open
    if (!inFile.is_open()){
        return false;
    // If it is open, read the data into maze
    }else{
        string data;
        int cols = 0;
        int rows = 0;
        bool firstRow = true;
        while(getline(inFile, data)){
            rows++;
            if (firstRow){
                cols = data.length();
            }
            maze.resize(rows, cols);
            for (int i = 0; i < data.length(); i++){
                maze[rows-1][i] = data[i];
            }
            firstRow = false;
        }
        // Close file
        inFile.close();
        // Initialize temporary maze variable
        tempMaze = maze;
        return true;
    }
}

// Find and set coordinates for start of maze by iterating through all coordinates
void Maze::find_start(){
    for (int i = 0; i < maze.numrows(); i++){
        for (int j = 0; j < maze.numcols(); j++){
            if (maze[i][j] == 'S'){
                startY = i;
                startX = j;
            }
        }
    }
}

// Find path using DFS algorthm
bool Maze::find_path(int x, int y){
    // Check if coordinates are in the matrix, if not return false
    if (x < 0 || x > maze.numcols()-1 || y < 0 || y > maze.numrows()-1){
        return false;
    }
    bool noExit = false;
    display_maze();
    // Check coordinate
    switch (maze[y][x]){
        // If start coordinate or open coordinate, check the ones around it
        case 'S':
            maze[y][x] = 'S';
        case '.':
            if (maze[y][x] != 'S'){
                maze[y][x] = '+';
            }
            // Check north direction, if not open set noExit to true
            if (find_path(x, y-1)){
                return true;
            }else{
                noExit = true;
            }
            // Check east direction, if not open set noExit to true
            if (find_path(x+1, y)){
                return true;
            }else{
                noExit = true;
            }
            // Check south direction, if not open set noExit to true
            if (find_path(x, y+1)){
                return true;
            }else{
                noExit = true;
            }
            // Check west direction, if not open set noExit to true
            if (find_path(x-1, y)){
                return true;
            }else{
                noExit = true;
            }
            // If none of the directions are open, mark path as wrong path
            if (noExit){
                if (maze[y][x] != 'S'){
                    maze[y][x] = '-';
                }
                return false;
            }
            break;
        // If goal, return true
        case 'G':
            return true;
            break;
        // Return false if hit wall, coordinate is wrong or coordinate is already filled
        case '-':
        case '#':
        case '+':
            return false;
            break;
        default:
            // All characters other than period, minus, plus, S, or G will be considered wall characters
            return false;
    }
}

// Return x coordinate of start
int Maze::getStartX(){
    return startX;
}
// Return y coordinate of start
int Maze::getStartY(){
    return startY;
}
// Return number of rows in maze
int Maze::getRows(){
    return maze.numrows();
}

// Create corresponding numbered maze from original maze
void Maze::load_dijkstra_maze(){
    int open = (maze.numrows()*maze.numcols());
    dijkstraMaze.resize(maze.numrows(), maze.numcols());
    for (int i = 0; i < maze.numrows(); i++){
        for (int j = 0; j < maze.numcols(); j++){
            if (maze[i][j] == 'S'){
                dijkstraMaze[i][j] = START;
            }else if (maze[i][j] == 'G'){
                dijkstraMaze[i][j] = GOAL;
            }else if (maze[i][j] == '.'){
                dijkstraMaze[i][j] = open;
            }else{
                dijkstraMaze[i][j] = WALL;
            }
        }
    }
}

// Finds path using dijkstra BFS algorithm, m represents the previous coordinate
bool Maze::dijkstra_path(int x, int y, int m){
    // Check if coordinates are in the matrix, if not return false
    if (x < 0 || x > dijkstraMaze.numcols()-1 || y < 0 || y > dijkstraMaze.numrows()-1){
        return false;
    }
    // Check if coordinate is one plus the previous coordinate
    if (dijkstraMaze[y][x] > (m+1) || dijkstraMaze[y][x] == START){
        if (dijkstraMaze[y][x] != START){
            if (dijkstraMaze[y][x] > (m+1)){
                dijkstraMaze[y][x] = m+1;
            }
        }
        // Check all other directions
        dijkstra_path(x, y-1, dijkstraMaze[y][x]);
        dijkstra_path(x+1, y, dijkstraMaze[y][x]);
        dijkstra_path(x, y+1, dijkstraMaze[y][x]);
        dijkstra_path(x-1, y, dijkstraMaze[y][x]);
    // Set goal if it is reached
    }else if (dijkstraMaze[y][x] == GOAL){
        dijkstraMaze[y][x] = m+1;
        goalX = x;
        goalY = y;
        return true;
    }
    // Update display maze with contents of dijkstra maze
    dijkstraMazeDisplay = dijkstraMaze;
}
// Return x coordinate of goal
int Maze::getGoalX(){
    return goalX;
}
// Return y coordinate of goal
int Maze::getGoalY(){
    return goalY;
}
// Check if the goal coordinate has been reached
int Maze::check_end(){
    // Iterate through maze to find end coordinates
    for (int i = 0; i < maze.numrows(); i++){
        for (int j = 0; j < maze.numcols(); j++){
            if (maze[i][j] == 'G'){
                goalY = i;
                goalX = j;
            }
        }
    }
    // If goal variable has not been changed, return -1
    if (dijkstraMaze[goalY][goalX] == GOAL){
        return -1;
    // If goal variable has been changed return 0 to represent a solved maze
    }else{
        return 0;
    }
}

// Find shortest path in dijkstra maze
bool Maze::true_path(int x, int y, int avoid_Direction){
    // Check if coordinates are in the matrix, if not return false
    if (x < 0 || x > dijkstraMaze.numcols()-1 || y < 0 || y > dijkstraMaze.numrows()-1){
        return false;
    }
    // If start has been reached return true
    if (dijkstraMaze[y][x] == START){
        return true;
    // Otherwise check all directions to find correct path
    }else{
        // If you did not come from the direction you are searching and the coordinates are in the matrix, search that location
        if (avoid_Direction != SOUTH && check_new_coordinate(x, y, NORTH)){
            // Check to see if location was one less than location you are starting from
            if (dijkstraMaze[y-1][x] == dijkstraMaze[y][x]-1){
                // If coordinate is not start coordinate, mark it as the right path
                if (dijkstraMaze[y-1][x] != START){
                    dijkstraMazeDisplay[y-1][x] = TRUE_PATH;
                }
                // Continue following that path
                true_path(x, y-1, NORTH);
            }
        }
        if (avoid_Direction != WEST && check_new_coordinate(x, y, EAST)){
            if (dijkstraMaze[y][x+1] == dijkstraMaze[y][x]-1){
                if (dijkstraMaze[y][x+1] != START){
                    dijkstraMazeDisplay[y][x+1] = -TRUE_PATH;
                }
                true_path(x+1, y, EAST);
            }
        }
        if (avoid_Direction != NORTH && check_new_coordinate(x, y, SOUTH)){
            if (dijkstraMaze[y+1][x] == dijkstraMaze[y][x]-1){
                if (dijkstraMaze[y+1][x] != START){
                    dijkstraMazeDisplay[y+1][x] = TRUE_PATH;
                }
                true_path(x, y+1, SOUTH);
            }
        }
        if (avoid_Direction != EAST && check_new_coordinate(x, y, WEST)){
            if (dijkstraMaze[y][x-1] == dijkstraMaze[y][x]-1){
                if (dijkstraMaze[y][x-1] != START){
                    dijkstraMazeDisplay[y][x-1] = TRUE_PATH;
                }
                true_path(x-1, y, WEST);
            }
        }
    }
}
// Checks to see if coordinate going a certain direction from a given coordinate is within the matrix
bool Maze::check_new_coordinate(int x, int y, int direction){
    // Checks each direction depending on if it is the one that was passed through
    if (direction == NORTH){
        if ((x < 0) || (x > dijkstraMaze.numcols()-1) || (y-1 < 0) || (y-1 > dijkstraMaze.numrows()-1)){
            return false;
        }else{
            return true;
        }
    }else if (direction == EAST){
        if ((x+1 < 0) || (x+1 > dijkstraMaze.numcols()-1) || (y < 0) || (y > dijkstraMaze.numrows()-1)){
            return false;
        }else{
            return true;
        }
    }else if (direction == SOUTH){
        if ((x < 0) || (x > dijkstraMaze.numcols()-1) || (y+1 < 0) || (y+1 > dijkstraMaze.numrows()-1)){
            return false;
        }else{
            return true;
        }
    }else if (direction == WEST){
        if ((x-1 < 0) || (x-1 > dijkstraMaze.numcols()-1) || (y < 0) || (y > dijkstraMaze.numrows()-1)){
            return false;
        }else{
            return true;
        }
    }
}

/// Prints the contents of the dijkstra maze -not in use, just used for debugging
void Maze::print_dijkstra_maze(){
    for (int i = 0; i < dijkstraMaze.numrows(); i++){
        for (int j = 0; j < dijkstraMaze.numcols(); j++){
            cout << dijkstraMaze[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
