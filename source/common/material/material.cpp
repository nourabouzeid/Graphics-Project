#include "material.hpp"
#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"
#include<iostream>
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
        shader->set("material.albedo_tint", albedoTint);
        shader->set("material.specular_tint", specularTint);
        shader->set("material.emissive_tint", emissiveTint);
        shader->set("material.roughness_range", roughnessRange);
        shader->set("alphaThreshold", alphaThreshold);

        // Bind textures and samplers to texture units
        int textureUnit = 0;

        if (albedoMap) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            albedoMap->bind();
            sampler->bind(textureUnit);
            shader->set("material.albedo_map", textureUnit++);
        }

        if (specularMap) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            specularMap->bind();
            sampler->bind(textureUnit);
            shader->set("material.specular_map", textureUnit++);
        }

        if (roughnessMap) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            roughnessMap->bind();
            sampler->bind(textureUnit);
            shader->set("material.roughness_map", textureUnit++);
        }

        if (ambientMap) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            ambientMap->bind();
            sampler->bind(textureUnit);
            shader->set("material.ambient_occlusion_map", textureUnit++);
        }

        if (emissiveMap) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            emissiveMap->bind();
            sampler->bind(textureUnit);
            shader->set("material.emissive_map", textureUnit);
        }
    }

    void LitMaterial::deserialize(const nlohmann::json& data) {
        // Call the base Material deserialize first
        Material::deserialize(data);

        if (!data.is_object())
            return;

        // Load alpha threshold
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        albedoTint = data.value("albedoTint", glm::vec3(0.5f));
        specularTint = data.value("specularTint", glm::vec3(1.0f));
        emissiveTint = data.value("emissiveTint", glm::vec3(1.0f));
        roughnessRange = data.value("roughnessRange", glm::vec2(0.0f, 1.0f));


        // Load textures
        albedoMap = AssetLoader<Texture2D>::get(data.value("albedoMap", ""));
        specularMap = AssetLoader<Texture2D>::get(data.value("specularMap", ""));
        roughnessMap = AssetLoader<Texture2D>::get(data.value("roughnessMap", ""));
        ambientMap = AssetLoader<Texture2D>::get(data.value("ambientMap", ""));
        emissiveMap = AssetLoader<Texture2D>::get(data.value("emissiveMap", ""));

        // Load sampler (shared for all textures)
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

}