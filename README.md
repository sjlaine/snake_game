# SNAKE GAME IN C!

## Play the Game
Clone this repo, and cd into the `snake_game` directory in a terminal.
Run `./snake_game` to play!

### Rules
Use the arrow keys to change the direction of the snake.
You earn points by eating apples (magenta `0`s).
If you hit yourself (the snake) or the edge of the board, you lose.
Have fun!

## Development

In order to compile the game code, you must have a gcc installed. To check if
you have it, run `which gcc`.

To compile:
`gcc game.c -lncurses`

Here you can add an optional output flag, to tell the compiler what to name the
output script (ex: `-o snake_game`).
