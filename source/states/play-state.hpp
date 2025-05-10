#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/free-movement.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include <systems/collision-system.hpp>
#include <unordered_map>
#include <json/json.hpp>

#include <iomanip>
#include <sstream>

#define GAME_COUNTER_TIME 90.0f

// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;

    our::FreeMovementSystem freeMovement;
    our::CollisionSystem collisionSystem;

    float countdownTime = GAME_COUNTER_TIME;
    bool isGameOver = false;
    our::TexturedMaterial* heartMaterial = nullptr;
    our::TexturedMaterial* timerMaterial = nullptr;
    std::unordered_map<char, our::Texture2D*> digitTextures;
    our::Mesh* quadMesh = nullptr;
    int lives = 3;
    int level = 0;
    const int MAX_LEVELS = 3;

    bool softReset = false;

    void onInitialize() override {
        std::cout<<"lives = "<<lives;
        std::cout<<"Initializing Play!\n";
        heartMaterial = new our::TexturedMaterial();
        heartMaterial->shader = new our::ShaderProgram();
        heartMaterial->shader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
        heartMaterial->shader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
        heartMaterial->shader->link();
        heartMaterial->texture = our::texture_utils::loadImage("assets/textures/heart.png");
        heartMaterial->sampler = new our::Sampler();
        heartMaterial->sampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        heartMaterial->sampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        heartMaterial->tint = glm::vec4(1, 1, 1, 1);

        heartMaterial->pipelineState.blending.enabled = true;
        heartMaterial->pipelineState.blending.sourceFactor = GL_SRC_ALPHA;
        heartMaterial->pipelineState.blending.destinationFactor = GL_ONE_MINUS_SRC_ALPHA;

        digitTextures['0'] = our::texture_utils::loadImage("assets/textures/digits/0.png");
        digitTextures['1'] = our::texture_utils::loadImage("assets/textures/digits/1.png");
        digitTextures['2'] = our::texture_utils::loadImage("assets/textures/digits/2.png");
        digitTextures['3'] = our::texture_utils::loadImage("assets/textures/digits/3.png");
        digitTextures['4'] = our::texture_utils::loadImage("assets/textures/digits/4.png");
        digitTextures['5'] = our::texture_utils::loadImage("assets/textures/digits/5.png");
        digitTextures['6'] = our::texture_utils::loadImage("assets/textures/digits/6.png");
        digitTextures['7'] = our::texture_utils::loadImage("assets/textures/digits/7.png");
        digitTextures['8'] = our::texture_utils::loadImage("assets/textures/digits/8.png");
        digitTextures['9'] = our::texture_utils::loadImage("assets/textures/digits/9.png");
        digitTextures[':'] = our::texture_utils::loadImage("assets/textures/digits/colon.png");

        timerMaterial = new our::TexturedMaterial();
        timerMaterial->shader = new our::ShaderProgram();
        timerMaterial->shader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
        timerMaterial->shader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
        timerMaterial->shader->link();
        timerMaterial->texture = digitTextures['0'];
        timerMaterial->sampler = new our::Sampler();
        timerMaterial->sampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        timerMaterial->sampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        timerMaterial->tint = glm::vec4(1, 1, 1, 1);
        timerMaterial->pipelineState.blending.enabled = true;
        timerMaterial->pipelineState.blending.sourceFactor = GL_SRC_ALPHA;
        timerMaterial->pipelineState.blending.destinationFactor = GL_ONE_MINUS_SRC_ALPHA;


        quadMesh = new our::Mesh({
            {{0, 0, 0}, {255,255,255,255}, {0, 1}, {0,0,1}},
            {{1, 0, 0}, {255,255,255,255}, {1, 1}, {0,0,1}},
            {{1, 1, 0}, {255,255,255,255}, {1, 0}, {0,0,1}},
            {{0, 1, 0}, {255,255,255,255}, {0, 0}, {0,0,1}},
            }, { 0, 1, 2, 2, 3, 0 });


        cameraController.enter(getApp());
        freeMovement.enter(getApp(), &collisionSystem);
        collisionSystem.setup(&renderer);

        loadConfig();
        // We initialize the camera controller system since it needs a pointer to the app
        // Then we initialize the renderer
        // auto size = getApp()->getFrameBufferSize();
        // renderer.initialize(size, config["renderer"]);

        countdownTime = GAME_COUNTER_TIME;
        isGameOver = false;

        collisionSystem.onHitTrap = [this]() {
            lives--;
            this->decrementLife();
            };
        collisionSystem.onHitKey = [this]() {
            level++;
            if(level < MAX_LEVELS)
                this->nextLevel();
            else
                this->winGame();
        };
    }

    void loadConfig(){
        // We get the scene configuration from the app config
        auto& config = getApp()->getConfig(level)["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets") && !softReset) {
            our::deserializeAllAssets(config["assets"]);
        }
        softReset = true;
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world")) {
            world.deserialize(config["world"]);
        }
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);

    }

    void onDraw(double deltaTime) override {
        std::cout<<"Drawing play!\n";
        if (isGameOver) return;

        // Update the countdown timer
        countdownTime -= (float)deltaTime;
        if (countdownTime <= 0) {
            countdownTime = GAME_COUNTER_TIME;
            isGameOver = true;
            world.clearEntities();
            getApp()->changeState("game-over");
        }

        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        freeMovement.update(&world, (float)deltaTime);
        collisionSystem.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene
        renderer.render(&world, (float)deltaTime);

        // Get a reference to the keyboard object
        auto& keyboard = getApp()->getKeyboard();

        if (keyboard.justPressed(GLFW_KEY_ESCAPE)) {
            // If the escape  key is pressed in this frame, go to the play state
            world.clearEntities();
            getApp()->changeState("menu");
        }

        // Display the countdown timer at the top-right of the screen
        // glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(size.x - 150, 20.0f, 0.0f)); // Top-right position
        glm::ivec2 screenSize = getApp()->getFrameBufferSize();
        glm::mat4 VP = glm::ortho(0.0f, (float)screenSize.x, (float)screenSize.y, 0.0f, 1.0f, -1.0f);

        int minutes = (int)(countdownTime) / 60;
        int seconds = (int)(countdownTime) % 60;
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;

        displayDigits(ss.str(), VP, screenSize.x - 250, 20, 1.0f);

        // Displaying hearts at top left
        float heartHeight = 50.0f;
        float heartWidth = 60.0f;
        float spacing = 25.0f;

        for (int i = 0; i < lives; ++i) {
            float x = spacing + i * (heartHeight + spacing);
            float y = spacing;

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(heartWidth, heartHeight, 1.0f));

            heartMaterial->setup();
            heartMaterial->shader->set("transform", VP * model);
            quadMesh->draw();
        }
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        freeMovement.exit();
        // Clear the world
        world.clear();


        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }

    void displayDigits(const std::string& text, const glm::mat4& VP, float x, float y, float scale) {
        float charWidth = 30.0f * scale;
        float charHeight = 50.0f * scale;
        float spacing = 5.0f * scale;

        for (char c : text) {
            if (digitTextures.count(c) == 0) continue;

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(charWidth, charHeight, 1.0f));

            timerMaterial->texture = digitTextures[c];
            timerMaterial->setup();
            timerMaterial->shader->set("transform", VP * model);
            quadMesh->draw();

            x += charWidth + spacing;
        }
    }

public:
    void decrementLife() {
        if(lives >= 0)
            getApp()->changeState("play");
        else{
            isGameOver = true;
            getApp()->changeState("game-over");
        }
        
        for(auto entity : world.getEntities()){
            world.markForRemoval(entity);
        }

    }

    void winGame() {
        level = 0;
        isGameOver = true;
        getApp()->changeState("win");
        for(auto entity : world.getEntities()){
            world.markForRemoval(entity);
        }
    }

    void nextLevel(){
        getApp()->changeState("play");
        
        for(auto entity : world.getEntities()){
            world.markForRemoval(entity);
        }

    }
};