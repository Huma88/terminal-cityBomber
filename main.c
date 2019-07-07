#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define _SCREENWIDTH 15
#define _SCREENHEIGHT 8
#define _FRAMERATE 6

typedef struct cities {
    int width;
    int height;
    char tile;
    int widthStart;
    int buildings[_SCREENWIDTH];
    char icon;
} city;

typedef struct positions {
    int x;
    int y;
} position;

typedef struct planes {
    char icon;
    int speed;
    int bombNumber;
    position pos;
} plane;

typedef struct bomb {
    char icon;
    int speed;
    position pos;
}

void draw(city newAlen, plane bomber);
void sleep(int ms);

void main() {
    srand(time(NULL));   // Initialization, should only be called once.
    //creamos ciudad
    city newAlen;
    newAlen.width = _SCREENWIDTH;
    newAlen.height = _SCREENHEIGHT -2;
    newAlen.widthStart = 0;
    newAlen.icon = 'A';
    for(int i = 0; i < newAlen.width; i++) {
        newAlen.buildings[i] = rand() % (newAlen.height+1);
    }
    //creamos avion
    plane bomber;
    bomber.icon = '>';
    bomber.speed = 1;
    bomber.bombs = 3;
    bomber.pos.x = 0;
    bomber.pos.y = _SCREENHEIGHT;
    //dibujamos
    draw(newAlen, bomber);
}

void draw(city newAlen, plane bomber) {
    int gameover = 0;
	while(!gameover) {
        //limpiamos pantalla
        system("cls");
        //pedimos tecla
        getch();
        for(int i = _SCREENHEIGHT; i > -1; i--) {
            for(int j = 0; j < _SCREENWIDTH; j++) {
                if (newAlen.buildings[j] >= i) {
                    printf("%c",newAlen.icon);
                } else if (bomber.pos.x == j && bomber.pos.y == i) {
                    printf("%c",bomber.icon);
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }
        bomber.pos.x++;
        if(bomber.pos.x > _SCREENWIDTH) {
            bomber.pos.x = 0;
            bomber.pos.y--;
        }
        if(bomber.pos.x < _SCREENWIDTH && newAlen.buildings[bomber.pos.x] >= bomber.pos.y) {
            gameover = 1;
        }
        //sleepeamos para crear el frameRate
        sleep(1000/_FRAMERATE);
    }
    system("cls");
    printf("YOU LOSE!\n");
}

void sleep(int ms) {
    #ifdef _WIN32
        Sleep(ms);
    #else
        usleep(ms*1000);  /* sleep for 100 milliSeconds */
    #endif
}