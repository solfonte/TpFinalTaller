#include "../include/player.h"

#define VIDA_MAX 100
#define NIVEL 1
#define PUNTAJE_INICIAL 0
#define CANT_VIDA_MAX 2
#define BALAS_INICIAL 8

Player::Player(const char *rutaimg, SDL_Renderer *render, int id) :
        posicion(70, 70, 0), id(id), arma(render),
        infoJugador(render, VIDA_MAX, NIVEL, PUNTAJE_INICIAL, CANT_VIDA_MAX, BALAS_INICIAL) {

}

void Player::actualizar(int posx, int posy, int vida, float angulo, int idArma,
                        bool disparando, int puntaje, int cantVidas, int balas) {
    this->posicion.actualizar_posicion(posx, posy);
    this->posicion.setAngulo(angulo);
    this->arma.actualizar(idArma, disparando);
    this->infoJugador.actualizarDatosJugador(vida, 1, puntaje, cantVidas, balas);
}

void Player::renderizar() {
    this->arma.renderizar();
    this->infoJugador.renderizar();
}
void Player::actualizarArma(int idArma, bool estado) {
    this->arma.actualizar(idArma, estado);
}
Posicion &Player::getPosicion() {
    return this->posicion;
}

float Player::getAnguloDeVista() {
    return this->posicion.getAnguloDeVista();
}

int Player::getPuntaje() {
    return this->infoJugador.getPuntaje();
}

int Player::getId() {
    return id;
}

void Player::actualizarDatosJugador(int vida, int cant_vidas, int puntaje, int balas) {
    this->infoJugador.actualizarDatosJugador(vida, 1, puntaje, cant_vidas, balas);
}

Player::~Player() {
}
