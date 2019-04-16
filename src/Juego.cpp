#include "../include/Juego.h"
#include "../include/Astar.h"
#include "../include/Posicion.h"
#include "../include/EstadoJuego.h"
#include <vector>
#include <iostream>

using namespace sf;
using namespace std;

Juego* Juego::pinstance = 0;

Juego* Juego::Instance(){
    if(pinstance == 0)
        pinstance = new Juego;
    return pinstance;
}

Juego::Juego(){
    srand(time(0));
    dimensiones = Vector2i(1280, 720);
    nEnemigos = 5;

    window = new RenderWindow(VideoMode(dimensiones.x, dimensiones.y), "Incorpore");
    window->setFramerateLimit(60);

    evento = new Event;

    view.setSize(dimensiones.x, dimensiones.y);
    view.zoom(0.2f);

    centrado = false;
    jugando = true;

    cargaMapa();
    cargaPlayer();
    cargarHUD();
    gameLoop();
}

void Juego::cargaPlayer(){

    jugador = new Jugador(mapa->generaPosicion());

    for(int i = 0; i < nEnemigos; i++){
        enemigos.push_back(new Enemigo(mapa->generaPosicion()));
    }

    view.setCenter(jugador->getActual()->sprite.getPosition().x, jugador->getActual()->sprite.getPosition().y);
}



void Juego::cargaMapa(){
    mapa = new Mapa;
    pocion = new Consumible(mapa->generaPosicion());
}

void Juego::cargarHUD(){
    hud = new Hud();
    hud->setPosicionVida(view.getCenter().x - dimensiones.x/10 + 2, view.getCenter().y - dimensiones.y/10 + 2);
    hud->setPosicionHabilidades(view.getCenter().x - hud->getPiezaHabilidades().getGlobalBounds().width/2, view.getCenter().y + dimensiones.y/10 - hud->getPiezaHabilidades().getGlobalBounds().height);
}

void Juego::gameLoop(){
    sf::Clock frameClock;
    sf::Event a_evento;
    sf::Time tiempo;

    while(window->isOpen() && jugando){
        procesarEventos();
        delta = frameClock.restart().asSeconds();
        jugador->update(delta, *window, mapa->getNumColisiones(), mapa->getBounds());
        hud->compruebaTeclas();
        manejarIA();
        if(jugador->getVidas() < 10 && pocion->isConsumible()){
           if(pocion->consume(jugador->getEntidadHitbox())){
                hud->modificar_vida(1,1);
           }
           //delete pocion;
        }

        if(enemigos.size() > 0)
        {

            for(int i = 0; i < enemigos.size(); i++) {

                if(enemigos.at(i)->getBorrado() == false){
                  //  std::cout << enemigos.size() << std::endl;
                    enemigos.at(i)->update(delta, *window, mapa->getNumColisiones(), mapa->getBounds(), Posicion(mapa->getPosicionEntidad(*enemigos.at(i)).x, mapa->getPosicionEntidad(*enemigos.at(i)).y), jugador->getAtaqueHitbox());

                    if(jugador->recibeDmg(enemigos.at(i)->getEntidadHitbox(), enemigos.at(i)->getVida())){
                        hud->modificar_vida(1,2);
                    }

                } else {
                    delete enemigos.at(i);
                    enemigos.erase(enemigos.begin()+i);
                }
            }
        }

        if(!centrado) {
            setView();
            centrado = true;
        }

        setView();
        render();
    }
}

void Juego::procesarEventos(){
    while(window->pollEvent(*evento)){
        switch(evento->type){
            case sf::Event::Closed:

                jugando=false;

                break;
            case sf::Event::MouseButtonPressed:
                if(evento->mouseButton.button == Mouse::Left) {
                    //cout << "HE PULSADO EL BOTÓN"<< endl;
                    jugador->getArma().atacar(0, enemigos, enemigos.size());
                }
                break;
            default: break;

        }
    }
}

void Juego::setView(){
    view.move(jugador->getMovement() * delta);
    hud->move(jugador->getMovement() * delta);
}

void Juego::render(){
    window->clear(Color(28,17,23,255));
    window->setView(view);

    mapa->draw(*window, *jugador, enemigos, enemigos.size());

    pocion->draw(*window);
    hud->draw(*window);
    jugador->drawBoundingBoxes(*window);
    /*
    for(int i = 0; i < enemigos.size(); i++){
        if(enemigos.at(i)->getBorrado() == false)
        enemigos.at(i)->draw(*window);
    }

    for(int i = 0; i < enemigos.size(); i++){
        if(enemigos.at(i)->getBorrado() == false)
        enemigos.at(i)->drawBoundingBoxes(*window);
    }*/

    window->display();
}

void Juego::manejarIA(){

    if(true){   // anyadir clock para que se ejecute cada X tiempo

        Vector2i v = mapa->getPosicionEntidad(*jugador);
        Posicion pos_jugador = Posicion(v.x, v.y);

        // para cada enemigo crear Posicion y llamar a la IA
        // nEnemigos: numero de enemigos en el mapa

        for(int a = 0; a < enemigos.size(); a++){
        if(enemigos.at(a)->getBorrado() == false)
        {
            Vector2i e = mapa->getPosicionEntidad(*enemigos.at(a));
            Posicion pos_enemigo = Posicion(e.x, e.y);
            //asignar posicion al enemigo
            //se crea el astar
            Astar *ia = new Astar(pos_jugador, pos_enemigo, mapa->getColisiones(), mapa->getHeight(), mapa->getWidth());

            //se llama a astar.mapear()

            std::vector<Posicion> path = ia->mapear();

            ////std::cout << path.size() << std::endl;

            enemigos.at(a)->setPath(path);
            delete ia;

            // SEGUIR EL CAMINO (CON BUCLE INTERPOLADO)
            // mover a enemigos[a] hacia el siguiente punto
            /*
            int flag = true;
            for(int a = 0; a < mapa->getHeight(); a++)
            {
                //std::cout << "  ";
                for(int b = 0; b < mapa->getWidth(); b++)
                {
                    flag = true;
                    for(int c = 0; c < path.size()-1; c++)
                    {
                        if(path.at(c).getX() == b && path.at(c).getY() == a)
                        {
                            //std::cout << "-";
                            flag = false;
                            break;
                        }
                    }
                    if(flag && v.x == b && v.y == a)
                    {
                        //std::cout << "#";
                        flag = false;
                    }
                    if(flag && e.x == b && e.y == a)
                    {
                        //std::cout << "@";
                        flag = false;
                    }
                    if(flag)
                    {
                        if(mapa->getColisiones()[a][b] == 1)
                        {
                            //std::cout << "1";
                        }
                        else
                        {
                            //std::cout << "0";
                        }
                    }
                }
                //std::cout << std::endl;
            }
            */
        }

        } //fin for each enemigo
    }


}

 //Manejo de estados

 //Pop al estado actual y subimos el siguiente
    void Juego::cambioEstado(unique_ptr<EstadoJuego> estados){

        if(!estado.empty()){
            estado.pop();
        }

        subirEstado(std::move(estados));

}

//Subir el estado del juego a la pila

void Juego::subirEstado(unique_ptr<EstadoJuego> estados)
{
    if (!estado.empty())
        estado.top()->pause();

    // GameStates start paused and must be resumed
    estados->resume();

    estado.push(std::move(estados));
}

//Pop del estado fuera de la pila

void Juego::popEstado()
{
    if (!estado.empty())
    {
        estado.pop();
        estado.top()->resume(); // Del estado anterior
    }
}
//Quitar juego, preparamos para salir
void Juego::quit(){

    jugando=false;

}
