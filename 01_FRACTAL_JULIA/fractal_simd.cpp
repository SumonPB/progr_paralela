#include "fractal_simd.h"

#include <cstring>

//no pueden existir mas de dos funciones
void julia_simd(double x_min, double y_min, double x_max, double y_max, uint32_t width, uint32_t heigt, uint32_t* pixel_buffer){
    std::memset(pixel_buffer,0xFF000000,width*heigt*sizeof(uint32_t));

}


