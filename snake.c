#include <ncurses.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#include <time.h>

#define MAXWIDTH 79
#define MAXHEIGHT 23

int kbhit (void) {
  /* checks if key has been hit or not */
  struct timeval tv;
  fd_set read_fd;

  tv.tv_sec=0;
  tv.tv_usec=0;
  FD_ZERO(&read_fd);
  FD_SET(0,&read_fd);

  if(select(1, &read_fd, NULL, NULL, &tv) == -1)
    return 0;

  if(FD_ISSET(0,&read_fd))
    return 1;

  return 0;
} 


int colliding(int *snakeArray, int snakeLength) {
  /* checks if snake is colliding with walls */
  int x = *snakeArray;
  int y = *(snakeArray+1);

  if (x < 0 || x > MAXWIDTH) {
    return 1;
  }
  else if (y < 0 || y > MAXHEIGHT) {
    return 1;
  }

  for (size_t i = 2; i < snakeLength*2+2; i+=2) {
    if (x == *(snakeArray+i) && y == *(snakeArray+i+1)) {
      return 1;
    }
  }
  return 0;
}

void moveSnake(int *snakeArray, int dirX, int dirY, int speed, int snakeLength) {
  /* moves snake one position to the direction provided also controls the speed
     of the game using usleep function */
  int oldX = 0;
  int oldY = 0;
  int curX = *snakeArray;
  int curY = *(snakeArray+1);
  int newX = curX + dirX;
  int newY = curY + dirY;
  int col = 0;
  int row = 0;
  *(snakeArray) = newX;
  *(snakeArray+1) = newY;
  attron(COLOR_PAIR(1));
  mvprintw(newY, newX, "O");

  for (size_t i = 2; i < snakeLength*2; i += 2) {
    oldX = curX;
    oldY = curY;
    curX = *(snakeArray+i);
    curY = *(snakeArray+i+1);
    newX = oldX;
    newY = oldY;
    *(snakeArray+i) = newX;
    *(snakeArray+i+1) = newY;

    mvprintw(newY, newX, "O");
  }
  if (dirY != 0) {
    usleep(500000/speed);
  }

  usleep(1000000/speed);
}

void letThereBeApple(int* appleX, int* appleY, int *appleEaten) {
  /* new random apple location if apple is eaten, if not then prints apple at
     previous location */
  if (*appleEaten) {
    srand(time(0)); //use current time as seed for random generator
    *appleX = (rand() % MAXWIDTH)+1;
    *appleY = (rand() % MAXHEIGHT)+1;
    *appleEaten = 0;
  }
  attron(COLOR_PAIR(2));
  mvprintw(*appleY, *appleX,"O");
}

void eatApple(int *snakeArray, int appleX, int appleY,
              int *appleEaten, int *snakeLength) {
  /* if snake find apple get one point, also sets appleEaten to true and
  prints snakeLength and debug info */
  int x = *snakeArray;
  int y = *(snakeArray+1);
  if (x == appleX && y == appleY) {
    *appleEaten = 1;
    *snakeLength += 1;
    int snakeLen = *snakeLength;
    int lastX = *(snakeArray + snakeLen*2-2);
    int lastY = *(snakeArray + snakeLen*2-2+1);
    *(snakeArray + snakeLen*2) = lastX;
    *(snakeArray + snakeLen*2+1) = lastY;
  }
  mvprintw(0,2,"Iskor: %d", *snakeLength - 4);
}

int main() {

  int snakeArray[100][2];
  for (size_t i = 0; i < 100; i++) {
    for (size_t j = 0; j < 2; j++) {
      snakeArray[i][j] = 0;
    }
  }
  snakeArray[0][0] = 3;
  snakeArray[0][1] = 3;
  int keyPressed = 0;   /* which key user pressed */
  int dirX = 1;        /* direction xy */
  int dirY = 0;
  int speed = 10;       /* controls speed of the snake */
  int appleX = 0;       /* current apple xy position */
  int appleY = 0;
  int appleEaten = 1;   /* is apple eaten? */
  int snakeLength = 4;  /* player snakeLength */



  initscr();

 void draw_borders(WINDOW *screen) {
  int x, y, i; getmaxyx(screen, y, x);

// 4 corners
  mvwprintw(screen, 0, 0, "+");
  mvwprintw(screen, y - 1, 0, "+");
  mvwprintw(screen, 0, x - 1, "+");
  mvwprintw(screen, y - 1, x - 1, "+");

// sides
 for (i = 1; i < (y - 1); i++) {
    mvwprintw(screen, i, 0, "|");
    mvwprintw(screen, i, x - 1, "|");
    }

// top and bottom
  for (i = 1; i < (x - 1); i++) {
    mvwprintw(screen, 0, i, "-");
    mvwprintw(screen, y - 1, i, "-");
    }
}
  start_color();
  init_pair(1,COLOR_GREEN,COLOR_BLACK);
  init_pair(2,COLOR_YELLOW,COLOR_BLACK);			/* Start curses mode */
  curs_set(false);
  noecho();


  /* MAIN LOOP */
  while (!colliding(&snakeArray[0][0], snakeLength)) {
    erase();

    letThereBeApple(&appleX, &appleY, &appleEaten);
    moveSnake(&snakeArray[0][0], dirX, dirY, speed, snakeLength);
    eatApple(&snakeArray[0][0], appleX, appleY, &appleEaten, &snakeLength);


    refresh();

    /* change direction with wasd */
    if (kbhit()) {
      keyPressed = getch();

      if (keyPressed == 'w' && !(dirY == 1 && dirX == 0)) {
        dirY = -1;
        dirX = 0;
      }
      if (keyPressed == 's' && !(dirY == -1 && dirX == 0)) {
        dirY = 1;
        dirX = 0;
      }
      if (keyPressed == 'a' && !(dirY == 0 && dirX == 1)) {
        dirY = 0;
        dirX = -1;
      }
      if (keyPressed == 'd' && !(dirY == 0 && dirX == -1)) {
        dirY = 0;
        dirX = 1;
      }
    }


  }
  erase();
  attron(COLOR_PAIR(1));
  mvprintw(MAXHEIGHT/1.8,MAXWIDTH/4,"Total Score: %d", snakeLength - 4);
  mvprintw(MAXHEIGHT/1.2,MAXWIDTH/4,"Press Any Key!");
  
  	getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */

	return 0;
}
