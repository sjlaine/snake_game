#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <curses.h>
#include <locale.h>
#include <inttypes.h>

// need to know where the front and back of the snake are
// 2d array: position (x, y), whether it's a snake, direction
// also store start and end coordinates of the snake

#define WIDTH 21
#define HEIGHT 21

#define EMPTY 0
#define APPLE 1
#define SNAKE_UP 2
#define SNAKE_RIGHT 3
#define SNAKE_DOWN 4
#define SNAKE_LEFT 5

#define START_X ((WIDTH / 2) - 1)
#define START_Y ((HEIGHT / 2) - 1)

#define ESC 27
#define CTRL 91
#define ARROW_UP 65
#define ARROW_DOWN 66
#define ARROW_LEFT 68
#define ARROW_RIGHT 67

// colors
#define MAG "\e[0;35m"
#define GRN "\e[0;32m"
#define BCYN "\e[1;36m"
#define LGRY "\e[0;37m"

#define reset "\e[0m"

int board[HEIGHT][WIDTH];
int score = 0;

int snake_head_x = START_X;
int snake_head_y = START_Y;

int snake_tail_x = START_X;
int snake_tail_y = START_Y;

void display_board() {
  printf("\n\r");

  for(int y = 0; y < HEIGHT; ++y) {
    printf(" ");
    for(int x = 0; x < WIDTH; ++x) {
      int spot = board[y][x];
      if (spot == APPLE) {
        printf(MAG "0 " reset);
      } else if (x == snake_head_x && y == snake_head_y) {
        switch(spot) {
          case SNAKE_UP:
            printf(GRN "^ " reset);
            break;
          case SNAKE_RIGHT:
            printf(GRN "> " reset);
            break;
          case SNAKE_DOWN:
            printf(GRN "v " reset);
            break;
          case SNAKE_LEFT:
            printf(GRN "< " reset);
            break;
        }
      } else if (spot >= SNAKE_UP) {
        printf(GRN "O " reset);
      } else {
        printf(LGRY ". " reset);
      }
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

void remove_tail() {
  int current_tail_direction = board[snake_tail_y][snake_tail_x];
  int new_tail_x = snake_tail_x;
  int new_tail_y = snake_tail_y;

  switch(current_tail_direction) {
    case SNAKE_UP:
      new_tail_y--;
      break;
    case SNAKE_RIGHT:
      new_tail_x++;
      break;
    case SNAKE_DOWN:
      new_tail_y++;
      break;
    case SNAKE_LEFT:
      new_tail_x--;
      break;
  }

  board[snake_tail_y][snake_tail_x] = 0;
  snake_tail_x = new_tail_x;
  snake_tail_y = new_tail_y;
}

void move_snake_head(int direction) {
  int new_head_x = snake_head_x;
  int new_head_y = snake_head_y;

  switch(direction) {
    case SNAKE_UP:
      new_head_y--;
      break;
    case SNAKE_RIGHT:
      new_head_x++;
      break;
    case SNAKE_DOWN:
      new_head_y++;
      break;
    case SNAKE_LEFT:
      new_head_x--;
      break;
  }

  int new_location_type = board[new_head_y][new_head_x];


  if (new_location_type >= SNAKE_UP || snake_head_x >= WIDTH || snake_head_y >= HEIGHT) {
    endwin();
    printf(BCYN "Sorry, you lost!\n\rScore: %d\n\r" reset, score);
    exit(1);
  } else {
    // set the prev head to current direction
    board[snake_head_y][snake_head_x] = direction;
    // set new head to current direction
    board[new_head_y][new_head_x] = direction;

    // set new head coords
    snake_head_x = new_head_x;
    snake_head_y = new_head_y;

    if (new_location_type == APPLE) {
      // leave tail alone
      score++;
      place_apple();
    } else {
      remove_tail();
    }
  }
}

void next_board(int direction) {
  move_snake_head(direction);
}

int get_arrow_keys() {
  struct timespec beforeTime;
  clock_gettime(CLOCK_REALTIME,&beforeTime);    

  int c;
  long timeDiff = 0;
  // flushinp();

  // while timeDiff less than 1s, build arrowkey string
  while(timeDiff < 100000000) {
    if (getch() == ESC) {
      if (getch() == CTRL) {
        c = getch();
      }
    }

    struct timespec afterTime;
    clock_gettime(CLOCK_REALTIME,&afterTime);
    timeDiff = afterTime.tv_nsec - beforeTime.tv_nsec;
  }

  printf("timeDiff: %ld", timeDiff);

  // refresh();

  // calculate time elapsed
  // struct timespec afterTime;
  // clock_gettime(CLOCK_REALTIME,&afterTime);
  // long timeDiff = afterTime.tv_nsec - beforeTime.tv_nsec;

  // if less than 1 sec, timeout for the difference
  // long makeUpTime = 1000000000 - timeDiff;
  // if (makeUpTime > 0) {
  //   usleep(makeUpTime / 1000);
  // }
  return c;
}

int main() {
  srand(time(NULL));
  keypad(stdscr, TRUE);
  initialize_board();
  initscr();
  nodelay(stdscr, TRUE);
  
  int next_direction = SNAKE_RIGHT;

  while(1) {
    printf("\033[H\033[J\033[H");
    printf("in the first while");

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

  next_board(next_direction);
  display_board();

  endwin();
  return 0;
}
