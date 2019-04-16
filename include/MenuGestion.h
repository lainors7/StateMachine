#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include "Menu.h"

class MenuGestion
{
public:
    MenuGestion();
    void update();
    void render();
    void cargador();
    void procesar_eventos();
    void eventos_raton();
private:
    sf::RenderWindow *window;
    sf::Event *evento;
    sf::Mouse *raton;
    bool menuprincipal = true;
    bool sel_raton = false;
    bool sel_teclado = true;
    Menu *menu_inicial;
};
