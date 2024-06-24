#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>

class Laberinto {
public:
    Laberinto(int ancho, int alto);
    void generar();
    void imprimir();
    bool resolver();
    void imprimirSolucion();

private:
    struct Celda {
        bool visitado = false;
        bool arriba = true;
        bool abajo = true;
        bool izquierda = true;
        bool derecha = true;
        bool enSolucion = false;
    };

    int ancho;
    int alto;
    std::vector<std::vector<Celda>> grilla;

    void eliminarPared(Celda& actual, Celda& siguiente, char direccion);
    bool resolverAux(int x, int y);
    void reiniciarVisitados();
};

Laberinto::Laberinto(int ancho, int alto) : ancho(ancho), alto(alto) {
    grilla.resize(alto, std::vector<Celda>(ancho));
    srand(time(nullptr));
}

void Laberinto::generar() {
    std::stack<std::pair<int, int>> pila;
    int inicioX = 0;
    int inicioY = 0;
    grilla[inicioY][inicioX].visitado = true;
    pila.push({inicioX, inicioY});

    while (!pila.empty()) {
        int x = pila.top().first;
        int y = pila.top().second;

        std::vector<std::pair<int, int>> vecinos;

        if (x > 0 && !grilla[y][x - 1].visitado) vecinos.push_back({x - 1, y}); // Izquierda
        if (x < ancho - 1 && !grilla[y][x + 1].visitado) vecinos.push_back({x + 1, y}); // Derecha
        if (y > 0 && !grilla[y - 1][x].visitado) vecinos.push_back({x, y - 1}); // Arriba
        if (y < alto - 1 && !grilla[y + 1][x].visitado) vecinos.push_back({x, y + 1}); // Abajo

        if (!vecinos.empty()) {
            auto [nextX, nextY] = vecinos[rand() % vecinos.size()];

            if (nextX == x - 1) eliminarPared(grilla[y][x], grilla[nextY][nextX], 'I');
            else if (nextX == x + 1) eliminarPared(grilla[y][x], grilla[nextY][nextX], 'D');
            else if (nextY == y - 1) eliminarPared(grilla[y][x], grilla[nextY][nextX], 'A');
            else if (nextY == y + 1) eliminarPared(grilla[y][x], grilla[nextY][nextX], 'B');    
            

            grilla[nextY][nextX].visitado = true;
            pila.push({nextX, nextY});
        } else {
            pila.pop();
        }
    }

    // Asegurar entrada y salida
    grilla[0][0].izquierda = false;  // Entrada
    grilla[alto - 1][ancho - 1].derecha = false;  // Salida

    reiniciarVisitados();  // Resetear el estado de visitado antes de resolver el laberinto
}

void Laberinto::eliminarPared(Celda& actual, Celda& siguiente, char direccion) {
    switch (direccion) {
        case 'I':
            actual.izquierda = false;
            siguiente.derecha = false;
            break;
        case 'D':
            actual.derecha = false;
            siguiente.izquierda = false;
            break;
        case 'A':
            actual.arriba = false;
            siguiente.abajo = false;
            break;
        case 'B':
            actual.abajo = false;
            siguiente.arriba = false;
            break;
    }
}

bool Laberinto::resolver() {
    return resolverAux(0, 0);
}

bool Laberinto::resolverAux(int x, int y) {
    if (x == ancho - 1 && y == alto - 1) {
        grilla[y][x].enSolucion = true;
        return true;
    }

    if (x >= 0 && x < ancho && y >= 0 && y < alto && !grilla[y][x].visitado) {
        grilla[y][x].visitado = true;

        if (!grilla[y][x].arriba && resolverAux(x, y - 1)) {
            grilla[y][x].enSolucion = true;
            return true;
        }
        if (!grilla[y][x].derecha && resolverAux(x + 1, y)) {
            grilla[y][x].enSolucion = true;
            return true;
        }
        if (!grilla[y][x].abajo && resolverAux(x, y + 1)) {
            grilla[y][x].enSolucion = true;
            return true;
        }
        if (!grilla[y][x].izquierda && resolverAux(x - 1, y)) {
            grilla[y][x].enSolucion = true;
            return true;
        }
    }
    return false;
}

void Laberinto::reiniciarVisitados() {
    for (int y = 0; y < alto; ++y) {
        for (int x = 0; x < ancho; ++x) {
            grilla[y][x].visitado = false;
        }
    }
}

void Laberinto::imprimirSolucion() {
    for (int y = 0; y < alto; ++y) {
        // Imprimir paredes superiores
        for (int x = 0; x < ancho; ++x) {
            std::cout << (grilla[y][x].arriba ? "#___" : "#   ");
        }
        std::cout << "#\n";

        // Imprimir paredes izquierdas y celdas
        for (int x = 0; x < ancho; ++x) {
            if (grilla[y][x].izquierda)
                std::cout << "|";
            else
                std::cout << " ";
            if (grilla[y][x].enSolucion)
                std::cout << " . ";
            else
                std::cout << "   ";
        }
        std::cout << "|\n";
    }

    // Imprimir paredes inferiores
    for (int x = 0; x < ancho; ++x) {
        std::cout << "#___";
    }
    std::cout << "#\n";
}

void Laberinto::imprimir() {
    for (int y = 0; y < alto; ++y) {
        // Imprimir paredes superiores
        for (int x = 0; x < ancho; ++x) {
            std::cout << (grilla[y][x].arriba ? "#___" : "#   ");
        }
        std::cout << "#\n";

        // Imprimir paredes izquierdas y celdas
        for (int x = 0; x < ancho; ++x) {
            std::cout << (grilla[y][x].izquierda ? "|   " : "    ");
        }
        std::cout << "|\n";
    }

    // Imprimir paredes inferiores
    for (int x = 0; x < ancho; ++x) {
        std::cout << "#___";
    }
    std::cout << "#\n";
}

int main() {
    int ancho;
    int alto;
    std::cout << "Digite el numero del ancho: ";
    std::cin >> ancho;
    std::cout << "Digite el numero del alto: ";
    std::cin >> alto;

    Laberinto laberinto(ancho, alto);
    laberinto.generar();
    laberinto.imprimir();

    std::cout << " \nLa solucion al laberinto\n";
    
    laberinto.resolver();
    laberinto.imprimirSolucion();

    return 0;
}
