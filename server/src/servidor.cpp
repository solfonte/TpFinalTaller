#include "../include/servidor.h"
#include <iostream>
#include <exception>
#include "actualizacion.h"
#include "jugador.h"

// en si recibe un archivo yaml y luego sereializa;
Servidor::Servidor(ProtectedQueue<Comando*> &cola_comandos,ProtectedQueue<Actualizacion>& actualizaciones, Map &mapa, int cant_jugadores) :
        cola_comandos(cola_comandos),
        cola_actualizaciones(actualizaciones),
        jugadores(),
        estadoJuego(mapa),
        cant_jugadores(cant_jugadores),
        sigue_corriendo(true),
        arrancoPartida(false){}

Servidor::~Servidor() {
  std::map<int, Cliente*>::iterator it;
  for (it = this->jugadores.begin(); it != this->jugadores.end(); ++it){
    delete it->second;
  }
}

void Servidor::procesar_comandos(ProtectedQueue<Comando*> &cola_comandos, EstadoJuego &estadoJuego){
    bool termine = false;
    while (!termine) {
        try {
            Comando *comando = cola_comandos.obtener_dato(); // el comando va a tener quien le envio lo que tiene que hacer, osea el id
            comando->ejecutar(estadoJuego);
            Actualizacion actualizacion(estadoJuego);
            this->cola_actualizaciones.aniadir_dato(actualizacion);
            delete comando;
        } catch (const std::exception &exception) {
            termine = true;
        }
    }
}
void Servidor::agregarCliente(std::string& nombreJugador, Cliente* cliente){
      // asignarle un id random
      // el mapa deveria crear al jugador o hay que avisarle que hay un nuevo jugador
      // para asignarle posicion;
      int id = 111;
      //Jugador jugador(nombreJugador, id);
      this->estadoJuego.agregarJugador(nombreJugador, id);
      this->jugadores.insert(std::make_pair(id,cliente));
      if (this->jugadores.size() == this->cant_jugadores){
            this->arrancoPartida = true;
            this->start();
      }


}
void Servidor::lanzarJugadores(){

    for (auto it = this->jugadores.begin(); it != this->jugadores.end(); it++){
          it->second->start();
    }

}

void Servidor::lanzarContadorTiempoPartida(){}

bool Servidor::yaArranco(){
    return this->arrancoPartida;
}
bool Servidor::terminoPartida(){
    return !(this->sigue_corriendo);
}
ProtectedQueue<Comando*>& Servidor::obtenerColaEventos(){
    return this->cola_comandos;
}
//servidor->deberia llamarse JuegoServer y despues le cambiamos a Juego
// servidor es partida
void Servidor::run(){

    this->lanzarJugadores();
    this->lanzarContadorTiempoPartida();


    bool termine = false;
    while (!termine) {
        //el while va a depender del obtener comandos con un try catch
        //deberia haber un obtener comandos pero como lo tiene de atributo por ahora no

        procesar_comandos(this->cola_comandos, this->estadoJuego);//devolveria actualizaciones
        //enviar_actualizaciones(cola de actualizaciones);
        //std::chrono::milliseconds duration(10);
        //std::this_thread::sleep_for(duration);
        termine = true;
    }
}
