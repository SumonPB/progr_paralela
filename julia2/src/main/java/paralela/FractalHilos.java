package paralela;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class FractalHilos {
    public int[] pixel_buffer;

    public FractalHilos() {
        pixel_buffer = new int[FractalParams.WIDTH * FractalParams.HEIGHT];
    }

    int acotado_2(double x, double y) {
        /*
         * dados: c, z0
         * Zn+1 = Zn^2 + c
         */

        int iter = 1;

        double zr = x;
        double zi = y;

        while (iter < FractalParams.max_iteraciones && (zr * zr + zi * zi) < 4.0) {
            double dr = zr * zr - zi * zi + FractalParams.c_real;
            double di = 2.0 * zr * zi + FractalParams.c_imag;
            zr = dr;
            zi = di;

            iter++;

        }
        if (iter < FractalParams.max_iteraciones) {
            // nomras > 2
            int index = iter % FractalParams.PALETTE_SIZE;
            return FractalParams.color_ramp2[index];
        }

        // los bits esta alreves en cuanto a los colores
        return 0xFF000000; // negro

    }

    public void rangos(double x_min, double y_min, double x_max, double y_max, int width, int height, int yinicio,
            int yfinal) {

        double dx = (x_max - x_min) / width;
        double dy = (y_max - y_min) / height;

        for (int j = yinicio; j < yfinal; j++) {
            for (int i = 0; i < width; i++) {

                // z = x+yi = (x,y)
                double x = x_min + i * dx;
                double y = y_max - j * dy;

                // std::complex<double> z(x, y);

                // similar al var
                var color = acotado_2(x, y);

                // index j*w + i
                pixel_buffer[j * width + i] = color;
            }
        }

    }

    public void generarFractalParalelo(
            double x_min,
            double y_min,
            double x_max,
            double y_max,
            int width,
            int height) throws InterruptedException {

        // obtiene cantidad de núcleos/hilos disponibles
        int numHilos = Runtime.getRuntime().availableProcessors();

        ExecutorService executor = Executors.newFixedThreadPool(numHilos);

        int filasPorHilo = height / numHilos;

        for (int t = 0; t < numHilos; t++) {

            int inicio = t * filasPorHilo;

            int fin;

            if (t == numHilos - 1) {
                fin = height;
            } else {
                fin = inicio + filasPorHilo;

            }
            System.out.println("================================");
            System.out.println("Hilo " + t + " procesará filas de " + inicio + " a " + fin);
            executor.execute(() -> {

                rangos(
                        x_min,
                        y_min,
                        x_max,
                        y_max,
                        width,
                        height,
                        inicio,
                        fin);

            });
        }
        // no aceptar más tareas
        executor.shutdown();

        // esperar a que terminen
        executor.awaitTermination(
                Long.MAX_VALUE,
                TimeUnit.NANOSECONDS);

    }
}
