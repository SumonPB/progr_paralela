#include <fmt/core.h>
#include <vector>
#include <math.h>
#include <mpi.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

void escalaGrisesFuncion(uint32_t *pixelBuffer, int numeroDatos, uint32_t *escalaGrises)
{
    for (int i = 0; i < numeroDatos; i++)
    {
        uint32_t pixel = pixelBuffer[i];
        uint32_t a = (pixel >> 24) & 0xff;
        uint32_t r = (pixel >> 16) & 0xff;
        uint32_t g = (pixel >> 8) & 0xff;
        uint32_t b = (pixel >> 0) & 0xff;

        int gray = (r * 0.21) + (g * 0.72) + (b * 0.07);

        uint32_t nPixel = (a << 24) | (gray << 16) | (gray << 8) | (gray << 0);
        escalaGrises[i] = nPixel;
    }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int nprocs, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int datosProcesar;
    int padded_rows;
    int total;
    uint32_t *pixelbuffer;
    uint32_t *pixelBufferPadded;
    int width, height, channels;
    if (rank == 0)
    {
        
        uint8_t *rgba_pixels =
            stbi_load("img.jpg", &width, &height, &channels, STBI_rgb_alpha);
        channels = 4;
        total = width * height;
        pixelbuffer = new uint32_t[total];
        for (int i = 0; i < total; i++)
        {
            uint32_t r = rgba_pixels[i * 4 + 0];
            uint32_t g = rgba_pixels[i * 4 + 1];
            uint32_t b = rgba_pixels[i * 4 + 2];
            uint32_t a = rgba_pixels[i * 4 + 3];

            uint32_t pixel = (a << 24) | (r << 16) | (g << 8) | (b << 0);

            pixelbuffer[i] = pixel;
        }
        datosProcesar = ceil((total * 1.0) / nprocs);
        padded_rows = datosProcesar * nprocs;
        pixelBufferPadded = new uint32_t[padded_rows]();
        for (int i = 0; i < total; i++)
        {
            pixelBufferPadded[i] = pixelbuffer[i];
        }
    }

    MPI_Bcast(
        &datosProcesar,
        1,
        MPI_INT,
        0,
        MPI_COMM_WORLD);
    MPI_Bcast(
        &padded_rows,
        1,
        MPI_INT,
        0,
        MPI_COMM_WORLD);
    uint32_t *dataTMP = new uint32_t[datosProcesar];
    MPI_Scatter(
        pixelBufferPadded,
        datosProcesar,
        MPI_UINT32_T,
        dataTMP,
        datosProcesar,
        MPI_UINT32_T,
        0,
        MPI_COMM_WORLD);
    uint32_t *grayBufferTmp = new uint32_t[datosProcesar];

    escalaGrisesFuncion(dataTMP, datosProcesar, grayBufferTmp);

    uint32_t *grayBuffer  = nullptr;

    if (rank == 0)
    {
               grayBuffer = new uint32_t[padded_rows];
    }
    

    MPI_Gather(
        grayBufferTmp,
        datosProcesar,
        MPI_UINT32_T,
        grayBuffer,
        datosProcesar,
        MPI_UINT32_T,
        0,
        MPI_COMM_WORLD);
    if (rank == 0)
    {

        stbi_write_png("img-gris.png", width, height, 4,
                       grayBuffer, width*4);
    }

    MPI_Finalize();

    return 0;
}
