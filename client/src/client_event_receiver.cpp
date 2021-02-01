#include "../include/client_event_receiver.h"
#include "socket.h"
#include "map.h"
#include "items/item.h"
#include "jugador.h"
#include "estadoJuego.h"

#include <utility>
#include <algorithm>
#include <vector>
#include <map>

ClientEventReceiver::ClientEventReceiver(Socket& socket,
                                         ProtectedQueue<Actualizacion *> &updates, Modelo &modelo, int idJugador) :

        protocolo(std::move(socket)), updates(updates), modelo(modelo), idJugador(idJugador) {}

void ClientEventReceiver::run() {
    while (this->running) {

          std::vector<char> informacion = this->protocolo.recibir();

          EstadoJuego* estadoJuego;
          estadoJuego->deserializar(informacion);
          std::map<int, Jugador*> jugadores = estadoJuego->obtenerJugadores();
          std::map<int, Jugador*>::iterator it;
          Jugador* jugador = jugadores.at(idJugador);
          int vida = jugador->puntos_de_vida();
          int posx = jugador->getPosicion().pixelesEnX();
          int posy = jugador->getPosicion().pixelesEnY();
          int angulo = jugador->getAnguloDeVista();
          int idArma = jugador->getArma()->getId();
          int puntaje = jugador->obtenerLogro().obtener_puntaje();
          bool disparando = jugador->estaDisparando();
          int cantVidas = jugador->puntos_de_vida();
          int balas = jugador->cantidad_balas();
          modelo.actualizarJugador(posx, posy, vida, angulo, idArma,
                                  disparando, puntaje, cantVidas, balas);
          for (it = jugadores.begin(); it != jugadores.end(); it++){
              if (it->first != idJugador){
                    int idE = it->first;
                    int vidaE = it->second->puntos_de_vida();
                    int posxE = it->second->getPosicion().pixelesEnX();
                    int posyE = it->second->getPosicion().pixelesEnY();
                    int idArmaE = idArma;
                    int anguloJugador = angulo;
                    int anguloE = it->second->getAnguloDeVista();
                    bool disparandoE = it->second->estaDisparando();
                    modelo.actualizarEnemigo(idE, vidaE, disparandoE, posxE,
                                              posyE, idArmaE, anguloJugador,
                                              anguloE);
              }
          }

          Map* mapa = estadoJuego->obtenerMapa();
          std::vector<Item*> items = mapa->obtenerItems();
          for (int i = 0; i< items.size(); i++){
              Item* item = items[i];
              int idI = item->getId();
              Type tipo = item->getTipo();
              int posxI = item->obtenerPosicion().pixelesEnX();
              int posyI = item->obtenerPosicion().pixelesEnY();
              modelo.actualizarObjeto(idI, tipo, posxI, posyI);
          }

      }
}

void ClientEventReceiver::cerrar() {
    this->running = false;
}

ClientEventReceiver::~ClientEventReceiver() {
    this->running = false;
    this->join();
}
