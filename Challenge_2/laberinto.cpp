#include <iostream>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <queue>

using namespace std;

// Definición de una estructura para las celdas del laberinto
struct Celda {
    int x, y;
    Celda(int x, int y) : x(x), y(y) {}
};

// Función para imprimir el laberinto
void imprimirLaberinto(int** matriz, int filas, int columnas) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (matriz[i][j] == 1) {
                char a = 178; // Carácter para paredes
                cout << a << a; // Imprimir pared
            } else if (matriz[i][j] == 2) {
                cout << "**"; // Imprimir camino correcto
            } else {
                cout << "  "; // Imprimir espacio vacío
            }
        }
        cout << "\n"; // Nueva línea para cada fila
    }
}

// Función para verificar si existe un camino desde la entrada hasta la salida y marcarlo
bool hayCamino(int** matriz, int filas, int columnas, Celda entrada, Celda salida) {
    vector<vector<bool>> visitado(filas, vector<bool>(columnas, false));
    queue<Celda> q;
    q.push(entrada);
    visitado[entrada.y][entrada.x] = true;

    int dx[] = { 1, -1, 0, 0 };
    int dy[] = { 0, 0, 1, -1 };
    vector<vector<Celda>> prev(filas, vector<Celda>(columnas, Celda(-1, -1)));

    bool caminoEncontrado = false;

    while (!q.empty()) {
        Celda actual = q.front();
        q.pop();

        if (actual.x == salida.x && actual.y == salida.y) {
            caminoEncontrado = true;
            // Marcar el camino correcto en la matriz
            Celda temp = actual;
            while (temp.x != -1 && temp.y != -1) {
                matriz[temp.y][temp.x] = 2; // Marcar el camino
                temp = prev[temp.y][temp.x];
            }
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nx = actual.x + dx[i];
            int ny = actual.y + dy[i];

            if (nx >= 0 && nx < columnas && ny >= 0 && ny < filas && !visitado[ny][nx] && matriz[ny][nx] == 0) {
                visitado[ny][nx] = true;
                q.push(Celda(nx, ny));
                prev[ny][nx] = actual;
            }
        }
    }

    return caminoEncontrado;
}

// Función para liberar la memoria asignada para la matriz
void liberarMatriz(int** matriz, int filas) {
    for (int i = 0; i < filas; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;
}

// Función para generar un laberinto utilizando backtracking
void generarLaberintoBacktracking(int** matriz, int x, int y, int filas, int columnas) {
    int dx[] = { 2, -2, 0, 0 };
    int dy[] = { 0, 0, 2, -2 };

    // Mezclar las direcciones para generar caminos aleatorios
    for (int i = 0; i < 4; ++i) {
        int r = rand() % 4;
        swap(dx[i], dx[r]);
        swap(dy[i], dy[r]);
    }

    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx > 0 && nx < columnas - 1 && ny > 0 && ny < filas - 1 && matriz[ny][nx] == 1) {
            int wx = x + dx[i] / 2;
            int wy = y + dy[i] / 2;

            matriz[ny][nx] = 0;
            matriz[wy][wx] = 0;

            generarLaberintoBacktracking(matriz, nx, ny, filas, columnas);
        }
    }
}

// Función para generar un laberinto
int** generarLaberinto(int filas, int columnas, Celda &entrada, Celda &salida) {
    int **matriz = nullptr;

    bool caminoEncontrado = false;
    while (!caminoEncontrado) {
        // Crear e inicializar la matriz
        if (matriz != nullptr) {
            liberarMatriz(matriz, filas);
        }

        matriz = new int *[filas];
        for (int i = 0; i < filas; i++) {
            matriz[i] = new int[columnas];
            for (int j = 0; j < columnas; j++) {
                matriz[i][j] = 1; // Inicializar todas las celdas como paredes
            }
        }

        // Punto inicial
        matriz[1][1] = 0;
        generarLaberintoBacktracking(matriz, 1, 1, filas, columnas);

        // Crear entrada y salida en extremos opuestos
        int ladoEntrada = rand() % 4;
        int ladoSalida = (ladoEntrada + 2) % 4; // Lado opuesto al lado de la entrada

        switch (ladoEntrada) {
            case 0: entrada = Celda(1, 0); break; // Top
            case 1: entrada = Celda(columnas - 1, rand() % (filas - 2) + 1); break; // Right
            case 2: entrada = Celda(rand() % (columnas - 2) + 1, filas - 1); break; // Bottom
            case 3: entrada = Celda(0, rand() % (filas - 2) + 1); break; // Left
        }

        switch (ladoSalida) {
            case 0: salida = Celda(1, 0); break; // Top
            case 1: salida = Celda(columnas - 1, rand() % (filas - 2) + 1); break; // Right
            case 2: salida = Celda(rand() % (columnas - 2) + 1, filas - 1); break; // Bottom
            case 3: salida = Celda(0, rand() % (filas - 2) + 1); break; // Left
        }

        matriz[entrada.y][entrada.x] = 0; // Marcar entrada
        matriz[salida.y][salida.x] = 0; // Marcar salida

        // Verificar si hay un camino
        caminoEncontrado = hayCamino(matriz, filas, columnas, entrada, salida);
    }

    return matriz;
}

// Pruebas unitarias
void pruebasUnitarias() {
    int filas = 15;
    int columnas = 15;

    Celda entrada(1, 0);
    Celda salida(columnas - 2, filas - 1);

    int** laberinto = generarLaberinto(filas, columnas, entrada, salida);

    // Imprimir el laberinto generado
    imprimirLaberinto(laberinto, filas, columnas);

    liberarMatriz(laberinto, filas);
}

int main() {
    srand(time(NULL)); // Inicializar el generador de números aleatorios
    pruebasUnitarias();
    return 0;
}
