/*********************************************
* Erica Buchanan           Maze Assignment   *
* 03/05/2021                                 *
*                                            *
* This program solves a maze, following "."  *
* as open spaces, "S" as the starting point  *
* and "G" as an ending point. Any other      *
* character is considered a block in the     *
* path. The maze can be solved either using  *
* a Depth First Search algorithm or using    *
* Dijkstra's algorithm                       *
*********************************************/

#include "includes.h"
#include "Maze.h"

// Declare global variable
ALLEGRO_DISPLAY *display;

// Declare prototype
void display_legend(Maze &m);

int main(int argc, char *argv[]){
    // Asks user to pick an algorithm
    cout << "Pick algorithm 1 (Dijkstra) or 2 (DFS): ";
    int choice;
    cin >> choice;
    // Read in the maze from a text file and return -1 if not able to read
    Maze m("maze1.txt");
    if(!m.read_maze()){
        return -1;
    }
    // Initialize allegro and return -1 if something isn't initialized
    if (!m.initialize_allegro()){
        return -1;
    }
    // Display the maze legend in allegro window
    display_legend(m);
    // Find the coordinates of the starting point
    m.find_start();
    // Solve the maze using the user's algorithm choice
    if (choice == DIJKSTRA){
        // Load corresponding maze of numbers from regular character maze
        m.load_dijkstra_maze();
        // Find best path using dijkstra algorithm
        m.dijkstra_path(m.getStartX(), m.getStartY(), -2);
        // Check if a path was found and tell user
        if (m.check_end() == -1){
            cout << "No path found." << endl;
        }else{
            cout << "Path found!" << endl;
            // Display dijkstra solved maze
            m.display_dijkstra_maze();
            // Let user enter character to close allegro window
            cout << "Return any character to exit";
            char character;
            cin >> character;
        }
    }else if (choice == DFS){
        // Find path using DFS algorithm
        if (!m.find_path(m.getStartX(), m.getStartY())){
            // Tell user if path was found or not
            cout << "No path found." << endl;
        }else{
            cout << "Path found!!" << endl;
        }
        // Display algorithm solving the maze
        m.display_maze();
        // Let user enter character to close allegro window
        cout << "Return any character to exit ";
        char character;
        cin >> character;
    }
    return 0;
}
