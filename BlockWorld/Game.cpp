#include "Game.h"

Camera camera(glm::vec3(0, 50.0f, 0));
ChunkManager cm(10, 7, 10);
World world(cm, Player());

float lastX = 1200 / 2.0f;
float lastY = 800 / 2.0f;
bool firstMouse = true;

double deltaTime = 0.0;
double lastFrame = 0.0;

int RADIUS = 6;
int MIN_RADIUS = 5;
float GRAVITY = -0.055f;

const double FPS = 1.0 / 100.0;
const float movementSpeed = 0.015f;

// Used to check if the world has changed and needs to be rerendered
bool needToUpdateBuffers = false;


void processInput(GLFWwindow* window, World& world)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, movementSpeed, world);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, movementSpeed, world);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, movementSpeed, world);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, movementSpeed, world);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, movementSpeed, world);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, movementSpeed, world);
    }

}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Block destruction
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        needToUpdateBuffers = world.handleBlockDestruction(world.getPlayerPosition(), camera.Front);
    }

    // Block creation
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        std::cout << "User clicked right mouse button \n";
    }
}



GLFWwindow* initGlfwWindow(int width, int height) 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Game", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Error when creating window!";
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Used for displaying the mouse or not
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    // Handles vsync
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);

    return window;
}


std::future<vector<float>> gatherBuffersAsync(ChunkManager& cm, GridCoordinate& chunkPlayerIsIn)
{
    auto gatherBuffers = std::async(std::launch::async, [&cm, &chunkPlayerIsIn] {
            auto buffers =  cm.getBuffersInArea(int(chunkPlayerIsIn.x), int(chunkPlayerIsIn.y), int(chunkPlayerIsIn.z), RADIUS);
            return buffers;
        });

    return gatherBuffers;
}

std::future<vector<float>> gatherChunksAfterBlockDestruction(ChunkManager& cm, GridCoordinate& chunkPlayerIsIn)
{
    auto gatherBuffers = std::async(std::launch::async, [&cm, &chunkPlayerIsIn] {
            auto buffers = cm.getBuffersCurrentlyInMap();
            return buffers;
        });

    return gatherBuffers;
}

Game::Game(std::string vertexShaderPath, std::string fragShaderPath, unsigned int screenWidth, unsigned int screenHeight) :
    window(initGlfwWindow(screenWidth, screenHeight)), fpsWidget(window),
    width(screenWidth), height(screenHeight), shader(vertexShaderPath, fragShaderPath) {}


void Game::loop()
{
    auto chunkBuffers = cm.getBuffersInArea(0, 0, 0, RADIUS);

    VertexArray vao;
    VertexBuffer vb(&chunkBuffers[0], chunkBuffers.size() * sizeof(float));
    BufferLayout layout;

    layout.pushBufferLayout(3, GL_FLOAT, false);
    layout.pushBufferLayout(1, GL_FLOAT, false);

    vao.addBuffer(vb, layout);
    vao.bind();

    shader.use();

    auto chunkPlayerIsIn = glm::vec3(0, 0, 0);
    auto currentChunkPlayerIsIn = glm::vec3(0, 0, 0);

    bool gatheringResources = false;
    std::future<vector<float>> gatherResourcesFuture;

    bool gatheringResourcesAfterBlockDestructionFlag = false;
    std::future<vector<float>> chunksAfterBlockDestructionFuture;

    double lastUpdateTime = 0, lastFrameTime = 0, currentTime = 0;
    int currentFPS = 0;

    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastUpdateTime;
        currentFPS = int(1 / (currentTime - lastFrameTime));

        if ((currentTime - lastFrameTime) >= FPS) {

            // Process the user input and then update their position in the world
            processInput(window, world);
            world.getPlayer().updatePlayerPosition(camera.getPosition());

            // Handle gravity for when player is not on top of a block
            if (!world.playerCollidesOnY())
            {
                camera.changePositionY(float(GRAVITY));
            }

            auto cameraPos = camera.getPosition();
            currentChunkPlayerIsIn = cm.getChunkPlayerIsIn(cameraPos.x, cameraPos.y, cameraPos.z);

            render(chunkBuffers, chunkPlayerIsIn, currentFPS);
            lastFrameTime = currentTime;

            //if (needToUpdateBuffers && !gatheringResourcesAfterBlockDestructionFlag)
            //{

            //    chunksAfterBlockDestructionFuture = gatherChunksAfterBlockDestruction(cm, currentChunkPlayerIsIn);
            //    gatheringResourcesAfterBlockDestructionFlag = true;
            //}

            //if (gatheringResourcesAfterBlockDestructionFlag && chunksAfterBlockDestructionFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
            //{
            //    // chunkBuffers = cm.getBuffersCurrentlyInMap();
            //    chunkBuffers = chunksAfterBlockDestructionFuture.get();

            //    VertexBuffer newVb = VertexBuffer(&chunkBuffers[0], chunkBuffers.size() * sizeof(float));
            //    vao.addBuffer(newVb, layout);

            //    gatheringResourcesAfterBlockDestructionFlag = false;
            //}


            // If the player has moved chunks then we need to render new chunks on the outskirts of the map.
            // This is done by creating a future and setting a flag that says we are currently gathering resources 
            // and should wait for this future to be done before consuming it.

            if (currentChunkPlayerIsIn != chunkPlayerIsIn && !gatheringResources)
            {
                gatherResourcesFuture = gatherBuffersAsync(cm, currentChunkPlayerIsIn);
                gatheringResources = true;
            }

            // Test if the future is ready to consume
            if (gatheringResources && gatherResourcesFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
            {
                chunkBuffers = gatherResourcesFuture.get();
                chunkBuffers.shrink_to_fit();

                VertexBuffer newVb = VertexBuffer(&chunkBuffers[0], chunkBuffers.size() * sizeof(float));
                vao.addBuffer(newVb, layout);

                gatheringResources = false;
            }


            chunkPlayerIsIn = currentChunkPlayerIsIn;
            lastUpdateTime = currentTime;
        }
    }

    glfwTerminate();
}


void Game::render(const vector<float>& chunkBuffers, const GridCoordinate& chunkPlayerIsIn, const unsigned int currentFPS)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set uniforms for the shader program.
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 500.0f);
    shader.setMat4("projection", projection);
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("view", view);

    glDrawArrays(GL_TRIANGLES, 0, GLsizei(chunkBuffers.size() * 4));

    // Render the current FPS to ImGui
    fpsWidget.render(currentFPS, camera.getPosition(), glm::vec3(chunkPlayerIsIn.x, chunkPlayerIsIn.y, chunkPlayerIsIn.z));

    glfwSwapBuffers(window);
    glfwPollEvents();
}


void Game::clearColor(float r, float g, float b, float a) 
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}
