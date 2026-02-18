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

void pintarLimites() {
	// Pintar líneas horizontales
	for(int i = 2; i < 78; i++) {
		goToXY(i,3);
		printf("\u2550");
		goToXY(i,28);
		printf("\u2550");
	}
	
	// Pintar líneas verticales
	for (int i = 4; i < 28; i++) {
		goToXY(2,i);
		printf("\u2551");
		goToXY(77,i);
		printf("\u2551");
	}
	
	// Pintar esquinas
	goToXY(2,3);
	printf("\u2554");
	goToXY(2,28);
	printf("\u255A");
	goToXY(77,3);
	printf("\u2557");
	goToXY(77,28);
	printf("\u255D");
}

class Nave {
	int x, y; // Por defecto son privados
	int corazones;
	public:
		// El constructor se nombra igual que la clase
		Nave(int _x,int _y, int _corazones);
		void pintar();
		void borrar();
		void mover();
		void pintarCorazones();
};

Nave::Nave(int _x, int _y, int _corazones) {
	x = _x;
	y = _y;
	corazones = _corazones;
}
// Nave::Nave(int _x,int _y): x(_x),y(_y) {}  -> alternativa

void Nave::pintar() {
	goToXY(x,y);
	printf("  \u25B2");
	goToXY(x,y+1);
	printf(" (\u266B)");
	goToXY(x,y+2);
	printf("\u25B2\u00A5 \u00A5\u25B2");
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
				x <= 3 ? x : x--;
				break;
			case 'd': 
				x + 5 >= 77 ? x : x++;
				break;
			case 'w':
				y <= 4 ? y : y--;
				break;
			case 's':
				y + 3 >= 28 ? y : y++;
				break;
			case UP: 
				y <= 4 ? y : y--;
				break;
			case DOWN: 
				y + 3 >= 28 ? y : y++;
				break;
			case LEFT:
				x <= 3 ? x : x--;
				break;
			case RIGHT:
				x + 5 >= 77 ? x : x++;
				break;
			case 'e':
				corazones--;
				break;
			default:
				break;
		}
		pintar();
		pintarCorazones();
	}
}

void Nave::pintarCorazones() {
	goToXY(64,2);
	printf("Vidas");
	goToXY(70,2);
	printf("     ");
	for (int i = 0; i < corazones; i++) {
		goToXY(70+i,2);
		printf("\u2665");
	}
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	hideCursor();
	pintarLimites();
	Nave nave(8,8,3);
	nave.pintar();
	nave.pintarCorazones();
	
	bool gameOver = false;
	while(!gameOver) {
		nave.mover();
		
		Sleep(30); // Detiene ejecución del programa por 30 ms
	}

	return 0;
}