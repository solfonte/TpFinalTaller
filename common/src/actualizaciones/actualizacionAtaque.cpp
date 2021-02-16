#include "actualizaciones/actualizacionAtaque.h"
#include <iostream>

ActualizacionAtaque::ActualizacionAtaque(Jugador *jugador,
                                         std::map<int, Jugador *> &jugadoresAtacados) :
        jugador(jugador),
        jugadoresAtacados(jugadoresAtacados) {}


ActualizacionAtaque::~ActualizacionAtaque() {}


std::vector<char> ActualizacionAtaque::serializar() {
    std::vector<char> informacion;
    std::vector<char> aux(4);
    aux = numberToCharArray(this->obtenerId());
    informacion.insert(informacion.end(), aux.begin(), aux.end());

    std::vector<char> jugadorSerializado = jugador->serializar();
    aux = numberToCharArray(jugadorSerializado.size());
    informacion.insert(informacion.end(), aux.begin(), aux.end());
    informacion.insert(informacion.end(), jugadorSerializado.begin(), jugadorSerializado.end());

    std::map<int, Jugador *>::iterator it;
    aux = numberToCharArray(this->jugadoresAtacados.size());
    informacion.insert(informacion.end(), aux.begin(), aux.end());
    for (it = jugadoresAtacados.begin(); it != jugadoresAtacados.end(); ++it) {
        Jugador jug = *it->second;
        jugadorSerializado = jug.serializar();
        aux = numberToCharArray(jugadorSerializado.size());
        informacion.insert(informacion.end(), aux.begin(), aux.end());
        informacion.insert(informacion.end(), jugadorSerializado.begin(), jugadorSerializado.end());
    }
    return informacion;
}

void ActualizacionAtaque::deserializar(std::vector<char> &serializado) {
    std::vector<char> sub(4);
    int idx = 0;
    sub = std::vector<char>(&serializado[idx], &serializado[idx + 4]);
    idx += 4;
    std::vector<char> informacionJugador(serializado.begin() + idx,
                                          serializado.begin() + idx +
                                          charArrayToNumber(sub));

    this->jugador = new Jugador();
    this->jugador->deserializar(informacionJugador);
    idx += charArrayToNumber(sub);
    sub = std::vector<char>(&serializado[idx], &serializado[idx + 4]);
    int jugadoresSize = charArrayToNumber(sub);
    std::cerr << "jugadoresSize: " << jugadoresSize << "\n";
    idx += 4;
    for (int i = 0; i < jugadoresSize; i++) {
        sub = std::vector<char>(&serializado[idx], &serializado[idx + 4]);
        idx += 4;
        std::vector<char> jugadorSerializado(serializado.begin() + idx,
                                             serializado.begin() + idx + charArrayToNumber(sub));
        idx += charArrayToNumber(sub);
        Jugador *jugador = new Jugador();
        jugador->deserializar(jugadorSerializado);
        this->jugadoresAtacados.insert(std::make_pair(jugador->getId(), jugador));
    }

}