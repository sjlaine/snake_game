#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#define ESC 27
#define CTRL 91
#define ARROW_UP 65
#define ARROW_DOWN 66
#define ARROW_LEFT 68
#define ARROW_RIGHT 67

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
  initscr();
  cbreak();
  printw("Tset");
  int c;
  while(1) {
    int c = get_arrow_keys();
    printf("\033[H\033[J\033[H");

    switch(c) {
      case ARROW_UP:
        printf("arrow up!");
        break;
      case ARROW_DOWN:
        printf("arrow down!");
        break;
      case ARROW_LEFT:
        printf("arrow left!");
        break;
      case ARROW_RIGHT:
        printf("arrow right!");
        break;
      default:
        break;
    }
  }
  endwin();
}
