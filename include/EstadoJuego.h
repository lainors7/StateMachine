#ifndef ESTADOJUEGO_H
#define ESTADOJUEGO_H
#include "Juego.h"

class EstadoJuego
{
    public:
        EstadoJuego(Juego& a_juego);
        virtual ~EstadoJuego();

        //Maquina de estados
        //Tiempo de vida de las funciones
        virtual void pause() = 0;
        virtual void resume() = 0;

    protected:
        Juego& juego;

    private:
};

#endif // ESTADOJUEGO_H
