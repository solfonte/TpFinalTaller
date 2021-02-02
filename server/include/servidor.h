#ifndef _SERVIDOR_H
#define _SERVIDOR_H

#include <map>
#include "thread.h"
#include "protected_queue.h"
#include "actualizacion.h"
#include "comandos/comando.h"
#include "map.h"
#include "estadoJuego.h"
#include "jugador.h"
#include "old_cliente.h"
#include "iserializable.h"

class Servidor : public Thread, public ISerializable {
public:
    Servidor(/*ProtectedQueue<Comando*> &cola_comandos,ProtectedQueue<Actualizacion>& actualizaciones,*/Map *mapa,
                                                                                                        int cant_jugadores);

    ~Servidor();

    void run() override;

    void agregarCliente(std::string &nombreJugador, Cliente *cliente);

    bool yaArranco();

    bool terminoPartida();

    ProtectedQueue<Comando *> &obtenerColaEventos();

    ProtectedQueue<Actualizacion> &obtenerColaActualizaciones();

    void lanzarJugadores();

    void lanzarContadorTiempoPartida();

    void borrarClientes();

    std::vector<char> serializar() {
        std::vector<char> informacion;
        informacion.push_back((char) jugadores.size()); //jugadores metidos
        informacion.push_back((char) cant_jugadores); // cantidad total disponible
        return informacion;
    }

    void deserializar(std::vector<char>& serializado) {
    }

    void enviar_actualizaciones(ProtectedQueue<Actualizacion> &actualizaciones);

    void actualizarContador();

private:
    void procesar_comandos(ProtectedQueue<Comando *> &cola_comandos, EstadoJuego &estadoJuego);

    ProtectedQueue<Comando *> cola_comandos;
    ProtectedQueue<Actualizacion> cola_actualizaciones;
    std::map<int, Cliente *> jugadores;
    EstadoJuego estadoJuego;
    int cant_jugadores;
    bool sigue_corriendo;
    bool arrancoPartida;

};

#endif
