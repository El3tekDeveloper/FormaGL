# FormaGL

<p align="center">
  <img src="assets/media/ViewImage.png" alt="FromaGL Preview" width="800"/>
</p>

<p align="center">
  <b>⚡ Lightweight Modern OpenGL Framework for C++</b><br>
  <i>Simplifying graphics programming without the weight of a full engine.</i>
</p>

---

## About

**FromaGL** is a lightweight C++ framework built on top of modern OpenGL.  
It provides essential tools and abstractions for rendering, lighting, geometry, model loading, optimization, and basic physics — aimed at developers who want full control without rewriting boilerplate every project.

✅ Not a game engine — just the tools you need  
✅ Minimal, modular, and beginner-friendly  
✅ Currently in active development

NOTE: This project is currently a personal side project and still evolving.
---

## Features (Work in Progress)

- ✔ Window & OpenGL context creation
- ✔ Shader and material system
- ✔ Geometry utilities (Cube, Plane, Sphere, etc.)
- ✔ Texture handling and asset loading
- ✔ Basic lighting utilities (Directional, Point, Spot)
- ✔ Model loading support (`.obj` planned, `.gltf`)
- ✔ Optimized OpenGL helpers (VAO/VBO management, batching)
- ⏳ Physics module (planned)
- ⏳ Renderer presets (PBR, shadows, post-processing)
- ⏳ ECS integration
- ⏳ Debug UI (ImGui)
- ⏳ Full documentation and examples

---

## Roadmap

| Status | Feature |
|--------|---------|
| ✅ | Core windowing + input system |
| ✅ | Shader framework |
| ✅ | Basic geometry module |
| 🔄 | Model importer |
| 🔄 | Lighting framework |
| ⏳ | Physics wrapper |
| ⏳ | Full render pipeline |
| ⏳ | Example projects |
| ⏳ | First public release `v0.1` |

---

## Build & Usage

> 📌 Full build instructions will be added once the API becomes stable.  
> Current build system: **CMake** — requires **GLFW**, **GLAD**, **glm**, **stb**.

```bash
git clone https://github.com/El3tekDeveloper/FormaGL.git
cd FromaGL
cmake -B build
cmake --build build
