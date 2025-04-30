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
            current = (current[0] - 2 - height, current[1], current[2])
        elif path[i] == 'left':
            # if i > 2 and path[i - 3] == 'right':
            #     current = (current[0] + 4 + width, current[1], current[2])
            # else:
            current = (current[0] + 2 + height, current[1], current[2])
        else:
            current = (current[0], current[1], current[2] + 2 + height)

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
        pos = [base_pos[0]-1 , base_pos[1]+0.5, base_pos[2] +1]
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
      "size": [0.6, 0.6],
      "anchor": "top_left"
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

    # Camera
    world.append({
        "position": [0, 2, 2],
        "scale": [1, 1, 1],
        "components": [
            {"type": "Camera"},
            {"type": "Free Camera Controller"}
        ]
    })

    # Character
    world.append({
        "position": round_vector([0, .8, 0]),
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
      "size": [0.1, 0.1],
      "anchor": "top_left"
    }


        ]
    })

    # Ground, box, key, and traps
    for i, pos in enumerate(positions):
       
        # Ground
        world.append({
            "position": round_vector(pos),
            "rotation": [0, 0, 0],
            "scale": [8, 8, 8],
            "name":"groundEarth",
            "components": [
                {
                    "type": "Mesh Renderer",
                    "mesh": "groundEarth",
                    "material": "groundEarth"
                },
                

      
            ]
        })

        if i < len(positions):
            # Box
            box_pos = [pos[0] ,pos[1]+ 3, pos[2]]
            world.append({
                "position": round_vector(box_pos),
                "rotation": [0, 0, 0],
                "scale": [0.5, 0.5, 0.5],
                "name":"box",   
                "components": [
                    {
                        "type": "Mesh Renderer",
                        "mesh": "box",
                        "material": "box"
                    }
                ,  {
      "type": "Collision",
      "size": [1.0, 1.0],
      "anchor": "center"
    }

                ]
            })
        if i == len(positions) - 1:
            # Key
            key_pos = [pos[0], pos[1]+2, pos[2]]
            world.append({
                "position": round_vector(key_pos),
                "rotation": [0, 0, 0],
                "scale": [0.2, 0.2, 0.3],
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
      "size": [1.0, 1.0],
      "anchor": "center"
    }

                ]
            })

        # Traps
        traps = generate_traps_for_island(pos)
        world.extend(traps)

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
                }
            },
            "textures": {
                "character": "assets/textures/colormap.png",
                "groundEarth": "assets/textures/ground_new6.jpg",
                "box": "assets/textures/1335133419679.png",
                "key": "assets/textures/gold.png",
                "trap": "assets/textures/trap.png"
            },
            "meshes": {
                "character": "assets/models/character-male-a.obj",
                "groundEarth": "assets/models/ground_new6.obj",
                "box": "assets/models/box.obj",
                "key": "assets/models/key.obj",
                "trap": "assets/models/trap.obj"
            },
            "samplers": {
                "default": {},
                "pixelated": {
                    "MAG_FILTER": "GL_NEAREST"
                }
            },
            "materials": {
                "character": {
                    "type": "textured",
                    "shader": "textured",
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
                    "texture": "character",
                    "sampler": "default"
                },
                "groundEarth": {
                    "type": "textured",
                    "shader": "textured",
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
                    "texture": "groundEarth",
                    "sampler": "default"
                },
                "box": {
                    "type": "textured",
                    "shader": "textured",
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
                    "texture": "box",
                    "sampler": "default"
                },
                "key": {
                    "type": "textured",
                    "shader": "textured",
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
                    "texture": "key",
                    "sampler": "default"
                },
                "trap": {
                    "type": "textured",
                    "shader": "textured",
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
                    "texture": "trap",
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
