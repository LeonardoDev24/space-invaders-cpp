#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <list>
using namespace std;

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
	int vidas;
	public:
		// El constructor se nombra igual que la clase
		Nave(int _x,int _y, int _corazones,int _vidas);
		int getX();
		int getY();
		void setCorazones();
		void pintar();
		void borrar();
		void mover();
		void pintarCorazones();
		void explosion();
};

Nave::Nave(int _x, int _y, int _corazones, int _vidas) {
	x = _x;
	y = _y;
	corazones = _corazones;
	vidas = _vidas;
}
// Nave::Nave(int _x,int _y): x(_x),y(_y) {}  -> alternativa

int Nave::getX() {
	return x;
}

int Nave::getY() {
	return y;
}

void Nave::setCorazones() {
	corazones--;
}

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
		printf("        ");
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
	goToXY(50,2);
	printf("Vidas %d",vidas);
	goToXY(64,2);
	printf("Salud");
	goToXY(70,2);
	printf("     ");
	for (int i = 0; i < corazones; i++) {
		goToXY(70+i,2);
		printf("\u2665");
	}
}

void Nave::explosion() {
	if (corazones == 0) {
		borrar();
		goToXY(x,y);
		printf("   **   ");
		goToXY(x,y+1);
		printf("  ****  ");
		goToXY(x,y+2);
		printf("   **   ");
		Sleep(200);
		
		borrar();
		goToXY(x,y);
		printf(" * ** * ");
		goToXY(x,y+1);
		printf("  ****  ");
		goToXY(x,y+2);
		printf(" * ** * ");
		Sleep(200);
		borrar();
		
		vidas--;
		corazones = 3;
		pintarCorazones();
		pintar();
	}
}

class Asteroide {
	int x,y;
	public:
		Asteroide(int _x, int _y);
		void pintar();
		void mover();
		void choque(class Nave &N);
};

Asteroide::Asteroide(int _x,int _y) {
	x = _x;
	y = _y;
}

void Asteroide::pintar() {
	goToXY(x,y);
	printf("\u24B8");
}

void Asteroide::mover() {
	goToXY(x,y);
	printf(" "); // Borrar
	y++;
	if (y > 27) {
		x = rand()%71 + 4;
		y = 4;
	}
	pintar();
}

void Asteroide::choque(class Nave &N) {
	if (x >= N.getX() && x < N.getX() + 6 && y >= N.getY() && y < N.getY() + 2) {
		N.setCorazones();
		N.borrar();
		N.pintar();
		N.pintarCorazones();
		
		x = rand()%71 + 4;
		y = 4;
	}
}

class Bala {
	int x,y;
	public:
		Bala(int _x, int _y);
		void mover();
};

Bala::Bala(int _x, int _y) {
	x = _x;
	y = _y;
}

void Bala::mover() {
	goToXY(x,y);
	printf(" "); // Borrar
	if (y > 4) y--;
	goToXY(x,y);
	printf("*");
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	hideCursor();
	pintarLimites();
	
	Nave nave(8,8,3,3);
	nave.pintar();
	nave.pintarCorazones();
	
	Asteroide ast1(10,4),ast2(4,8),ast3(15,10),ast4(20,6),ast5(40,12);
	
	list<Bala*> balas;
	list<Bala*>::iterator it;
	
	bool gameOver = false;
	while(!gameOver) {
		if (kbhit()) {
			char tecla = getch();
			if (tecla == 'k' || tecla == 'g') {
				balas.push_back(new Bala(nave.getX()+2,nave.getY()-1));
			}
		}
		
		for (it = balas.begin(); it != balas.end(); it++) {
			// it es un puntero, para acceder a sus métodos se hace con el operador flecha
			(*it)->mover();
		}
		
		ast1.mover();
		ast1.choque(nave);
		ast2.mover();
		ast2.choque(nave);
		ast3.mover();
		ast3.choque(nave);
		ast4.mover();
		ast4.choque(nave);
		ast5.mover();
		ast5.choque(nave);
		
		nave.explosion();
		nave.mover();
		
		Sleep(30); // Detiene ejecución del programa por 30 ms
	}

	return 0;
}