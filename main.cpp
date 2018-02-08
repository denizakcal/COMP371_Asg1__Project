#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
//#include "Objloader.hpp"

using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;
const GLfloat CAMERA_MOVEMENT_STEP = 0.08;

GLfloat rot;
glm::vec3 camera_position;
glm::vec3 scale;


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        camera_position.z += CAMERA_MOVEMENT_STEP;

    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        camera_position.z -= CAMERA_MOVEMENT_STEP;

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        camera_position.x -= CAMERA_MOVEMENT_STEP;

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        camera_position.x += CAMERA_MOVEMENT_STEP;

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        camera_position.y -= CAMERA_MOVEMENT_STEP;

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        camera_position.y += CAMERA_MOVEMENT_STEP;

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        rot += 10.0f;

    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        rot -= 10.0f;

    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


}

// The MAIN function, from here we start the application and run the game loop
int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);




    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Horse", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }


    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );

    glfwMakeContextCurrent(window);
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);


    // Build and compile our shader program
    // Vertex shader

    // Read the Vertex Shader code from the file
    string vertex_shader_path = "vertex.shader";
    string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_shader_path, ios::in);

    if (VertexShaderStream.is_open()) {
        string Line = "";
        while (getline(VertexShaderStream, Line))
        VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
        getchar();
        exit(-1);
    }

    // Read the Fragment Shader code from the file
    string fragment_shader_path = "fragment.shader";
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

    if (FragmentShaderStream.is_open()) {
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
        FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Are you in the right directory?\n", fragment_shader_path.c_str());
        getchar();
        exit(-1);
    }
    //Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader); //free up memory
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    //----------------------------------------------------------------------------------------------------------

    GLfloat xloc;

    GLfloat yloc;

    GLfloat zloc;

    vector <glm::vec3> vert;

    vector <glm::uvec3> ind;

    for (int i = 0; i <= 100; i++){
        for (int j = 0; j <= 100; j++){

            xloc = float(j)/float(100);

            zloc = float(i)/float(100);

            vert.push_back(glm::vec3(xloc,yloc,zloc));

        }
    }

    for (int k = 0; k <= 100; k++){
        for (int l = 0; l <= 100; l++){

            int ind1 = k * (100 + 1);
            int ind2 = (l+1) * (100 + 1);

            ind.push_back(glm::uvec3(ind1+l, ind1+l+1, ind2+l+1));
            ind.push_back(glm::uvec3(ind1+l, ind2+l+1, ind2+l));
        }
    }




    vector<glm::vec3> vertCube =
    {
        // Cube
        // Front face triangle 1
        glm::vec3(0.5,-0.5,0.5),
        glm::vec3(-0.5,0.5,0.5),
        glm::vec3(-0.5,-0.5,0.5),

        // Front face triangle 2
        glm::vec3(0.5,-0.5,0.5),
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(-0.5,0.5,0.5),

        // Back face triangle 1
        glm::vec3(0.5,-0.5,-0.5),
        glm::vec3(-0.5,0.5,-0.5),
        glm::vec3(-0.5,-0.5,-0.5),

        // Back face triangle 2
        glm::vec3(0.5,-0.5,-0.5),
        glm::vec3(0.5,0.5,-0.5),
        glm::vec3(-0.5,0.5,-0.5),

        // Left face triangle 1
        glm::vec3(-0.5,-0.5,0.5),
        glm::vec3(-0.5,0.5,-0.5),
        glm::vec3(-0.5,-0.5,-0.5),

        // Left face triangle 2
        glm::vec3(-0.5,-0.5,0.5),
        glm::vec3(-0.5,0.5,0.5),
        glm::vec3(-0.5,0.5,-0.5),

        // Right face triangle 1
        glm::vec3(0.5,-0.5,-0.5),
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(0.5,-0.5,0.5),

        // Right face triangle 2
        glm::vec3(0.5,-0.5,-0.5),
        glm::vec3(0.5,0.5,-0.5),
        glm::vec3(0.5,0.5,0.5),

        // Top face triangle 1
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(-0.5,0.5,-0.5),
        glm::vec3(-0.5,0.5,0.5),

        // Top face triangle 2
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(0.5,0.5,-0.5),
        glm::vec3(-0.5,0.5,-0.5),

        // Bottom face triangle 1
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(-0.5,-0.5,-0.5),
        glm::vec3(-0.5,-0.5,0.5),

        // Bottom face triangle 2
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(0.5,-0.5,-0.5),
        glm::vec3(-0.5,-0.5,-0.5),
    };



    //------ MODEL MATRIX ---------
    //glm::mat4 mm;
    //glm::mat4 scale;
    //glm::mat4 translate;
    //glm::mat4 rotate;

    //------ VIEW MATRIX ---------

    //view_matrix = glm::lookAt(c_pos, c_pos + c_dir, c_up);

    //------ PROJECTION MATRIX -------
    //glm::mat4 pm = glm::perspective(45.f, 800.f / 600.f, 0.1f, 100.f);

    //GLuint mm_addr = glGetUniformLocation(shaderProgram, "model_matrix");
    //GLuint vm_addr = glGetUniformLocation(shaderProgram, "view_matrix");
    //GLuint pm_addr = glGetUniformLocation(shaderProgram, "projection_matrix");
    //glUniformMatrix4fv(mm_addr, 1, false, glm::value_ptr(mm));
    //glUniformMatrix4fv(pm_addr, 1, false, glm::value_ptr(pm));


    GLuint VAO, VBO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    //glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


    glBufferData(GL_ARRAY_BUFFER, vertCube.size() * sizeof(glm::vec3), &vertCube.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(glm::uvec3), &ind.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

    scale = glm::vec3(1.0f);

    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection_matrix");
    GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "view_matrix");
    GLuint transformLoc = glGetUniformLocation(shaderProgram, "model_matrix");


    // Game loop


    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);



        glm::mat4 view_matrix;
        view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, -3.0f));
        view_matrix = translate(view_matrix, camera_position);
        //view_matrix = glm::lookAt(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 model_matrix;
        model_matrix = glm::scale(model_matrix, glm::vec3(1.0f));
        model_matrix = glm::rotate(model_matrix, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
        //model_matrix = glm::rotate(model_matrix, (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
        glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));


        //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 12*3);


        glBindVertexArray(0);


        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}
