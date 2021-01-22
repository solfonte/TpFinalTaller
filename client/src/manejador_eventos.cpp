#include "../include/manejador_eventos.h"
#include "../include/player.h"

#define MOV_ABAJO -2
#define MOV_ARRIBA 2
#define MOV_DERECHA 1
#define MOV_IZQUIERDA -1
#include <iostream>

ManejadorEventos::ManejadorEventos(int idJugador, BlockingQueue<Comando*>& eventos):
    idJugador(idJugador),
    eventos(eventos),
    corriendo(true){}

ManejadorEventos::~ManejadorEventos(){}

void ManejadorEventos::run(){
    SDL_Event evento;
    while(this->corriendo){

        if (SDL_PollEvent(&evento)){;
              detectarEventos(evento);
          }
          std::chrono::milliseconds duration(1000);
          std::this_thread::sleep_for(duration);
    }
}

void ManejadorEventos::crearMovimiento(int direccion){
    Comando* movimiento = new Movimiento(this->idJugador, direccion);
    this->eventos.push(movimiento);
}
void ManejadorEventos::detectarEventos(SDL_Event& evento){
      switch (evento.type) {
          case SDL_KEYDOWN:
              std::cout << "tecla preionada\n";
              switch( evento.key.keysym.sym ){
                  // aca mandariamos la informacion o crearimos el evento;
                      case SDLK_LEFT:         // x, y, vida, angulo;

                          crearMovimiento(MOV_IZQUIERDA);
                        //  player.settear_estado(-1, 0, 100, 50); // esto es para probar que se cambia el estado
                                                                // y se renderiza la imagen;
                            std::cout << "me muevo a izquierda\n";
                          break;
                      case SDLK_RIGHT:
                          crearMovimiento(MOV_DERECHA);
                        //  player.settear_estado(1, 0, 100, 50);
                        std::cout << "me muevo a derecha\n";
                          break;
                      case SDLK_UP:
                          crearMovimiento(MOV_ARRIBA);
                        //  player.settear_estado(0, 1, 100, 50);
                        std::cout << "me muevo arriba\n";
                          break;
                      case SDLK_DOWN:
                          crearMovimiento(MOV_ABAJO);
                        //  player.settear_estado(0, -1, 100, 50);
                        std::cout << "me muevo abajo\n";
                          break;
              }
              break;
          case SDL_KEYUP:
              std::cout << "tecla despresionada\n";
              break;
          case SDL_QUIT:
              this->corriendo = false;
              break;
          default:
              break;
      }
}

void ManejadorEventos::stop(){
    this->corriendo = false;
}