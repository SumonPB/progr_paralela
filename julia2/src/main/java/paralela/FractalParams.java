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

    public static final int[] color_ramp1 = {
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
public static final int[] color_ramp2 = {
    0xFF0000FF, // Rojo (Pure Red)
    0xFF0040FF, // Naranja rojizo
    0xFF0080FF, // Naranja
    0xFF00BFFF, // Amarillo anaranjado
    0xFF00FFFF, // Amarillo
    0xFF00FFBF, // Lima amarillenta
    0xFF00FF80, // Lima
    0xFF00FF40, // Verde claro
    0xFF00FF00, // Verde puro
    0xFF40FF00, // Verde azulado
    0xFF80FF00, // Esmeralda
    0xFFBFFF00, // Turquesa
    0xFFFFFF00, // Cyan puro
    0xFFFFBF00, // Celeste
    0xFFFF8000, // Azul claro
    0xFFFF4000, // Azul vibrante
    0xFFFF0000, // Azul puro (Royal Blue)
    0xFFFF0040, // Violeta azulado
    0xFFFF0080, // Violeta
    0xFFFF00BF, // Púrpura
    0xFFFF00FF  // Magenta / Morado
};
}
