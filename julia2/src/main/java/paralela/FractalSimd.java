package paralela;

import java.nio.ByteBuffer;

public class FractalSimd {
    ByteBuffer pixel_buffer;

    FractalSimd() {
        pixel_buffer = ByteBuffer.allocateDirect(FractalParams.WIDTH * FractalParams.HEIGHT * 4);
    }

    public void JuliaSimd() {
        FractalDll.INSTANCE.julia_simd(FractalParams.xMin, FractalParams.yMin,
                FractalParams.xMax, FractalParams.yMax,
                FractalParams.WIDTH, FractalParams.HEIGHT,
                FractalParams.max_iteraciones,
                pixel_buffer);
    }

}
