package paralela;

import java.nio.Buffer;

import jnr.ffi.LibraryLoader;

public interface FractalDll {
    String LIBRARY_NAME = "lib03_FRACTAL-DLL.dll";// NOMBRE DEL DLL

    // cargar el dll, se hace una instancia de la interfaz y se llama a la funcion
    FractalDll INSTANCE = LibraryLoader.create(FractalDll.class).load(LIBRARY_NAME);// crea un proxy de la interfaz y
                                                                                    // carga el dll

    void julia_simd(
            double x_min, double y_min,
            double x_max, double y_max,
            int width, int height,
            int max_iteraciones, Buffer pixel_buffer);
}
