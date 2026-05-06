package paralela;

public class FractalParams {
    public static int max_iteraciones = 10;
    public static final double c_real = -0.7;
    public static final double c_imag = 0.27015;
    public static final int PALETTE_SIZE = 16;
    public static final int WIDTH = 1600;
    public static final int HEIGHT = 900;

    public static final double xMin = -1.5;
    public static final double xMax = 1.5;

    public static final double yMin = -1.0;
    public static final double yMax = 1.0;

    public static final int[] color_ramp = {
            0xFFFF0000, // azul real en ABGR
            0xFFF20000,
            0xFFE60000,
            0xFFD90000,
            0xFFCC0000,
            0xFFBF0000,
            0xFFB30000,
            0xFFA60000,
            0xFF990000,
            0xFF8C0000,
            0xFF800000,
            0xFF730000,
            0xFF660000,
            0xFF590000,
            0xFF4D0000,
            0xFF400000,
            0xFF330000,
            0xFF260000,
            0xFF190000,
            0xFF0D0000,
            0xFF000000
    };
}
