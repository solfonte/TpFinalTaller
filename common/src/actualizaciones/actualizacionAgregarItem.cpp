#include "../include/actualizaciones/actualizacionAgregarItem.h"
#include <iostream>
#define PUNTOS_CRUZ 10
#define PUNTOS_COPA 50
#define PUNTOS_COFRE 100
#define PUNTOS_CORONA 200

ActualizacionAgregarItem::ActualizacionAgregarItem(Item* item): item(item){}

ActualizacionAgregarItem::~ActualizacionAgregarItem(){}

std::vector<char> ActualizacionAgregarItem::serializar() {
    std::vector<char> informacion;
    std::vector<char> aux(4);
    aux = numberToCharArray(this->obtenerId());
    informacion.insert(informacion.end(), aux.begin(), aux.end());

    std::vector<char> itemSerializado = item->serializar();
    aux = numberToCharArray(itemSerializado.size());
    informacion.insert(informacion.end(), aux.begin(), aux.end());
    informacion.insert(informacion.end(), itemSerializado.begin(), itemSerializado.end());

    return informacion;
}

Item* ActualizacionAgregarItem::deserializarItem(std::vector<char> &informacion) {
    std::vector<char> sub(4);
    int idx = 0;
    sub = std::vector<char>(&informacion[idx], &informacion[idx + 4]);
    int id = charArrayToNumber(sub);
    idx += 4;
    sub = std::vector<char>(&informacion[idx], &informacion[idx + 4]);
    int idTipo = charArrayToNumber(sub);
    idx +=4;
    std::cerr << "deserializo item\n";
    std::cerr << "tipoid: " << idTipo << "\n";
    Posicion posicion;
    std::vector<char> posicionSerializado(informacion.begin() + idx,
                                          informacion.end());
    posicion.deserializar(posicionSerializado);
    if (idTipo == ObjetosJuego::obtenerTipoPorNombre("balas").getType()) {
        return new Balas(posicion, 5, id);
    } else if (idTipo == ObjetosJuego::obtenerTipoPorNombre("comida").getType()) {
        return new Comida(posicion, id);
    } else if (idTipo == ObjetosJuego::obtenerTipoPorNombre("kitsMedicos").getType()) {
        return new KitsMedicos(posicion, id);
    } else if (idTipo == ObjetosJuego::obtenerTipoPorNombre("llave").getType()) {
        return new Llave(posicion, id);
    } else if (idTipo == ObjetosJuego::obtenerTipoPorNombre("sangre").getType()) {
        return new Sangre(posicion, id);
    } else if (idTipo == ObjetosJuego::obtenerTipoPorNombre("cruz").getType()) {
        return new Tesoro(id, ObjetosJuego::obtenerTipoPorNombre("cruz"), PUNTOS_CRUZ, posicion);
    } else if (idTipo == ObjetosJuego::obtenerTipoPorNombre("copa").getType()) {
        return new Tesoro(id, ObjetosJuego::obtenerTipoPorNombre("copa"), PUNTOS_COPA, posicion);
    } else if (idTipo == ObjetosJuego::obtenerTipoPorNombre("cofre").getType()) {
        return new Tesoro(id, ObjetosJuego::obtenerTipoPorNombre("cofre"), PUNTOS_COFRE, posicion);
    } else if (idTipo == ObjetosJuego::obtenerTipoPorNombre("corona").getType()) {
        return new Tesoro(id, ObjetosJuego::obtenerTipoPorNombre("corona"), PUNTOS_CORONA, posicion);
    }
    return new NoItem(posicion, idTipo);
}

void ActualizacionAgregarItem::deserializar(std::vector<char> &serializado) {
    std::vector<char> sub(4);
    int idx = 0;
    sub = std::vector<char>(&serializado[idx], &serializado[idx + 4]);
    std::vector<char> itemSerializado(serializado.begin() + idx,
                                      serializado.begin() + idx + charArrayToNumber(sub));
    this->item = deserializarItem(itemSerializado);
}