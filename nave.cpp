#include <stdio.h>
#include <windows.h>
#include <conio.h>

void goToXY(int x, int y) {
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	
	SetConsoleCursorPosition(hCon,dwPos);
}

int main() {
	int x = 10;
	int y = 8;
	goToXY(x,y);
	printf("*");
	
	bool gameOver = false;
	while(!gameOver) {
		if(kbhit()) {
			char tecla = getch(); // Devuelve la tecla presionada
			
			goToXY(x,y);
			printf(" "); // Borrar asterisco
			
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
				default:
					break;
			}
			
			goToXY(x,y);
			printf("*"); // Pinta después de presionar la tecla
		}
		
		Sleep(30); // Detiene ejecución del programa por 30 ms
	}

	return 0;
}