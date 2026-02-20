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
		int getVidas();
		void setCorazones();
		void pintar();
		void borrar();
		void mover(char tecla);
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

int Nave::getVidas() {
	return vidas;
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

void Nave::mover(char tecla) {
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
	if (corazones <= 0) {
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
		int getX();
		int getY();
		void pintar();
		void mover();
		void choque(class Nave &N);
};

Asteroide::Asteroide(int _x,int _y) {
	x = _x;
	y = _y;
}

int Asteroide::getX() {
	return x;
}

int Asteroide::getY() {
	return y;
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
		int getX();
		int getY();
		void mover();
		bool fuera();
};

Bala::Bala(int _x, int _y) {
	x = _x;
	y = _y;
}

int Bala::getX() {
	return x;
}

int Bala::getY() {
	return y;
}

void Bala::mover() {
	goToXY(x,y);
	printf(" "); // Borrar
	if (y > 4) y--;
	goToXY(x,y);
	printf("*");
}

bool Bala::fuera() {
	return y==4;
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	hideCursor();
	pintarLimites();
	goToXY(4,29);
	printf("This game was made with Notepad++ in C++ \u2665");
	
	Nave nave(37,22,3,3);
	nave.pintar();
	nave.pintarCorazones();
	
	list<Asteroide*> asteroides;
	list<Asteroide*>::iterator itA;
	for (int i = 0; i < 5; i++) {
		asteroides.push_back(new Asteroide(rand()%73+3,4));
	}
	
	list<Bala*> balas;
	list<Bala*>::iterator it;
	
	int puntos = 0;
	
	bool gameOver = false;
	while(!gameOver) {
		goToXY(4,2);
		printf("Puntos %d",puntos);
		
		if (kbhit()) {
			char tecla = getch();
			if (tecla == 'k' || tecla == 'g') {
				balas.push_back(new Bala(nave.getX()+2,nave.getY()-1));
			}
			nave.mover(tecla);
		}
		
		for (it = balas.begin(); it != balas.end(); it++) {
			// it es un puntero, para acceder a sus métodos se hace con el operador flecha
			(*it)->mover();
			if ((*it)->fuera()) {
				goToXY((*it)->getX(),(*it)->getY());
				printf(" ");
				delete(*it);
				it = balas.erase(it);
			}
		}
		
		for (itA = asteroides.begin(); itA != asteroides.end(); itA++) {
			(*itA)->mover();
			(*itA)->choque(nave);
		}
		
		for (itA = asteroides.begin(); itA != asteroides.end(); itA++) {
			for (it = balas.begin(); it != balas.end(); it++) {
				bool colision = (*itA)->getX() == (*it)->getX() && 
				((*itA)->getY()+1 == (*it)->getY() || (*itA)->getY() == (*it)->getY());
				
				if (colision) {
					// Borrar balas
					goToXY((*it)->getX(),(*it)->getY());
					printf(" ");
					delete(*it);
					it = balas.erase(it);
					
					// Borrar asteroides
					asteroides.push_back(new Asteroide(rand()%73+3,4));
					goToXY((*itA)->getX(),(*itA)->getY());
					printf(" ");
					delete(*itA);
					itA = asteroides.erase(itA);
					
					puntos++;
				}
			}
		}
		if (nave.getVidas() == 0) {
			gameOver = true;
		}
		nave.explosion();
		Sleep(30); // Detiene ejecución del programa por 30 ms
	}
	
	goToXY(35,14);
	printf("GAME OVER");
	goToXY(0,30);
	system("PAUSE");

	return 0;
}