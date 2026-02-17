#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

void goToXY(int x, int y) {
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	
	SetConsoleCursorPosition(hCon,dwPos);
}

void hideCursor() {
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 20;
	cci.bVisible = false;
	
	SetConsoleCursorInfo(hCon,&cci);
}

class Nave {
	int x, y; // Por defecto son privados
	public:
		Nave(int _x,int _y); // El constructor se nombra igual que la clase
		void pintar();
		void borrar();
		void mover();
};

Nave::Nave(int _x, int _y) {
	x = _x;
	y = _y;
}
// Nave::Nave(int _x,int _y): x(_x),y(_y) {}  -> alternativa

void Nave::pintar() {
	goToXY(x,y);
	printf("  %c",142);
	goToXY(x,y+1);
	printf(" %c%c%c",40,35,41);
	goToXY(x,y+2);
	printf("%c%c %c%c",220,223,223,220);
}

void Nave::borrar() {
	for (int i = 0; i < 3; i++) {
		goToXY(x,y+i);
		printf("     ");
	}
}

void Nave::mover() {
	if (kbhit()) {
		char tecla = getch();
		borrar();
		switch (tecla)  {
			case 'a': 
				x--;
				break;
			case 'd': 
				x++;
				break;
			case 'w':
				y--;
				break;
			case 's':
				y++;
				break;
			case UP: 
				y--;
				break;
			case DOWN: 
				y++;
				break;
			case LEFT:
				x--;
				break;
			case RIGHT:
				x++;
				break;
			default:
				break;
		}
		pintar();
	}
}

int main() {
	hideCursor();
	Nave nave(8,8);
	nave.pintar();
	
	bool gameOver = false;
	while(!gameOver) {
		nave.mover();
		
		Sleep(30); // Detiene ejecución del programa por 30 ms
	}

	return 0;
}