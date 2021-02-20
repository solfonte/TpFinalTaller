#include "../include/servidor.h"
#include "../include/aceptador.h"

#define TERMINAR 'q'

Servidor::Servidor(Parser &parser) : parser(parser) {}

void Servidor::correr() {
    std::string port = parser.obtenerPuerto();
    this->socket.bind_and_listen(port.c_str());
    Aceptador aceptador(this->socket, parser.obtenerRutaMapas(), parser.obtenerMapas());
    aceptador.start();

<<<<<<< HEAD
Servidor::~Servidor() {}

void Servidor::procesar_comandos(ProtectedQueue<Comando *> &cola_comandos, EstadoJuego &estadoJuego) {
    bool termine = false;
    while (!termine) {
        try {
            Comando *comando = cola_comandos.obtener_dato();
            std::cerr << " proceso comando " << std::endl;
            std::vector<Actualizacion *> actualizaciones = comando->ejecutar(estadoJuego);
            // puede ser una lista de actualizaciones;
            // actualizacion partivulasr -> item comsumido(efecto, id, posicion, id jugador, pos jugador);
            delete comando;
            std::cerr << "enviar" << std::endl;
            this->enviar_actualizaciones(actualizaciones);
        } catch (const std::exception &exception) {
            termine = true;
        }
    }
}

void Servidor::agregarCliente(std::string &nombreJugador, ManejadorCliente *cliente, int &id) {

    id = this->obtenerIdParaJugador();
    this->estadoJuego.agregarJugador(nombreJugador, id);
    cliente->settearId(id);
    this->clientes.insert({id, cliente});
    this->cantJugadoresAgregados++;
    if (this->cantJugadoresAgregados == this->cantJugadoresPosibles) {
        this->arrancoPartida = true;
        this->start();
    }

}

int Servidor::obtenerIdParaJugador() {
    int id = this->generadorDeId;
    this->generadorDeId += 1;
    return id;
}

void Servidor::lanzarJugadores() {
    for (auto it = this->clientes.begin(); it != this->clientes.end(); ++it) {
        it->second->run();
    }
}
=======
    char caracter;
    do {
        std::cin >> caracter;
    } while (caracter != TERMINAR);
>>>>>>> 0fc9e59f0849ab6be23f77097005aecaf12a9b57

    this->socket.cerrar();
    aceptador.join();
}

<<<<<<< HEAD

void Servidor::enviar_actualizaciones(std::vector<Actualizacion *> actualizaciones) {
    //serializa y manda por sockets a cada jugador
    //Actualizacion *actualizacion = new Actualizacion(this->estadoJuego);
    // mandar una actualizaion en particular;
    std::cerr << " envio act " << std::endl;
    std::map<int, ManejadorCliente *>::iterator it;
    for (it = this->clientes.begin(); it != this->clientes.end(); ++it) {
        it->second->enviar_actualizaciones(actualizaciones);
    }
}

void Servidor::generarComandosLua(JugadorLua& jugadorLua, ProtectedQueue<Comando *> &cola_comandos){
    Comando* nuevoComando;
    char teclaComando = jugadorLua.procesar();
    switch(teclaComando){
        case 'w':
            nuevoComando = new Movimiento(jugadorLua.id, static_cast<Accion>(ARRIBA));
            std::cerr << "=== SE MUEVE PARA ARRIBA LUA==== " << std::endl;
            break;
        case 'd':
            nuevoComando = new Movimiento(jugadorLua.id, static_cast<Accion>(ROTAR_DERECHA));
            std::cerr << "=== ROTA A DERECHA LUA==== " << std::endl;
            break;
        case 's':
            nuevoComando = new Movimiento(jugadorLua.id, static_cast<Accion>(ABAJO));
            std::cerr << "=== SE MUEVE ABAJO LUA==== " << std::endl;
            break;
        case 'a':
            nuevoComando = new Movimiento(jugadorLua.id, static_cast<Accion>(ROTAR_IZQUIERDA));
            std::cerr << "=== ROTA A IZQUIERDA LUA==== " << std::endl;
            break;
        case 'p':
            nuevoComando = new Ataque(jugadorLua.id);
            std::cerr << "=== ATACA LUA==== " << std::endl;
            break;
        default:
            nuevoComando = new Movimiento(jugadorLua.id, static_cast<Accion>(ROTAR_DERECHA));
            std::cerr << "=== mov default LUA==== " << std::endl;
            break;
    }
    this->cola_comandos.aniadir_dato(nuevoComando);
}
void Servidor::verificarClientes(){
    std::map<int, ManejadorCliente *>::iterator it;
    while (it != this->clientes.end()){
      std::cerr << "entre al loop\n";
        if(it->second->termino()){
          std::cerr << "alguno de los hilos termino\n";
           it->second->cerrar();
           it->second->join();
           delete it->second;
           it = this->clientes.erase(it);
        }
    }
    if (this->clientes.empty()){
        this->sigue_corriendo = false;
    }
}
void Servidor::run() {
    std::cerr << "=== CREO JUGADOR LUA==== " << std::endl;
    std::string ruta("modulo.lua");

    JugadorLua jugadorLua(this->estadoJuego, ID_LUA, ruta);
    std::string nombre("IA");
    jugadorLua.instanciarJugador(nombre);

    this->lanzarJugadores();
    this->lanzarContadorTiempoPartida();
    std::vector<Actualizacion *> actualizaciones;
    actualizaciones.push_back(new ActualizacionInicioPartida(this->estadoJuego));
    this->enviar_actualizaciones(actualizaciones);

    //std::chrono::milliseconds duration(TIEMPO_SERVIDOR);
    //std::this_thread::sleep_for(duration);
    std::chrono::duration<double> tiempoServidor(TIEMPO_SERVIDOR);
    while (this->sigue_corriendo) {
        //el while va a depender del obtener comandos con un try catch
        //deberia haber un obtener comandos pero como lo tiene de atributo por ahora no
        try {
            auto inicio = std::chrono::high_resolution_clock::now();
            std::cerr << "=== GENERO COMANDOS LUA==== " << std::endl;
            generarComandosLua(jugadorLua, this->cola_comandos);
            //std::cerr << "proceso" << std::endl;
            procesar_comandos(this->cola_comandos, this->estadoJuego);
            this->actualizarContador();
            if (this->estadoJuego.terminoPartida()) {
                std::vector<Actualizacion *> actualizacionTermino;
                Actualizacion *terminoPartida = new ActualizacionTerminoPartida(this->estadoJuego);
                actualizacionTermino.push_back(terminoPartida);
                this->enviar_actualizaciones(actualizacionTermino);
                this->arrancoPartida = false;
                this->sigue_corriendo = false;
            }
            //this->verificarClientes();
            auto fin = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> sleepTime = tiempoServidor - (fin - inicio);
            //std::cerr << "sleep for" << time_span.count() << std::endl;
            std::this_thread::sleep_for(sleepTime);

            /*auto fin = std::chrono::high_resolution_clock::now();
            auto delta = fin - inicio;
            long tardanza = delta.count();
            if (tardanza >= TIEMPO_SERVIDOR) {
                tardanza = TIEMPO_SERVIDOR;
            }
            std::cerr << "sleep for" << TIEMPO_SERVIDOR-tardanza <<std::endl;
            std::chrono::milliseconds duration(TIEMPO_SERVIDOR - tardanza);
            std::this_thread::sleep_for(duration);
*/
        } catch (...) {
            this->sigue_corriendo = false;

        }


    }
    //mostramos el ranking, podemos mandar una actualizacion con los jugadores que ganaron
    //  this->sigue_corriendo = false; creo que no va esta linea
}

std::vector<char> Servidor::serializar() {
    std::vector<char> informacion;
    std::vector<char> cantJugadoresAct = numberToCharArray(this->cantJugadoresAgregados);
    informacion.insert(informacion.end(), cantJugadoresAct.begin(), cantJugadoresAct.end());
    std::vector<char> cantJugadores = numberToCharArray(this->cantJugadoresPosibles);
    informacion.insert(informacion.end(), cantJugadores.begin(), cantJugadores.end());
    return informacion;
}

void Servidor::joinClientes() {
    std::map<int, ManejadorCliente *>::iterator it;
    for (it = this->clientes.begin(); it != this->clientes.end(); ++it) {
        it->second->join();
    }
}
=======
Servidor::~Servidor() {}
>>>>>>> 0fc9e59f0849ab6be23f77097005aecaf12a9b57
