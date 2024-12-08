#include <GLAD/glad.h>
#include <glfw/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "resources/camera.h"
#include <iostream>
#include "render/ShaderProgram.h"
#include <sstream>
#include <fstream>
#include "resources/ResourceManager.h"
#include "render/texture2D.h"
#include "resources/Model.h"
#include "resources/MyFormat.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window, Render::ShaderProgram& program);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

// camera
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraFixed = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 lightPos = glm::vec3(0.3f, 1.f, 2.3f);
glm::vec3 cameraUpDown = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 lightColor = glm::vec3(1.f);

GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

GLfloat planeVertices[] = {
    -5.f, 3.f, 0.f,
    5.f, 3.f, 0.f,
    -5.f - 3.f, 0.f,
    5.f, -3.f, 0.f
};
GLfloat planeIndices[] = {
    0.f, 1.f, 2.f,
    1.f, 3.f, 2.f
};

GLfloat vertices[] = {
    -0.8f, -0.5f, -0.5f,
     0.8f, -0.5f, -0.5f,
     0.8f,  0.5f, -0.5f,
     0.8f,  0.5f, -0.5f,
    -0.8f,  0.5f, -0.5f,
    -0.8f, -0.5f, -0.5f,

    -0.8f, -0.5f,  0.5f,
     0.8f, -0.5f,  0.5f,
     0.8f,  0.5f,  0.5f,
     0.8f,  0.5f,  0.5f,
    -0.8f,  0.5f,  0.5f,
    -0.8f, -0.5f,  0.5f,

    -0.8f,  0.5f,  0.5f,
    -0.8f,  0.5f, -0.5f,
    -0.8f, -0.5f, -0.5f,
    -0.8f, -0.5f, -0.5f,
    -0.8f, -0.5f,  0.5f,
    -0.8f,  0.5f,  0.5f,

     0.8f,  0.5f,  0.5f,
     0.8f,  0.5f, -0.5f,
     0.8f, -0.5f, -0.5f,
     0.8f, -0.5f, -0.5f,
     0.8f, -0.5f,  0.5f,
     0.8f,  0.5f,  0.5f,

    -0.8f, -0.5f, -0.5f,
     0.8f, -0.5f, -0.5f,
     0.8f, -0.5f,  0.5f,
     0.8f, -0.5f,  0.5f,
    -0.8f, -0.5f,  0.5f,
    -0.8f, -0.5f, -0.5f,

    -0.8f,  0.5f, -0.5f,
     0.8f,  0.5f, -0.5f,
     0.8f,  0.5f,  0.5f,
     0.8f,  0.5f,  0.5f,
    -0.8f,  0.5f,  0.5f,
    -0.8f,  0.5f, -0.5f,
};


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    std::string lightVertex = "D:/cpp/1/3DModel/res/shader/lightVertex.txt";

    std::string lightFragment = "D:/cpp/1/3DModel/res/shader/lightFragment.txt";

    Render::ShaderProgram lightProg(lightVertex, lightFragment);


    lightProg.use();

    GLuint lightVBO = 0;
    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    
    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);



    // build and compile shaders
    // -------------------------
    std::string vertex = "D:/cpp/1/3DModel/res/shader/vertex.txt";

    std::string fragment = "D:/cpp/1/3DModel/res/shader/fragment.txt";

    Render::ShaderProgram ourShader(vertex, fragment);

  //  MyFormat format("D:/cpp/1/openProject/res/model/backpack.obj", "D:/cpp/1/openProject/res/format/format.dwu");
    Model ourModel("D:/cpp/1/openProject/res/format/format.dwu");

    // load models
    // 
    // 
    // -----------
  //  Model ourModel("D:/cpp/1/openProject/res/model/backpack.obj");

  
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    glClearColor(0.1f, 0.2f, 0.2f, 0.2f);
    while (!glfwWindowShouldClose(window))
    {
        glfwSetCursorPosCallback(window, mouseCallback);
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window, ourShader);

        // render
        // ------

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        ourShader.setVec3("pointLight.position", lightPos);
        ourShader.setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
        ourShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);

        // view/projection transformations
        glm::mat4 projection = glm::mat4(1.f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setVec3("viewPos", cameraPos);
        ourShader.setVec3("lightColor", lightColor);
        ourShader.setVec3("objectColor", 1.f, 0.5f, 0.31f);
        GLint modelLoc = ourShader.uniformLoc("model");
        GLint viewLoc = ourShader.uniformLoc("view");
        GLint projectionLoc = ourShader.uniformLoc("projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
       ourModel.Draw(ourShader);


        lightProg.use();
        projection = glm::mat4(1.f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        modelLoc = lightProg.uniformLoc("model");
        viewLoc = lightProg.uniformLoc("view");
        projectionLoc = lightProg.uniformLoc("projection");
        lightProg.setVec3("lightColor", lightColor);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

void processInput(GLFWwindow* window, Render::ShaderProgram& program)
{
    GLfloat cameraSpeed = 0.3f;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        lightPos += cameraSpeed * cameraFixed;

    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        lightPos -= cameraSpeed * cameraFixed;

    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        lightPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));

    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        lightPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));

    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        lightPos += cameraSpeed * cameraUpDown;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        lightPos -= cameraSpeed * cameraUpDown;
    }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    front.y = 0;
    cameraFixed = glm::normalize(front);
    front.x = 0;
    front.z = 0;
    front.y = glm::sin(glm::radians(pitch));
    cameraUpDown = front;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------


