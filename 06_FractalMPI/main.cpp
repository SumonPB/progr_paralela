#include <iostream>
// Parameteros
#include <complex>
#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include "fractal_mpi.h"
#include "mpi.h"

#ifdef _WIN32
#include <windows.h>
#endif

int max_iteraciones = 10;
double x_min = -1.5;
double x_max = 1.5;
double y_min = -1.0;
double y_max = 1.0;
uint32_t *pixel_buffer = nullptr;
uint32_t *texture_buffer = nullptr;
int running = 1;
// Complejo que almacena dobles
std::complex<double> c(-0.71, 0.27015);

// Pamarametro img
#define ANCHO 1600
#define ALTO 900

void setup_ui()
{
    texture_buffer = new uint32_t[ANCHO * ALTO];
    std::memset(texture_buffer, 0, ANCHO * ALTO * sizeof(uint32_t));
    // inicializar la ui
    //sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode({ANCHO, ALTO}), "Fractal MPI");
#ifdef _WIN32
    HWND hwnd = window.getNativeHandle();
    ShowWindow(hwnd, SW_MAXIMIZE); // Maximizar Ventana
#endif
    sf::Texture texture({ANCHO, ALTO});
    sf::Sprite sprite(texture);
    // textos
    sf::Font font("arial.ttf");
    sf::Text text(font, "Julia Set", 24);
    text.setFillColor(sf::Color::White);
    text.setPosition({10, 10});
    text.setStyle(sf::Text::Bold);
    // fps
    int frames = 0;
    int fps = 0;
    sf::Clock clock;

    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {

            // Close window: exit
            if (event->is<sf::Event::Closed>())
            {
  
                running = 0;
                window.close();
            }

            else if (event->is<sf::Event::KeyReleased>())
            {
                auto evt = event->getIf<sf::Event::KeyReleased>();

                switch (evt->scancode)
                {
                case sf::Keyboard::Scan::Up:
                    max_iteraciones += 10;
                    break;
                case sf::Keyboard::Scan::Down:
                    max_iteraciones -= 10;
                    if (max_iteraciones < 10)
                        max_iteraciones = 10;
                    break;
                }
                std::memset(texture_buffer, 0, ANCHO * ALTO * sizeof(uint32_t));
            }
        }

                              //notificar a los otros ransk que la app se esta cerrando
            MPI_Bcast(&running, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (running==0)
        {
            break;
        }

        // Dibuja Fractal dependera de la velocidad
        //texture.update((const uint8_t *)pixel_buffer);

        frames++;

        if (clock.getElapsedTime().asSeconds() >= 1.0f)
        {
            fps = frames;
            frames = 0;
            clock.restart();
        }
        // auto msg = fmt::format("julia: iteraciones; {}.fps{}, Mode: {}", max_iteraciones, fps, mode);
        // text.setString(msg);

        // Clear screen
        window.clear();
        {
            //window.draw(sprite);
            // window.draw(text);
            // window.draw(textoptions);
        }

        // Update the window
        window.display();
    }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int nprocs, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /*
    r0: start = 0*400 = 0, end = 0*400+400 = 400
    r1: start = 1*400 = 400, end = 1*400+400 = 800
    r2: start = 2*400 = 800, end = 2*400+400 = 1200
    r3: start = 3*400 = 1200, end = 3*400+400 = 1600

    */
    int delta = std::ceil(1.0 * ALTO / nprocs);
    int rows_start = rank * delta;
    int rows_end = rows_start + delta;

    int padding = delta * nprocs - ALTO; // filas de padding necesarias para que cada proceso tenga el mismo número de filas

    if (rows_end > ALTO)
    {
        rows_end = ALTO;
    }

    pixel_buffer = new uint32_t[ANCHO * delta]; // cada proceso solo necesita almacenar su bloque de filas, con padding incluido
    std::memset(pixel_buffer, 0, ANCHO * delta * sizeof(uint32_t));
    fmt::print("Rank {}: rows {} to {}\n", rank, rows_start, rows_end);

    if (rank == 0)
    {
        setup_ui();
    }
    else
    {
        while (true)
        {
        MPI_Bcast(&running, 1, MPI_INT, 0, MPI_COMM_WORLD); // recibir notificación de cierre
        if (running == 0)
        {
            fmt::print("Rank {}: received shutdown signal, exiting...\n", rank);
            break;
        }
        //julia_mpi(x_min, y_min, x_max, y_max, ANCHO, ALTO, rows_start, rows_end, pixel_buffer);
        
        }

        
    }

    MPI_Finalize();
    return 0;
}