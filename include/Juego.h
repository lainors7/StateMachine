#include <SFML/Graphics.hpp>
#include <iostream>
//#include "Jugador.h"
//#include "Enemigo.h"
#include "Arma.h"
#include "Hud.h"
#include "Mapa.h"
#include "Consumible.h"
#include "EstadoJuego.h"

//Maquina de estados
#include <memory>
#include <stack>


class EstadoJuego;
using namespace sf;

class Juego
{
    public:
        static Juego* Instance();
        Juego();
        void cargaPlayer();
        void cargaMapa();
        void cargarHUD();
        void gameLoop();

        void procesarEventos();
        void setView();
        void render();

        Vector2f generaPosicion();

        Vector2i dimensiones;

        //Maquina de estados

        void cambioEstado(std::unique_ptr<EstadoJuego> estado);
        void subirEstado(std::unique_ptr<EstadoJuego> estado);
        void popEstado();

void quit();
    private:
        static Juego* pinstance;

        void manejarIA();

        RenderWindow *window;
        RenderStates *states;
        Clock frameClock;
        Jugador *jugador;
        std::vector<Enemigo*> enemigos;
        Event *evento;
        Mapa *mapa;
        Hud *hud;
        Consumible *pocion;
        View view;

        //Maquina de estados
        std::stack<std::unique_ptr<EstadoJuego>> estado;
        bool jugando;

        bool centrado;
        float delta;
        int nEnemigos;
};
