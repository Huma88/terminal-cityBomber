#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define _SCREENWIDTH 30
#define _SCREENHEIGHT 15
#define _FRAMERATE 6

typedef struct city {
    int width;
    int height;
    int widthStart;
    int buildings[_SCREENWIDTH];
    char icon;
} city;

typedef struct position {
    int x;
    int y;
} position;

typedef struct bomb {
    char icon;
    int speed;
    int location[_SCREENWIDTH];
} bomb;

typedef struct plane {
    char icon;
    int speed;
    int bombNumber;
    position pos;
    bomb planebombs;
} plane;

void draw(city newAlen, plane bomber);
void sleep(int ms);
void clear();

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
    bomber.bombNumber = 3;
    bomber.pos.x = 0;
    bomber.pos.y = _SCREENHEIGHT;
    //creamos las bombas del avion
    bomber.planebombs.icon = '\'';
    bomber.planebombs.speed = 1;
    for(int i = 0; i < newAlen.width; i++) {
        bomber.planebombs.location[i] = -1;
    }
    //dibujamos
    draw(newAlen, bomber);
}

void draw(city newAlen, plane bomber) {
    int gameover = 0;
    int i,j;
	while(!gameover) {
        //limpiamos pantalla
        clear();
        //printamos pantalla
        for(i = _SCREENHEIGHT; i > -1; i--) {
            for(j = 0; j < _SCREENWIDTH; j++) {
                if (newAlen.buildings[j] >= i) {
                    printf("%c",newAlen.icon);
                } else if (bomber.planebombs.location[j] == i){
                    printf("%c",bomber.planebombs.icon);
                } else if (bomber.pos.x == j && bomber.pos.y == i) {
                    printf("%c",bomber.icon);
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }
        //comprobamos si se ha soltado bomba
        if(bomber.bombNumber > 0) {
            if(kbhit()) {
                char key = getch();
                if(key == ' ') {
                    bomber.planebombs.location[bomber.pos.x] = bomber.pos.y;
                    bomber.bombNumber--;
                }
            }
        }
        //limpiamos el buffer para que solo se admita una pulsacion por frame y no se acumule el lanzamiento de bombas
        fflush(stdin);
        //movemos bomber
        bomber.pos.x++;
        if(bomber.pos.x > _SCREENWIDTH) {
            bomber.pos.x = 0;
            bomber.pos.y--;
        }
        //movemos bombas
        for(i = 0; i < _SCREENWIDTH;i++) {
            if(bomber.planebombs.location[i] != -1) {
                bomber.planebombs.location[i]--;
                if(bomber.planebombs.location[i] <= newAlen.buildings[i]) {
                    newAlen.buildings[i]--;
                    bomber.planebombs.location[i] = -1;
                    bomber.bombNumber++;
                }
                if(bomber.planebombs.location[i] == 0) {
                    bomber.bombNumber++;
                }
            }
        }
        if(bomber.pos.x < _SCREENWIDTH && newAlen.buildings[bomber.pos.x] >= bomber.pos.y) {
            gameover = 1;
        }
        //sleepeamos para crear el frameRate
        sleep(1000/_FRAMERATE);
    }
    clear();
    printf("YOU LOSE!\n");
}

void sleep(int ms) {
    #ifdef _WIN32
        Sleep(ms);
    #else
        usleep(ms*1000);  /* sleep for 100 milliSeconds */
    #endif
}

void clear() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}