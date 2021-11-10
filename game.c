#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <curses.h>
#include <locale.h>

// need to know where the front and back of the snake are
// 2d array: position (x, y), whether it's a snake, direction
// also store start and end coordinates of the snake

#define WIDTH 21
#define HEIGHT 21

#define EMPTY 0
#define APPLE 1 // 1 = apple
#define SNAKE_UP 2 // 2 = snake up
#define SNAKE_RIGHT 3 // 3 = snake right
#define SNAKE_DOWN 4 // 4 = snake down
#define SNAKE_LEFT 5 // 5 = snake left

#define START_X ((WIDTH / 2) - 1)
#define START_Y ((HEIGHT / 2) - 1)

#define ESC 27
#define CTRL 91
#define ARROW_UP 65
#define ARROW_DOWN 66
#define ARROW_LEFT 68
#define ARROW_RIGHT 67

int board[HEIGHT][WIDTH];

int snake_head_x = START_X;
int snake_head_y = START_Y;

int snake_tail_x = START_X;
int snake_tail_y = START_Y;

void display_board() {
  printf("\n\r");

  for(int y = 0; y < HEIGHT; ++y) {
    for(int x = 0; x < WIDTH; ++x) {
      printf("%d ", board[y][x]);
    }

    printf("\n\r");
  }
}

void place_apple() {
  int x, y;

  do {
    x = rand()%WIDTH;
    y = rand()%HEIGHT;
  } while(board[y][x] != EMPTY);

  board[y][x] = APPLE;
}

void initialize_board() {
  board[START_Y][START_X] = SNAKE_RIGHT;
  place_apple();
}

void move_snake_tail(int tail_direction) {
  // unless an apple eaten, move tail in the same way as head
  int direction;

  if (tail_direction == 0) {
    direction = board[snake_tail_y][snake_tail_x];
  } else {
    direction = tail_direction;
  }

  board[snake_tail_y][snake_tail_x] = 0;

  switch(direction) {
    case SNAKE_UP:
      snake_tail_y--;
      break;
    case SNAKE_RIGHT:
      snake_tail_x++;
      break;
    case SNAKE_DOWN:
      snake_tail_y++;
      break;
    case SNAKE_LEFT:
      snake_tail_x--;
      break;
  }

  board[snake_tail_y][snake_tail_x] = direction;
}

void move_snake_head(int direction) {
  // (later: unless apple eaten)
  // remove snake from tail current location
  // increment tail and head
  int tail_direction = 0;

  if (snake_head_x == snake_tail_x && snake_head_y == snake_tail_y) {
    tail_direction = direction;
  }

  switch(direction) {
    case SNAKE_UP:
      snake_head_y--;
      break;
    case SNAKE_RIGHT:
      snake_head_x++;
      break;
    case SNAKE_DOWN:
      snake_head_y++;
      break;
    case SNAKE_LEFT:
      snake_head_x--;
      break;
  }

  int new_location = board[snake_head_y][snake_head_x];

  if (new_location == EMPTY && snake_head_x < WIDTH && snake_head_y < HEIGHT) {
    // board[snake_tail_y][snake_tail_x] = 0;
    // snake_tail_x = snake_head_x;
    // snake_tail_y = snake_head_y;

    board[snake_head_y][snake_head_x] = direction;
    move_snake_tail(tail_direction);
  } else if (new_location == APPLE) {
    // don't replace tail, so tail grows
    board[snake_head_y][snake_head_x] = direction;
    place_apple();
  } else {
    printf("Sorry, you lost!\n\r");
    // exit(1);
  }

}

void next_board(int direction) {
  // for any non-head snake, the snake takes over the value of the next spot
  move_snake_head(direction);
}

int get_arrow_keys() {
  refresh();
  timeout(1000);
  int c;

  if (getch() == ESC) {
    if (getch() == CTRL) {
      c = getch();
    }
  }

  return c;
}

int main() {
  srand(time(NULL));
  keypad(stdscr, TRUE);
  initialize_board();

  setlocale(LC_ALL, "");
  initscr();
  cbreak();

  int next_direction = SNAKE_RIGHT;

  while(1) {
    printf("\033[H\033[J\033[H");

    next_board(next_direction);
    display_board();

    int c = get_arrow_keys();

    switch(c) {
      case ARROW_UP:
        next_direction = SNAKE_UP;
        break;
      case ARROW_DOWN:
        next_direction = SNAKE_DOWN;
        break;
      case ARROW_LEFT:
        next_direction = SNAKE_LEFT;
        break;
      case ARROW_RIGHT:
        next_direction = SNAKE_RIGHT;
        break;
      default:
        break;
    }
  }

  endwin();
  return 0;
}

// move head and move tail funcs
// move head already exists basically
// move tail: unless apple, move tail the same as head
