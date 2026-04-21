#include <fmt/core.h>
#include <SFML/Graphics.hpp>

#include <complex>
#include "fractal_serial.h"

#ifdef _WIN32
    #include <windows.h>
#endif

// Pamarametro img
#define ANCHO 1600
#define ALTO 900

//Parameteros
int max_iteraciones = 10;
double x_min = -1.5;
double x_max = 1.5;
double y_min = -1.0;
double y_max = 1.0;

//Complejo que almacena dobles
std::complex<double> c(-0.71, 0.27015);

//textura (uint32_t tipo de dato sin signo de 16/32/64 etc)
uint32_t* pixel_buffer = nullptr;
// uint16_t* pixel_buffer = nullptr;

int main(){

    // en un solo vector puede representar todo la img ya sea por filas o columnas
    pixel_buffer = new uint32_t[ANCHO*ALTO];

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(sf::VideoMode({ANCHO, ALTO}), "Julia");

#ifdef _WIN32
    HWND hwnd = window.getNativeHandle();
    ShowWindow(hwnd, SW_MAXIMIZE); // Maximizar Ventana
#endif

        sf::Texture texture({ANCHO, ALTO});
        sf::Sprite sprite(texture);

        sf::Font font("arial.ttf");
        sf::Text text(font,"Julia Set",24);
        text.setFillColor(sf::Color::White);
        text.setPosition ({10,10});
        text.setStyle(sf::Text::Bold);

        //FPS
        int frames = 0;
        int fps = 0;
        sf::Clock clock;

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (event -> is < sf::Event::KeyReleased>() ){
                auto evt =event ->getIf <sf::Event::KeyReleased>();

                switch(evt -> scancode){
                    case sf::Keyboard::Scan::Up:
                        max_iteraciones += 10;
                        break;
                    case sf::Keyboard::Scan::Down:
                        max_iteraciones -= 10;
                        if(max_iteraciones <10) max_iteraciones = 10;
                        break;
                }
            }
        }

        // Dibuja Fractal dependera de la velocidad
        julia_serial_2(x_min,y_min,x_max,y_max, ANCHO, ALTO,pixel_buffer);
        texture.update((const uint8_t *)pixel_buffer);
        

        frames++;

        if(clock.getElapsedTime().asSeconds()>= 1.0f){
            fps = frames;
            frames = 0;
            clock.restart();
        }
        auto msg = fmt::format("julia: iteraciones; {}.fps{}", max_iteraciones,fps);
        text.setString(msg);


        
        // Clear screen
        window.clear();
        {
            window.draw(sprite);
            window.draw(text);
        }

        // Update the window
        window.display();
    }

    delete[] pixel_buffer;
    return 0;
}
