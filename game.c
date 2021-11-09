#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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

int board[HEIGHT][WIDTH];

int snake_head_x = START_X;
int snake_head_y = START_Y;

int snake_tail_x = START_X;
int snake_tail_y = START_Y;

void display_board() {
  printf("\n");

  for(int y = 0; y < HEIGHT; ++y) {
    for(int x = 0; x < WIDTH; ++x) {
      printf("%d ", board[y][x]);
    }

    printf("\n");
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

void move_snake(int direction) {
  // (later: unless apple eaten)
  // remove snake from tail current location
  // increment tail and head

  board[snake_tail_y][snake_tail_x] = 0;

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

  snake_tail_x = snake_head_x;
  snake_tail_y = snake_head_y;

  int new_location = board[snake_head_y][snake_head_x];

  if (new_location == EMPTY && snake_head_x < WIDTH && snake_head_y < HEIGHT) {
    printf("new location: %d", new_location);
    board[snake_head_y][snake_head_x] = direction;
  } else if (new_location == APPLE) {
    // make snake longer
  } else {
    printf("Sorry, you lost!\n");
    exit(1);
  }

}

// decide where snake head will be next
// if theres an apple, make snake longer
// if theres a snake, lose the game
// if it's an edge, lose the game
void next_board() {
  // for now, snake is just moving to the right
  move_snake(SNAKE_RIGHT);
}

int main() {
  srand(time(NULL));
  initialize_board();

  while(1) {
    usleep(1000000);

    printf("\033[H\033[J\033[H");

    // char input[200];
    // gets(input);
    // printf("input: %s", input);

    next_board();
    display_board();
  }

  return 0;
}
