import json
import random

width = 6
height = 6
NUM_ISLANDS = random.randint(4, 7)
DIRECTIONS = ['left', 'up', 'right']

def round_vector(vec, decimals=1):
    return [round(x, decimals) for x in vec]
def generate_valid_path(num_islands):
    path = []
    prev = None
    for _ in range(num_islands - 1):
        while True:
            dir = random.choice(DIRECTIONS)

            if prev == 'left' and dir == 'right':
                continue
            if prev == 'right' and dir == 'left':
                continue
            if prev == 'up' and dir == 'up':
                continue
            else:
                break

        path.append(dir)
        prev = dir
    return path


def island_position_from_path(path):
    positions = [(0, 0, 0)]
    current = (0, 0, 0)

    for i in range(len(path)):
        if path[i] == 'right':
            # if i > 2 and path[i - 3] == 'left':
            #     current = (current[0] - 4 - width, current[1], current[2])
            # else:
            current = (current[0] - 2 - height, 0, current[2])
        elif path[i] == 'left':
            # if i > 2 and path[i - 3] == 'right':
            #     current = (current[0] + 4 + width, current[1], current[2])
            # else:
            current = (current[0] + 2 + height, 0, current[2])
        else:
            current = (current[0],0, current[2] + 2 + height)

        positions.append(current)

    print(positions)
    return positions

def random_trap_velocity():
    directions = [
        (0.5, 0, 0), (-0.5, 0, 0),
        (0, 0, 0.5), (0, 0, -0.5),
        (0.5, 0, 0.5), (-0.5, 0, -0.5), (0.5, 0, -0.5), (-0.5, 0, 0.5)
    ]
    return random.choice(directions)

def generate_traps_for_island(base_pos):
    traps = []
    num_traps = random.randint(1, 3)
    for _ in range(num_traps):
        offset = (
            random.uniform(.1,  .4),
            1,
            random.uniform(.1,  .4)
        )
        pos = [base_pos[0]+0.4 , 0.8, base_pos[2]+1.1]
        vx, vy, vz = random_trap_velocity()
        trap = {
            "position": round_vector(pos),
            "rotation": [0, 0, 0],
            "scale": [1, 1, 1],
            "name": "trap",
            "components": [
                {
                    "type": "Mesh Renderer",
                    "mesh": "trap",
                    "material": "trap"
                },
{
      "type": "Collision",
      "size": [.7, 0.2,.7],
      "anchor": "center"
    }  

               
            ]
        }
        traps.append(trap)
    return traps

def generate_scene():
    path = generate_valid_path(NUM_ISLANDS)
    print(path)
    positions = island_position_from_path(path)

    world = []
    #lighting

    world.append({
        "rotation": [-45, 0, 0],
        "components": [
            {
                "type": "Light",
                "lightType": "directional",
                "color":[2,2,2]
             }
      
        ]
    })

    # Camera
    world.append({
        "position": [0, 5, 5],
        "scale": [1, 1, 1], 
        "components": [
            {"type": "Camera"},
            {"type": "Free Camera Controller"}
        ]
    })

    # Character
    world.append({
        "position": round_vector([0, .7, 0]),
        "rotation": [0, 180, 0],
        "scale": [1.3, 1.3, 1.3],
        "components": [
            {
                "type": "Mesh Renderer",
                "mesh": "character",
                "material": "character"
            },
             {
                        "type": "Free Movement"
            },
               {
      "type": "Collision",
      "size": [1, 1,0.5],
      "anchor": "bottom _center"
    }


        ]
    })

    # Ground, box, key, and traps
    for i, pos in enumerate(positions):
       
        # Ground
        world.append({
            "position": [pos[0], 0, pos[2]],
            "rotation": [0, -12, 0],
            "scale": [8, 8, 8],
            "name":"groundEarth",
            "components": [
                {
                    "type": "Mesh Renderer",

                    "mesh": "groundEarth",
                    "material": "groundEarth"
                },
{
                    "type": "Collision",
      "size": [6 ,4,6.5],
      "anchor": "center"
}
                

      
            ]
        })

        if i < len(positions)-1:
            # Box8
            box_pos = [pos[0]+2 ,pos[1]+1.1, pos[2]]
            world.append({
                "position": round_vector(box_pos),
                "rotation": [0, 0, 0],
                "scale": [1, 1, 1],
                "name":"box",   
                "components": [
                    {
                        "type": "Mesh Renderer",
                        "mesh": "box",
                        "material": "box"
                    }
                ,  {
                    "type": "Collision",
                    "size": [1.1, 1.1,1.1],
                    "anchor": "center"
                    }

                ]
            })
            traps = generate_traps_for_island(pos)
            world.extend(traps)
        else:
            world.append({
                "position": [pos[0], 1.5, pos[2]],
                "rotation": [0, 0, 0],
                "scale": [1, 1, 1],
                "name":"key",
                "components": [
                    {
                        "type": "Mesh Renderer",
                        "mesh": "key",
                        "material": "key"
                    },
                    {
                        "type": "Movement",
                        "angularVelocity": [0, 45, 0]
                    }
                    , 
                    {
                    "type": "Collision",
                    "size": [1.0, 1.0,1.0],
                    "anchor": "center"
                    }

                ]
            })

    return {
        "start-scene": "play",
        "window": {
            "title": "Push Box Game",
            "size": {"width": 1280, "height": 720},
            "fullscreen": False
        },
        "scene": {
            "renderer": {
                "sky": "assets/textures/sky.png",
                "postprocess": "assets/shaders/postprocess/vignette.frag"
            },
            "assets": {
            "shaders": {
                "tinted": {
                    "vs": "assets/shaders/tinted.vert",
                    "fs": "assets/shaders/tinted.frag"
                },
                "textured": {
                    "vs": "assets/shaders/textured.vert",
                    "fs": "assets/shaders/textured.frag"
                },
                "lit": {
                    "vs": "assets/shaders/light/light.vert",
                    "fs": "assets/shaders/light/light.frag"
                }
            },
            "textures": {
                "character": "assets/textures/colormap.png",
                "groundEarth": "assets/textures/ground_new6.jpg",
                "boxAlbedo": "assets/textures/wood/albedo.jpg",
                "box": "assets/textures/box2.jpg",
                "boxRoughness": "assets/textures/wood/roughness.jpg",
                "boxSpecular": "assets/textures/wood/specular.jpg",
                "key": "assets/textures/gold.png",
                "trap": "assets/textures/trap2.jpg"
            },
            "meshes": {
                "character": "assets/models/character-male-a.obj",
                "groundEarth": "assets/models/ground_new6.obj",
                "box": "assets/models/box2.obj",
                "key": "assets/models/key2.obj",
                "trap": "assets/models/trap2.obj"
            },
            "samplers": {
                "default": {},
                "pixelated": {
                    "MAG_FILTER": "GL_NEAREST"
                }
            },
            "materials": {
                "character": {
                    "type": "lit",
                    "shader": "lit",
                    "pipelineState": {
                        "faceCulling": {
                            "enabled": True
                        },
                        "depthTesting": {
                            "enabled": True
                        }
                    },
                    "albedoMap": "character",
                    "sampler": "default"
                },
                "groundEarth": {
                    "type": "lit",
                    "shader": "lit",
                    "pipelineState": {
                        "faceCulling": {
                            "enabled": True
                        },
                        "depthTesting": {
                            "enabled": True
                        }
                    },
                    "tint": [
                        1,
                        1,
                        1,
                        1
                    ],
                    "albedoMap": "groundEarth",
                    "sampler": "default"
                },
                "box": {
                    "type": "lit",
                    "shader": "lit",
                    "pipelineState": {
                        "faceCulling": {
                            "enabled": True
                        },
                        "depthTesting": {
                            "enabled": True
                        }
                    },
                    "tint": [
                        1,
                        1,
                        1,
                        1
                    ],
                    "albedoMap": "box",
                    "sampler": "default"
                },
                "key": {
                    "type": "lit",
                    "shader": "lit",
                    "pipelineState": {
                        "faceCulling": {
                            "enabled": True
                        },
                        "depthTesting": {
                            "enabled": True
                        }
                    },
                    "tint": [
                        1,
                        1,
                        1,
                        1
                    ],
                    "albedoMap": "key",
                    "sampler": "default"
                },
                "trap": {
                    "type": "lit",
                    "shader": "lit",
                    "pipelineState": {
                        "faceCulling": {
                            "enabled": True
                        },
                        "depthTesting": {
                            "enabled": True
                        }
                    },
                    "tint": [
                        1,
                        1,
                        1,
                        1
                    ],
                    "albedoMap": "trap",
                    "sampler": "default"
                }
            }
        },
            "world": world
        }
    }


# Generate and export
scene_data = generate_scene()

with open("./config/generated_game.jsonc", "w") as f:
    json.dump(scene_data, f, indent=4)

print("Scene JSON generated as 'generated_scene.json'")
