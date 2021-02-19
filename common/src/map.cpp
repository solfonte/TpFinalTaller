#include "../include/map.h"
#include <iostream>
#include "items/item.h"
#include "armas/arma.h"
#include "items/comida.h"
#include "items/balas.h"
#include "items/sangre.h"
#include "items/kitsMedicos.h"
#include "items/noItem.h"
#include "items/llave.h"
#include "items/tesoro.h"
#include "armas/canionDeCadena.h"
#include "armas/ametralladora.h"
#include "armas/lanzacohetes.h"
#include <algorithm>

#define PUNTOS_CRUZ 10
#define PUNTOS_COPA 50
#define PUNTOS_COFRE 100
#define PUNTOS_CORONA 200
#define ANGULO_DEFAULT 0

void Map::agregarArma(Posicion &posicion, Arma *arma) {}

void Map::sacarDelMapa(Posicion &posicion) {
    this->contenedorDeElementos.sacarElementoDePosicion(posicion);
}

Map::Map(unsigned rowSize, unsigned colSize, int anchoPantalla) : contenedorDeElementos() {
    if (rowSize < 1 || colSize < 1) {
        throw std::runtime_error("Invalid map");
    }

    this->rowSize = rowSize;
    this->colSize = colSize;
    map.resize(rowSize);
    for (unsigned i = 0; i < rowSize; i++) {
        map[i].resize(colSize, ObjetosJuego::obtenerTipoPorNombre("noItem"));
    }
    setLadoCelda(anchoPantalla);
}

unsigned Map::getRowSize() const {
    return this->rowSize;
}

unsigned Map::getColSize() const {
    return this->colSize;
}

int Map::crearIdValido() {
    int idValido = this->generadorDeId;
    this->generadorDeId++;
    return idValido;
}

void Map::crearElementoPosicionable(const unsigned rowNumber, const unsigned colNumber,
                                    Type value) {
    srand(time(NULL));
    int posElementox = 0;
    int posElementoy = 0;
    if (rowNumber == 0) {
        posElementox = rand() % ladoCelda;
    } else {
        posElementox = (ladoCelda * (rowNumber - 1) + rand() % ladoCelda);
    }
    if (colNumber == 0) {
        posElementoy = rand() % ladoCelda;
    } else {
        posElementoy = ladoCelda * (colNumber - 1) + rand() % ladoCelda;
    }
    std::cerr << "posElementox: " << posElementox << "\n";
    std::cerr << "posElemntoY:  " << posElementoy << "\n";
    std::cerr << "agrego un item\n";
    Posicion posicion = Posicion(posElementox, posElementoy, ANGULO_DEFAULT);
    if (value.getName() == "comida") {
        int idValido = this->crearIdValido();
        std::cerr << "\nid: " << idValido;
        this->contenedorDeElementos.agregarElemento(new Comida(posicion, idValido));
    } else if (value.getName() == "sangre") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos.agregarElemento(new Sangre(posicion, idValido));
    } else if (value.getName() == "kitsMedicos") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos.agregarElemento(new KitsMedicos(posicion, idValido));
    } else if (value.getName() == "balas") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos.agregarElemento(new Balas(posicion, BALAS, idValido));
    } else if (value.getName() == "ametralladora") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos.agregarElemento(new Ametralladora(posicion, idValido));
    } else if (value.getName() == "canionDeCadena") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos.agregarElemento(new CanionDeCadena(posicion, idValido));
    } else if (value.getName() == "lanzaCohetes") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos.agregarElemento(new LanzaCohetes(posicion, idValido));
    } else if (value.getName() == "cruz") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos.agregarElemento(
                new Tesoro(idValido, ObjetosJuego::obtenerTipoPorNombre("cruz"), PUNTOS_CRUZ, posicion));
    } else if (value.getName() == "copa") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos.agregarElemento(
                new Tesoro(idValido, ObjetosJuego::obtenerTipoPorNombre("copa"), PUNTOS_COPA, posicion));
    } else if (value.getName() == "cofre") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos.agregarElemento(
                new Tesoro(idValido, ObjetosJuego::obtenerTipoPorNombre("cofre"), PUNTOS_COFRE, posicion));
    } else if (value.getName() == "corona") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos.agregarElemento(
                new Tesoro(idValido, ObjetosJuego::obtenerTipoPorNombre("corona"), PUNTOS_CORONA, posicion));
    } else if (value.getName() == "llave") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos.agregarElemento(new Llave(posicion, idValido));
    } else {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos.agregarElemento(new NoItem(posicion, idValido));
    }

}

void Map::agregarElemento(Item *item) {
    this->agregarElemento(item);
}

Item *Map::buscarElemento(int &posx, int &posy) {
    return this->contenedorDeElementos.buscarElemento(posx, posy);
}

bool verificarTipo(int tipo) {
    return (tipo != TYPE_WALL_2 && tipo != TYPE_WALL && tipo != TYPE_WALL_3 &&
            tipo != TYPE_DOOR && tipo != TYPE_EMPTY && tipo != TYPE_FAKE_WALL);
}

void Map::setValue(const unsigned rowNumber, const unsigned colNumber, Type value) {
    this->map[rowNumber][colNumber] = value;
    int tipo = value.getType();
    if (verificarTipo(tipo)) {
        this->crearElementoPosicionable(rowNumber, colNumber, value);
    } else if (tipo == TYPE_DOOR || tipo == TYPE_KEY_DOOR) {
        this->aniadirPuerta(rowNumber, colNumber, tipo);
    }
}

void Map::aniadirPuerta(const unsigned rowNumber, const unsigned colNumber, int tipoPuerta) {
    bool necesitaLlave = (tipoPuerta == TYPE_DOOR ? false : true);//documentar
    Posicion pos((colNumber / 2) * this->ladoCelda, (rowNumber / 2) * this->ladoCelda, ANGULO_DEFAULT);
    Puerta puerta(necesitaLlave, pos, rowNumber, colNumber, false);
    this->contenedorDeElementos.aniadirPuerta(puerta);
}

Type &Map::operator()(const unsigned rowNumber, const unsigned colNumber) {
    return this->map[rowNumber][colNumber];
}

Map::~Map() {

}

bool Map::hayPuertas() {
    return this->contenedorDeElementos.hayPuertas();
}

Puerta &Map::puertaMasCercana(Posicion &posicionJugador, double &distancia) {
    return this->contenedorDeElementos.puertaMasCercana(posicionJugador, distancia);

}

std::vector<char> Map::serializar() {
    std::vector<char> informacion;
    std::vector<char> aux(4);
    aux = numberToCharArray(this->rowSize);
    informacion.insert(informacion.end(), aux.begin(), aux.end());

    aux = numberToCharArray(this->colSize);
    informacion.insert(informacion.end(), aux.begin(), aux.end());

    for (unsigned i = 0; i < rowSize; i++) {
        for (unsigned j = 0; j < colSize; j++) {
            aux = numberToCharArray((map[i][j]).getType());
            informacion.insert(informacion.end(), aux.begin(), aux.end());
        }
    }

    std::vector<char> contenedorSerializado = this->contenedorDeElementos.serializar();
    informacion.insert(informacion.end(), contenedorSerializado.begin(), contenedorSerializado.end());
    return informacion;
}

void Map::deserializar(std::vector<char> &serializado) {

    std::vector<char> sub(4);
    int idx = 0;
    sub = std::vector<char>(&serializado[idx], &serializado[idx + 4]);
    this->rowSize = charArrayToNumber(sub);

    idx += 4;
    sub = std::vector<char>(&serializado[idx], &serializado[idx + 4]);
    this->colSize = charArrayToNumber(sub);
    Map newMap(rowSize, colSize, ladoCelda * rowSize);//agregue aca el ancho
    this->map = newMap.map;
    for (unsigned i = 0; i < rowSize; i++) {
        for (unsigned j = 0; j < colSize; j++) {
            idx += 4;
            sub = std::vector<char>(&serializado[idx], &serializado[idx + 4]);
            this->map[i][j] = ObjetosJuego::obtenerTipoPorId(charArrayToNumber(sub));
        }
    }

    idx += 4;
    std::vector<char> contenedorDeElementosSerializado(serializado.begin() + idx, serializado.end());
    std::cerr << "tam srializado " << serializado.size() << "tam idx" << idx << std::endl;
    this->contenedorDeElementos.deserializar(contenedorDeElementosSerializado);
}

std::vector<Item *> &Map::obtenerItems() {
    return this->contenedorDeElementos.obtenerItems();
}

ContenedorDeElementos &Map::obtenerContenedor() {
    return this->contenedorDeElementos;
};

bool Map::hayColision(int fila, int columna) {
    try {
        int tamFila = this->rowSize;
        int tamCol = this->colSize;
        if (fila < 0 || fila > tamFila || columna < 0 || columna > tamCol)
            return false;
        int tipo = this->map[fila][columna].getType();
        if (tipo == TYPE_DOOR) {
            Puerta &puerta = this->contenedorDeElementos.obtenerPuertaEn(fila, columna);
            if (puerta.estaAbierta()) {
                tipo = TYPE_EMPTY;
            }
        }
        return (tipo == TYPE_DOOR || tipo == TYPE_WALL || tipo == TYPE_WALL_2 || tipo == TYPE_WALL_3 ||
                tipo == TYPE_KEY_DOOR || tipo == TYPE_FAKE_WALL);
    } catch (std::exception &exc) {
        return false;//capaz deberia devolver true? no se
    }
}

Posicion Map::obtenerPosicionIncialValida() {
    bool posEsValida = false;
    int posX, posY;
    srand(time(NULL));
    while (!posEsValida) {
        posX = rand() % this->rowSize;
        posY = rand() % this->colSize;
        if (!this->hayColision(posX, posY)) posEsValida = true;
    }
    std::cerr << "rowSize: " << rowSize << "colSize: " << colSize << std::endl;
    if (posX == 0) {
        posX = rand() % this->ladoCelda;
    } else {
        posX = posX * (this->ladoCelda - 1) + rand() % this->ladoCelda;
    }
    if (posY == 0) {
        posY = rand() % this->ladoCelda;
    } else {
        posY = posY * (this->ladoCelda - 1) + rand() % this->ladoCelda;
    }

    Posicion posicion(posX, posY, ANGULO_DEFAULT);
    return posicion;
}

void Map::setLadoCelda(int anchoPantalla) {
    std::cerr << "ancho oantalla: " << anchoPantalla << '\n';
    this->ladoCelda = anchoPantalla / rowSize;
}

int Map::getLadoCelda() {
    return this->ladoCelda;
}

std::vector<std::vector<int>> Map::getMapanumerico() {
    std::vector<std::vector<int>> mapaNumerico;
    for (unsigned i = 0; i < rowSize; i++) {
        std::vector<int> vector;
        for (unsigned j = 0; j < colSize; j++) {
            vector.push_back(map[i][j].getType());
        }
        mapaNumerico.push_back(vector);
    }
    return mapaNumerico;
}
