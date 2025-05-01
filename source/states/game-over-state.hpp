#pragma once

#include <application.hpp>
#include <shader/shader.hpp>
#include <texture/texture2d.hpp>
#include <texture/texture-utils.hpp>
#include <material/material.hpp>
#include <mesh/mesh.hpp>

#include <functional>
#include <array>

#include "input/button.hpp"

class GameOverState : public our::State
{
    our::TexturedMaterial *backgroundMaterial;
    our::TintedMaterial *highlightMaterial;
    our::Mesh *rectangle;
    float time;
    std::array<Button, 2> buttons;

    void onInitialize() override
    {
        backgroundMaterial = new our::TexturedMaterial();
        backgroundMaterial->shader = new our::ShaderProgram();
        backgroundMaterial->shader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
        backgroundMaterial->shader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
        backgroundMaterial->shader->link();
        backgroundMaterial->texture = our::texture_utils::loadImage("assets/textures/game-over.jpeg");
        backgroundMaterial->tint = glm::vec4(1.f, 1.0f, 1.0f, 1.0f);
        backgroundMaterial->sampler = new our::Sampler();
        backgroundMaterial->sampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        backgroundMaterial->sampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        highlightMaterial = new our::TintedMaterial();
        highlightMaterial->shader = new our::ShaderProgram();
        highlightMaterial->shader->attach("assets/shaders/tinted.vert", GL_VERTEX_SHADER);
        highlightMaterial->shader->attach("assets/shaders/tinted.frag", GL_FRAGMENT_SHADER);
        highlightMaterial->shader->link();
        highlightMaterial->tint = glm::vec4(0.0f, 0.0f, 0.0f, 0.5f);
        highlightMaterial->pipelineState.blending.enabled = true;
        highlightMaterial->pipelineState.blending.equation = GL_FUNC_REVERSE_SUBTRACT;
        highlightMaterial->pipelineState.blending.sourceFactor = GL_SRC_ALPHA;
        highlightMaterial->pipelineState.blending.destinationFactor = GL_ONE_MINUS_SRC_ALPHA;

        rectangle = new our::Mesh({
                                       {{0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
                                       {{1.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
                                       {{1.0f, 1.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                                       {{0.0f, 1.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                                   },
                                   {0, 1, 2, 2, 3, 0});

        time = 0;

        // Restart
        buttons[0].position = {470.0f, 510.0f};
        buttons[0].size = {340.0f, 60.0f};
        buttons[0].action = [this]()
        { this->getApp()->changeState("play"); };

        // Home
        buttons[1].position = {530.0f, 610.0f};
        buttons[1].size = {220.0f, 60.0f};
        buttons[1].action = [this]()
        { this->getApp()->changeState("menu"); };
    }

    void onDraw(double deltaTime) override
    {
        auto &mouse = getApp()->getMouse();
        glm::vec2 mousePos = mouse.getMousePosition();

        if (mouse.justPressed(0))
        {
            for (auto &button : buttons)
                if (button.isInside(mousePos))
                    button.action();
        }

        glm::ivec2 size = getApp()->getFrameBufferSize();
        glViewport(0, 0, size.x, size.y);

        glm::mat4 VP = glm::ortho(0.0f, (float)size.x, (float)size.y, 0.0f, 1.0f, -1.0f);
        glm::mat4 M = glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

        time += (float)deltaTime;
        backgroundMaterial->tint = glm::vec4(glm::smoothstep(0.00f, 2.00f, time));

        backgroundMaterial->setup();
        backgroundMaterial->shader->set("transform", VP * M);
        rectangle->draw();

        for (auto &button : buttons)
        {
            highlightMaterial->tint = button.isInside(mousePos) ?
                glm::vec4(0.0f, 0.0f, 0.0f, 0.3f) :
                glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);

            highlightMaterial->setup();
            highlightMaterial->shader->set("transform", VP * button.getLocalToWorld());
            rectangle->draw();
        }
    }

    void onDestroy() override
    {
        delete rectangle;
        delete backgroundMaterial->texture;
        delete backgroundMaterial->shader;
        delete backgroundMaterial;
        delete highlightMaterial->shader;
        delete highlightMaterial;
    }
};
