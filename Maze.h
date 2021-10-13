// Define Maze Class

#include "includes.h"
#ifndef MAZE_H_INCLUDED
#define MAZE_H_INCLUDED
class Maze{
    // Define all public member functions
    public:
        // Constructors
        Maze();
        Maze(string fileName);
        // Deconstructor
        ~Maze();
        // Maze member functions
        bool initialize_allegro();
        bool read_maze();
        void find_start();
        void print_dijkstra_maze(); /// not in use- just for bug fixes
        bool find_path(int x, int y);
        void display_maze();
        // Dijkstra related member functions
        bool dijkstra_path(int x, int y, int m);
        void load_dijkstra_maze();
        void display_dijkstra_maze();
        bool true_path(int x, int y, int avoid_Direction);
        bool check_new_coordinate(int x, int y, int direction);
        int check_end();
        // Helper member functions
        int getStartX();
        int getStartY();
        int getGoalX();
        int getGoalY();
        int getRows();
        // Declare font member variable
        ALLEGRO_FONT *font;
    private:
        // Declare all other private member variables
        apmatrix<char> maze;
        apmatrix<char> tempMaze;
        apmatrix<int> dijkstraMaze;
        apmatrix<int> dijkstraMazeDisplay;
        string mazeFileName;
        int startX;
        int startY;
        int goalX;
        int goalY;
};

#endif // MAZE_H_INCLUDED
