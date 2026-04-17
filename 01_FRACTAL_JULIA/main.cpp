#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include <complex>

#ifdef _WIN32
    #include <windows.h>
#endif

//--dimension  de la imagen
#define WIDTH 1600
#define HEIGTH 900

//--parametros
int max_iteraciones = 10;
double x_min = -1.5;
double x_max = 1.5;
double y_min = -1.0;
double y_max = 1.0;


//variable para numeros complejos
std::complex<double> c(-0.7,0.27015);

//---textura - USADO PARA SFML
uint32_t* pixel_buffer = nullptr;


int main(){
    pixel_buffer = new uint32_t[WIDTH,HEIGTH];

  sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGTH}), "Julia SFML");

#ifdef _WIN32
    HWND hwnd = window.getNativeHandle();
   ShowWindow(hwnd,SW_MAXIMIZE);


#endif
 while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }
 
        // Clear screen
        window.clear();
 
 
        // Update the window
        window.display();
    }

    //elimina la memoria usada
    delete[] pixel_buffer;

    return 0;
}
