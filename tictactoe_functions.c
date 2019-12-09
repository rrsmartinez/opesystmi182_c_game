#include <ncurses.h>
#include "tictactoe.h"

void initCurses() {

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
}

/* Draw game board */
void drawBoard() {

	int v, h;

	for(v=0;v<=6;v++) {
		mvaddch(v, 0, '|');
		mvaddch(v, 4, '|');
		mvaddch(v, 8, '|');
		mvaddch(v, 4, '|');
		mvaddch(v, 8, '|');
		mvaddch(v, 12,'|');

		if(v%2 == 0){
			for(h=0;h<=12;h++) {
				mvaddch(i, h, '=');
			}
		}
		move(1,2);
	}
	refresh();
}
