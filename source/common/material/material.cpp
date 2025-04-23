#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our
{

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const
    {
        // TODO: (Req 7) Write this function

        pipelineState.setup();
        // Use the shader program

        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data)
    {
        if (!data.is_object())
            return;

        if (data.contains("pipelineState"))
        {
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint
    void TintedMaterial::setup() const
    {
        // TODO: (Req 7) Write t his function

        Material::setup();

        // Set the tint uniform

        shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex"
    void TexturedMaterial::setup() const
    {
        // TODO: (Req 7) Write this function

        TintedMaterial::setup();

        shader->set("alphaThreshold", alphaThreshold);

        // Bind the texture and sampler to a texture unit

        glActiveTexture(GL_TEXTURE0); // Activate texture unit 0
        texture->bind();              // Bind the texture
        sampler->bind(0);             // Bind the sampler to texture unit 0

        // Send the texture unit index to the shader
        shader->set("tex", 0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data)
    {
        TintedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }
    void LitMaterial::setup() const {
        // Call the base Material setup first
        Material::setup();

        shader->set("alphaThreshold", alphaThreshold);

        // Bind textures and samplers to texture units
        int textureUnit = 0;

        if (albedo) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            albedo->bind();
            sampler->bind(textureUnit);
            shader->set("material.albedo", textureUnit++);
        }

        if (specular) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            specular->bind();
            sampler->bind(textureUnit);
            shader->set("material.specular", textureUnit++);
        }

        if (roughness) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            roughness->bind();
            sampler->bind(textureUnit);
            shader->set("material.roughness", textureUnit++);
        }

        if (ambient) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            ambient->bind();
            sampler->bind(textureUnit);
            shader->set("material.ambient", textureUnit++);
        }

        if (emissive) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            emissive->bind();
            sampler->bind(textureUnit);
            shader->set("material.emissive", textureUnit);
        }
    }

    void LitMaterial::deserialize(const nlohmann::json& data) {
        // Call the base Material deserialize first
        Material::deserialize(data);

        if (!data.is_object())
            return;

        // Load alpha threshold
        alphaThreshold = data.value("alphaThreshold", 0.0f);

        // Load textures
        albedo = AssetLoader<Texture2D>::get(data.value("albedo", ""));
        specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
        roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        ambient = AssetLoader<Texture2D>::get(data.value("ambient", ""));
        emissive = AssetLoader<Texture2D>::get(data.value("emissive", ""));

        // Load sampler (shared for all textures)
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

}