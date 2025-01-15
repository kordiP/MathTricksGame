#pragma once

const int GRID_SIZE_MIN = 4;
const int GRID_SIZE_MAX = 14;

const int OPERATIONS_COUNT = 4;
const char OPERATIONS_ARR[] = { '+', '-', '*', '/' };

const int COLOR_BLACK = 0;
const int COLOR_BLUE = 9;
const int COLOR_GREEN = 10;
const int COLOR_YELLOW = 14;
const int COLOR_WHITE = 15;

const int PLAYER_A_COLOR = COLOR_YELLOW + 16 * COLOR_BLUE;
const char PLAYER_A_NAME[] = "Player 1 (blue)";

const int PLAYER_B_COLOR = COLOR_YELLOW + 16 * COLOR_GREEN;
const char PLAYER_B_NAME[] = "Player 2 (Green)";

const int CELL_VAL_RANGE = 2;
const int MULTIPLIER_MAX = 4;

const int FILE_LINE_LEN_MAX = 255;

const char SAVE_FILE_NAME[] = "saveFile.txt";