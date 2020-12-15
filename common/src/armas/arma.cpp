#include "armas/arma.h"

int Arma::posicionAtacable(Posicion &atacante, Posicion &otroJugador) {
    float distancia_jugador = atacante.distanciaA(otroJugador);
    if (atacante.intersectaConMiAngulo(otroJugador) && distancia_jugador <= this->distancia_max_atacable) {
        return distancia_jugador;
    } else {
        return -1;
    }
}

int Arma::JugadorAMenorDistancia(Jugador *jugador, std::map<int, Jugador *> &jugadores) {
    std::map<int, Jugador *>::iterator it;
    int menorDistancia = this->distancia_max_atacable;
    int distanciaActual = this->distancia_max_atacable;
    int idJugadorMasCercano = NO_HAY_JUGADOR_CERCANO;
    for (it = jugadores.begin(); it != jugadores.end(); ++it) {
        if (jugador->getId() != it->second->getId()) {
            distanciaActual = posicionAtacable(jugador->getPosicion(), it->second->getPosicion());
            if (distanciaActual < menorDistancia) {
                menorDistancia = distanciaActual;
                idJugadorMasCercano = it->first;
                // consultar lo de que mata mas al costado o en frente
            }
        }
    }
    return idJugadorMasCercano;
}