package paralela;

import org.lwjgl.*;
import org.lwjgl.glfw.*;
import org.lwjgl.opengl.*;


import java.nio.*;

import static org.lwjgl.glfw.Callbacks.*;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;

import static org.lwjgl.system.MemoryUtil.*;

public class fractalMain {

    // windos handle
    private int textureID;
    private long window;

    private IntBuffer pixelBuffer;

    FractalCpu fractalCpu;
    FractalSimd fractalSimd;
    FractalHilos fractalHilos;
    FPSCounter fpsCounter;
    int mode = 1; // 1 cpu, 2 simd

    public fractalMain() {
        fractalCpu = new FractalCpu();
        fractalHilos = new FractalHilos();
        fractalSimd = new FractalSimd();
        
        fpsCounter = new FPSCounter();
        pixelBuffer = BufferUtils.createIntBuffer(FractalParams.WIDTH * FractalParams.HEIGHT);
    }

    public void run() {
        System.out.println("Fractal Julia " + Version.getVersion());

        init();
        loop();

        glfwFreeCallbacks(window);
        glfwDestroyWindow(window);

        glfwTerminate();
        glfwSetErrorCallback(null).free();
    }

    private void init() {

        GLFWErrorCallback.createPrint(System.err).set();

        if (!glfwInit())
            throw new IllegalStateException("Unable to initialize GLFW");

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        // Create the window
        window = glfwCreateWindow(FractalParams.WIDTH, FractalParams.HEIGHT, "Julia Set", 0, 0);
        if (window == NULL)
            throw new RuntimeException("Failed to create the GLFW window");

        // manejo de eventos
        glfwSetKeyCallback(window, (window, key, scancode, action, mods) -> {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
                glfwSetWindowShouldClose(window, true);
            if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
                FractalParams.max_iteraciones += 10;
            }
            if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
                FractalParams.max_iteraciones -= 10;
                if (FractalParams.max_iteraciones < 0)
                    FractalParams.max_iteraciones = 10;
            }
            if (key == GLFW_KEY_1 && action == GLFW_RELEASE) {
                System.out.println("CPU mode");
                mode = 1;
            }
            if (key == GLFW_KEY_2 && action == GLFW_RELEASE) {
                System.out.println("Modo C++ SIMD mode");
                mode = 2;
            }
            if (key == GLFW_KEY_3 && action == GLFW_RELEASE) {
                System.out.println("Modo hilos");
                mode = 3;
            }
            
        });
        GLFWVidMode vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(window,
                (vidmode.width() - FractalParams.WIDTH) / 2,
                (vidmode.height() - FractalParams.HEIGHT) / 2);

        // Make the OpenGL context current
        glfwMakeContextCurrent(window);

        GL.createCapabilities();
        GL.createCapabilitiesWGL(); // usado para windows

        // ----version de OpenGL
        String version = GL11.glGetString(GL11.GL_VERSION);
        String vendor = GL11.glGetString(GL11.GL_VENDOR);
        String renderer = GL11.glGetString(GL11.GL_RENDERER);

        System.out.println("OpenGL version: " + version);
        System.out.println("OpenGL vendor: " + vendor);
        System.out.println("OpenGL renderer: " + renderer);

        // --conf
        GL11.glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1, 1, -1, 1, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_TEXTURE_2D);
        glLoadIdentity();

        // Enable v-sync
        glfwSwapInterval(1);

        // Make the window visible
        glfwShowWindow(window);

        setupTexture();

    }

    private void setupTexture() {
        textureID = glGenTextures();
        glBindTexture(GL_TEXTURE_2D, textureID);

        // RESERVA LA MEMORIA UNICAMENTE
        glTexImage2D(
                GL_TEXTURE_2D, 0,
                GL_RGBA8,
                FractalParams.WIDTH,
                FractalParams.HEIGHT,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    }

    private void loop() {

        GL.createCapabilities();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            paint();

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
    }

    private void paint() {
        //cambiar al modo 3
        int fps = fpsCounter.update();
        System.out.println(fps);
        pixelBuffer.clear();

        if (mode == 1) {
            fractalCpu.julia_serial_2(FractalParams.xMin, FractalParams.yMin, FractalParams.xMax, FractalParams.yMax,
                    FractalParams.WIDTH, FractalParams.HEIGHT);
            pixelBuffer.put(fractalCpu.pixel_buffer);

        } else if (mode == 2) {
            fractalSimd.JuliaSimd();
            pixelBuffer.put(fractalSimd.pixel_buffer.asIntBuffer());
          //cambiar a FractalHilos  
        } else if (mode == 3) {
            try {
                fractalHilos.generarFractalParalelo(FractalParams.xMin, FractalParams.yMin, FractalParams.xMax, FractalParams.yMax,
                        FractalParams.WIDTH, FractalParams.HEIGHT);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            pixelBuffer.put(fractalHilos.pixel_buffer);
        }

        pixelBuffer.flip();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0,
                GL_RGBA8,
                FractalParams.WIDTH,
                FractalParams.HEIGHT,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                pixelBuffer);

        glBegin(GL_QUADS);
        {
            glTexCoord2d(0.0f, 0.0f);
            glVertex2d(-1, -1);

            glTexCoord2d(0.0f, 1f);
            glVertex2d(-1, 1);

            glTexCoord2d(1f, 1f);
            glVertex2d(1, 1);

            glTexCoord2d(1f, 0f);
            glVertex2d(1, -1);
        }
        glEnd();
    }

    // native la funcion es de otro lenguaje y se llama desde java, en este caso es
    // de c++ con simd, en este caso un dll

    public static void main(String[] args) {
        new fractalMain().run();
    }

}