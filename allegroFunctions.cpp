// Define all allegro based display functions
#include "Maze.h"

// Redeclare global variables
extern ALLEGRO_DISPLAY *display;

// Initialize allegro, display and font
bool Maze::initialize_allegro(){
    // Check if different components were initialized correctly
    if (!al_init() || !al_init_font_addon() || !al_init_ttf_addon() || !al_init_primitives_addon()){
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }
    // Create display
    int width = 0;
    // Make sure width is big enough to fit legend
    if (maze.numcols() < 10){
        width = 7;
    }else{
        width = maze.numcols();
    }
    // Create display
    display = al_create_display((width*TEXT_SIZE),((maze.numrows()*TEXT_SIZE)+(TEXT_SIZE*7)));
    // Check if display was created
    if (!display){
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize display.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }
    // Initialize font
    font = al_load_ttf_font("OpenSans-Regular.ttf", TEXT_SIZE, 0);
    // Check if font was initialized
    if (!font){
        cout << "Could not initialize font." << endl;
        return false;
    }
    // Set title for graphics window
    al_set_window_title(display, "Maze Assignment");
    return true;
}

// Displays legend with colour key
void display_legend(Maze &m){
    // Initialize white colour
    ALLEGRO_COLOR white = al_map_rgb(255,255,255);
    // Initialize y and x position in graphics window
    int yPosition = (m.getRows() * TEXT_SIZE)+5;
    int xPosition = TEXT_SIZE + 5;

    // Print words in legend
    al_draw_text(m.font, white, xPosition, (yPosition), ALLEGRO_ALIGN_LEFT, "Start");
    al_draw_text(m.font, white, xPosition, (yPosition+(TEXT_SIZE)), ALLEGRO_ALIGN_LEFT, "End");
    al_draw_text(m.font, white, xPosition, (yPosition+(TEXT_SIZE*2)), ALLEGRO_ALIGN_LEFT, "Wall");
    al_draw_text(m.font, white, xPosition, (yPosition+(TEXT_SIZE*3)), ALLEGRO_ALIGN_LEFT, "Open");
    al_draw_text(m.font, white, xPosition, (yPosition+(TEXT_SIZE*4)), ALLEGRO_ALIGN_LEFT, "Good Path");
    al_draw_text(m.font, white, xPosition, (yPosition+(TEXT_SIZE*5)), ALLEGRO_ALIGN_LEFT, "Wrong Path");

    // Initialize y position for colours in legend
    int legendY = yPosition+5;

    // Print colours in legend
    al_draw_filled_rectangle(0, legendY, TEXT_SIZE, legendY+TEXT_SIZE, al_map_rgb(0,100,255));
    al_draw_filled_rectangle(0, legendY+TEXT_SIZE, TEXT_SIZE, legendY+TEXT_SIZE*2, al_map_rgb(0,255,100));
    al_draw_filled_rectangle(0, legendY+TEXT_SIZE*2, TEXT_SIZE, legendY+TEXT_SIZE*3, al_map_rgb(100,100,100));
    al_draw_filled_rectangle(0, legendY+TEXT_SIZE*3, TEXT_SIZE, legendY+TEXT_SIZE*4, al_map_rgb(200,200,200));
    al_draw_filled_rectangle(0, legendY+TEXT_SIZE*4, TEXT_SIZE, legendY+TEXT_SIZE*5, al_map_rgb(10,200,150));
    al_draw_filled_rectangle(0, legendY+TEXT_SIZE*5, TEXT_SIZE, legendY+TEXT_SIZE*6, al_map_rgb(255,0,0));

    // Update display
    al_flip_display();
}

// Displays maze to allegro screen as it is updated
void Maze::display_maze(){
    // Initialize mazeChanged variable
    bool mazeChanged = false;
    // Iterate through maze to see if it has changed
    for (int i = 0; i < maze.numrows(); i++){
        for (int j = 0; j < maze.numcols(); j++){
            if (tempMaze[i][j] != maze[i][j]){
                mazeChanged = true;
            }
        }
    }
    // If the maze has changed, display it
    if (mazeChanged){
        // Declare allegro colour
        ALLEGRO_COLOR colour;
        // Iterate through maze and assign a colour depending on the contents of the coordinate
        for (int i = 0; i < maze.numrows(); i++){
            for (int j = 0; j < maze.numcols(); j++){
                switch (maze[i][j]){
                    // If open, display light gray colour
                    case '.':
                        colour = al_map_rgb(200,200,200);
                        break;
                    // If good path, display turquoise colour
                    case '+':
                        colour = al_map_rgb(10,200,150);
                        break;
                    // If wrong path, display red colour
                    case '-':
                        colour = al_map_rgb(255,0,0);
                        break;
                    // If start coordinate, display blue colour
                    case 'S':
                        colour = al_map_rgb(0,100,255);
                        break;
                    // If goal coordinate, display green colour
                    case 'G':
                        colour = al_map_rgb(0,255,100);
                        break;
                    // Otherwise display dark gray for maze walls
                    default:
                        colour = al_map_rgb(100,100,100);
                }
                // Draw rectangles at coordinate with the corresponding colour
                al_draw_filled_rectangle((j*TEXT_SIZE), (i*TEXT_SIZE), TEXT_SIZE+(j*TEXT_SIZE), TEXT_SIZE+(i*TEXT_SIZE), colour);
            }
        }
        // Update secondary maze variable
        tempMaze = maze;
        // Update display
        al_flip_display();
        al_rest(.2);
    }
}

// Displays final dijkstra maze to screen (shortest path)
void Maze::display_dijkstra_maze(){
    // Checks to see if a path was found, if it wasn't it doesn't display the maze
    if (check_end() != -1){
        // Declare open coordinate variable
        const int open = dijkstraMaze.numrows()*dijkstraMaze.numcols();
        // Find shortest path
        true_path(goalX, goalY, NULL_DIRECTION);
        // Define allegro colour
        ALLEGRO_COLOR colour;
        // Iterate through all coordinates and assign them each a colour
        for (int i = 0; i < dijkstraMaze.numrows(); i++){
            for (int j = 0; j < dijkstraMaze.numcols(); j++){
                // If start coordinate, display blue colour
                if (dijkstraMazeDisplay[i][j] == START){
                    colour = al_map_rgb(0,100,255);
                // If goal coordinate, display green colour
                }else if ((j == goalX && i == goalY) || dijkstraMazeDisplay[i][j] == GOAL){
                    colour = al_map_rgb(0,255,100);
                // If wall, display dark gray colour
                }else if (dijkstraMazeDisplay[i][j] == WALL){
                    colour = al_map_rgb(100,100,100);
                // If shortest path, display turquoise colour
                }else if (dijkstraMazeDisplay[i][j] == TRUE_PATH){
                    colour = al_map_rgb(10,200,150);
                // Otherwise display light gray open colour
                }else{
                    colour = al_map_rgb(200,200,200);
                }
                // Draw rectangles at coordinate with the corresponding colour
                al_draw_filled_rectangle((j*TEXT_SIZE), (i*TEXT_SIZE), TEXT_SIZE+(j*TEXT_SIZE), TEXT_SIZE+(i*TEXT_SIZE), colour);
            }
        }
        // Update display
        al_flip_display();
        al_rest(.2);
    }
}
