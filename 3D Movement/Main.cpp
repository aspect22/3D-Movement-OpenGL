// Glew
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Standard
#include <stdio.h>
#include <windows.h>

// Local
#include "shaders.h"
#include "debug.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
GLuint shader_programme;
GLuint shader_programme_pink;
GLuint shader_programme_white;
GLuint vao;
bool debugtoggle = false;


// Items to draw

float o_cube[] = {
    // x     y     z
    // X value is how far left or right
    // Y value is how far up or down
    // Z value is how far forward or back
   -0.50f,  0.50f,  0.0f, // top left
    0.00f,  0.50f,  0.0f, // top right
    0.00f, 0.00f,  0.0f, // bottom right
   -0.50f, 0.00f,  0.0f  // bottom left
};



// TODO: Move this into a seperate file
void compile_Shaders()
{
    printf("Compiling shaders...\n");
    // create buffer
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // create vertex array object
    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Create purple shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &color1, NULL);
    glCompileShader(fs);
    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    // Create pink shader
    GLuint vs_pink = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs_pink, 1, &vertex_shader, NULL);
    glCompileShader(vs_pink);
    GLuint fs_pink = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs_pink, 1, &color2, NULL);
    glCompileShader(fs_pink);
    shader_programme_pink = glCreateProgram();
    glAttachShader(shader_programme_pink, vs_pink);
    glAttachShader(shader_programme_pink, fs_pink);
    glLinkProgram(shader_programme_pink);

    // Create white shader
    GLuint vs_white = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs_white, 1, &vertex_shader, NULL);
    glCompileShader(vs_white);
    GLuint fs_white = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs_white, 1, &color3, NULL);
    glCompileShader(fs_white);
    shader_programme_white = glCreateProgram();
    glAttachShader(shader_programme_white, vs_white);
    glAttachShader(shader_programme_white, fs_white);
    glLinkProgram(shader_programme_white);

    printf("Shaders compiled.\n");
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    double aspectRatio = (double)width / (double)height;

    /*
    if (width >= height)
        gluOrtho2D(-0.5f * aspectRatio, 0.5f * aspectRatio, 0.0f, 1.0f);
    else
        gluOrtho2D(-0.5f, 0.5f, 0.0, 1.0 / aspectRatio);
    gluOrtho2D(-width / 2, width / 2, height / 2, -height / 2);
    */
    printf("Window resized to %d x %d\n", width, height);
    printf("Aspect ratio: %f\n", aspectRatio); 

}

int main() {
    // start GL context and O/S window using the GLFW helper library

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Render", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    // compile the shaders
    compile_Shaders();

    // debug
    CreateThread(NULL, 0, debug, NULL, 0, NULL);

    

    // main loop
    while (!glfwWindowShouldClose(window)) {
        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        switch (debugtoggle)
        {
        case false:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case true:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        default:
            break;
        }
    
        // Draw player1
        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        glBufferData(GL_ARRAY_BUFFER, sizeof(o_cube), o_cube, GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
        // Update other events like input handling 
        glfwPollEvents();
        // Put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
    }
    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}