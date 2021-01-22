#ifndef _MANEJADOR_EVENTOS_H
#define _MANEJADOR_EVENTOS_H

#include "thread.h" // porque por un hilo se esta dibujando y por el otro se va manejando los eventos, no es algo secuencial
#include <SDL2/SDL_events.h>
#include <SDL2/SDL.h>
#include "blocking_queue.h"
#include "comandos/comando.h"
#include "comandos/movimiento.h"
class ManejadorEventos: public Thread{
    public:
          ManejadorEventos(int id, BlockingQueue<Comando*>& eventos);
          ~ManejadorEventos();
          void run() override;
          void detectarEventos(SDL_Event& event);
          void stop();
          void crearMovimiento(int direccion);

    private:
        int idJugador;
        BlockingQueue<Comando*>& eventos;
        bool corriendo;

};

#endif