#include <iostream>
#include <fstream>
#include <flags/flags.h>
#include <json/json.hpp>

#include <application.hpp>

#include "states/menu-state.hpp"
#include "states/play-state.hpp"
#include "states/game-over-state.hpp"
#include "states/win-state.hpp"
#include "states/shader-test-state.hpp"
#include "states/mesh-test-state.hpp"
#include "states/transform-test-state.hpp"
#include "states/pipeline-test-state.hpp"
#include "states/texture-test-state.hpp"
#include "states/sampler-test-state.hpp"
#include "states/material-test-state.hpp"
#include "states/entity-test-state.hpp"
#include "states/renderer-test-state.hpp"

int main(int argc, char **argv)
{

    flags::args args(argc, argv); // Parse the command line arguments
    // config_path is the path to the json file containing the application configuration
    // Default: "config/app.json"

    // Assume a flag like -s scenes.txt is used to specify the scene list file
    std::string scene_list_file = args.get<std::string>("s", "config/levels_list.txt");

    // Open the scene list file
    std::ifstream list_file(scene_list_file);
    if (!list_file) {
        std::cerr << "Couldn't open scene list file: " << scene_list_file << std::endl;
        return -1;
    }

    std::vector<nlohmann::json> app_config_list;
    std::string path;
    while (std::getline(list_file, path)) {
        if (path.empty()) continue; // skip empty lines
        std::ifstream file_in(path);
        if (!file_in) {
            std::cerr << "Couldn't open file: " << path << std::endl;
            return -1;
        }

        try {
            app_config_list.push_back(nlohmann::json::parse(file_in, nullptr, true, true));
        } catch (const nlohmann::json::parse_error& e) {
            std::cerr << "Failed to parse JSON from " << path << ": " << e.what() << std::endl;
            return -1;
        }
        file_in.close();
    }
    // Create the application
    int run_for_frames = args.get<int>("f", 0);

    our::Application app(app_config_list);

    app.registerState<Menustate>("menu");
    app.registerState<Playstate>("play");
    app.registerState<GameOverState>("game-over");
    app.registerState<WinState>("win");
    app.registerState<ShaderTestState>("shader-test");
    app.registerState<MeshTestState>("mesh-test");
    app.registerState<TransformTestState>("transform-test");
    app.registerState<PipelineTestState>("pipeline-test");
    app.registerState<TextureTestState>("texture-test");
    app.registerState<SamplerTestState>("sampler-test");
    app.registerState<MaterialTestState>("material-test");
    app.registerState<EntityTestState>("entity-test");
    app.registerState<RendererTestState>("renderer-test");

    // Use first config to determine start-scene (can be changed to loop if needed)
    if (app_config_list.size() > 0 && app_config_list[0].contains("start-scene")) {
        app.changeState(app_config_list[0]["start-scene"].get<std::string>());
    }

    return app.run(run_for_frames);
}