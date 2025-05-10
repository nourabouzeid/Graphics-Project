<div>

# <img align=center width=75px height=75px src="https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExOXVmZXh1enF0a2RleG16bW51Z3Vtb2R1cHN4Z2pyMzJzbGw5a3Y1MSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/7TkSOPplgsyfaxoeDa/giphy.gif"> Push-The-Box

</div>

<div align="center">
    <h3> Menu screen </h3>
   <img align="center" width=100% src="assets/textures/menu.jpeg" alt="menu screen">
</div>
<hr/>

<div align="center">
    <h3> Push the blocks, reach the key, claim your win!</h3>
   <img align="center" width=100% src="docs/game_scene_level_1.png" alt="game scene">
</div>
<hr/>

<div align="center">
    <h3> Think you're skilled enough to dodge traps and escape this fate?</h3>
   <img align="center" width=100% src="assets/textures/game-over.jpeg" alt="game over">
</div>

<div >

## <img align="center" width=50 height=50 src="https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExNjA2bW4weHN5OW83MDV2emVrcHBwMzV6djQ2MG4zYzdhemxlMW9qdyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/VZ5PAfXIurhTFQVCL8/giphy.gif"/> Game Idea

</div>

You land in a strange world made of floating islands. Your goal is to push boxes to build paths, watch out for sneaky traps, and keep moving forward. If you’re smart and quick, you’ll reach the key and win the game!

<div >

## <img align="center" width=50 height=50 src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExZm5wcXY4NWU5Z3hlMXo3MHh6Zm5pMzl6eXBlbjF3dTg1enJoM3FoMSZlcD12MV9zdGlja2Vyc19zZWFyY2gmY3Q9cw/kL3ZvNKk4hyR1B8hVt/giphy.gif"/> Technology Used

</div>

<img width=100 src="https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Framkumarsingh1.github.io%2Finternwings%2Fimg%2Fc%2B%2B.gif&f=1&nofb=1&ipt=563965ccce2bda3f85e16c43c89e07cf1247ed613e9ecd34618384a861594a31">
<img width=100 src="https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fwww.profesionalreview.com%2Fwp-content%2Fuploads%2F2019%2F11%2FOpenGL-00.jpg&f=1&nofb=1&ipt=980185a5613bee73d9aab429516281446aedd8fa16da0f8d371c383c1fed9ca8">

### 🎮 Game Engine & Architecture

- Built from scratch using **C++17** and **OpenGL**.
- Modular **Entity-Component-System (ECS)** architecture.
- Scene data defined in **JSON files**, no hardcoded objects.

### 🎨 Rendering & Shaders

- Forward rendering pipeline with:
  - **Depth testing**
  - **Face culling**
  - **Blending**
  - **Color/depth masks**
- GLSL shaders for:
  - Color mixing
  - Checkerboard patterns
  - Texture mapping
  - Lighting and post-processing

### 💡 Lighting System

- Support for **multiple lights** with different types and parameters.
- Implemented lighting model with:
  - Albedo
  - Specular
  - Roughness
  - Emission

### 🌌 Sky & Post-processing

- Sky sphere rendering for immersive environments.
- **Framebuffer rendering** with post-processing effects:
  - Vignette
  - Chromatic aberration

### 🧱 Mesh & Materials

- Mesh rendering using:
  - Vertex Buffer Objects (VBOs)
  - Element Buffer Objects (EBOs)
  - Vertex Array Objects (VAOs)
- Material system combining:
  - Shader programs
  - Pipeline state
  - Textures & samplers

### 🧪 Testing & Configurations

- Modular test states: `shader-test`, `mesh-test`, `texture-test`, etc.
- JSON-based configuration files in `/config`.
- Batch testing using `scripts/run-all.ps1` and result comparison via `compare-all.ps1`.

<div >

## <img align="center" width=50 height=50 src="https://media2.giphy.com/media/v1.Y2lkPTc5MGI3NjExOWh6dWV4dXpyeTc3MTBxZm9hMTF1amc1bmV0eXQzY256ZW5vNTJncyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9cw/pnXGod6LmqiKhTL6iE/giphy.gif"/> Build and Run

</div>

- Make sure you have `cmake`,`c++ compiler ` and `cmake-tools` for vscode extenstion
- If you're using linux like me, don't worry, it's made `cross-platform`

1. From vscode, `F7` or choose `build` in the below bar
2. Run the following command
   ```bash
       ./bin/GAME_APPLICATION
   ```

## Contributors

<table  >
  <tr>
      <td align="center"><a href="https://github.com/jpassica"><img src="https://avatars.githubusercontent.com/u/118307227?v=4" width="150px;" alt=""/><br /><sub><b>jpassica </b></sub></a><br /></td>
     <td align="center"><a href="https://github.com/nourabouzeid"><img src="https://avatars.githubusercontent.com/u/118685428?v=4" width="150px;" alt=""/><br /><sub><b>nourabouzeid </b></sub></a><br /></td>
     <td align="center"><a href="https://github.com/AbdelruhmanSamy"><img src="https://avatars.githubusercontent.com/AbdelruhmanSamy?v=4" width="150px;" alt=""/><br /><sub><b>Abdelruhman Samy </b></sub></a><br /></td>
     <td align="center"><a href="https://github.com/Mariam-Amin12"><img src="https://avatars.githubusercontent.com/u/128838373?v=4" width="150px;" alt=""/><br /><sub><b>Mariam Amin </b></sub></a><br /></td>

  </tr>
</table>
