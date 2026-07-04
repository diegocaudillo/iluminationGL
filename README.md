# Pharos' Geometry

A real-time 3D scene viewer with shader support, written in C++ using OpenGL. Loads scenes defined in config files, supporting multiple meshes, textures, and GLSL shaders. Originally developed circa 2010 at CIMAT.

---

## Dependencies

Install all required libraries (Debian/Ubuntu):

```bash
sudo apt install libglew-dev freeglut3-dev libsoil-dev libopenctm-dev
```

These cover:
- **GLEW** — OpenGL extension loading
- **FreeGLUT** — window/input management
- **SOIL** — image/texture loading
- **OpenCTM** — compressed 3D mesh loading
- **OpenGL + GLU** — rendering (usually pre-installed with your GPU drivers)

---

## Building

```bash
make
```

This compiles `src/interfaz.cpp` and outputs the `scene` binary. If you hit linker errors, use the full command:

```bash
g++ -o scene src/interfaz.cpp -Wall \
  -I/usr/include/SOIL \
  -lGLEW -lglut -lSOIL -lopenctm -lGL -lGLU
```

---

## Project Structure

```
.
├── src/
│   ├── interfaz.cpp     # Entry point, GLUT setup, input handling
│   ├── geometria.cpp    # Scene graph, model transforms, lighting state
│   ├── auxiliares.cpp   # shader/model/texture utility classes
│   └── menu.cpp         # On-screen text rendering
├── shader/
│   ├── luces2.vert/.frag  # Main Phong lighting shader
│   ├── luces3.frag        # Skybox shader
│   ├── agua3.frag         # Water shader
│   └── letras.vert/.frag  # Text rendering shader
├── samples/
│   └── faro.cfg           # Default scene config (loaded on startup)
├── models/                # .ctm mesh files
├── textures/              # .bmp / .tga texture files
├── Makefile
└── README.md
```

---

## Scene Config Format

Scenes are defined in plain-text `.cfg` files. The default scene loaded on startup is `samples/faro.cfg`.

**Format:**

```
<number of models>

<textures count>
<path to .ctm mesh>
<vertex shader> <fragment shader>
<texture 1>
<texture 2>
... (repeat for each texture)
<4x4 transform matrix, row by row>

... (repeat block for each model)

<skybox texture count>
<skybox vertex shader> <skybox fragment shader>
<skybox texture 1>
```

See `samples/faro1.cfg` or `samples/otra.cfg` for working examples.

You can load a different scene at runtime by pressing **O** and typing the path.

---

## Controls

| Key | Action |
|-----|--------|
| `W` / `S` | Move scene forward / back |
| `A` / `D` | Rotate scene left / right |
| `R` / `F` | Tilt scene up / down |
| Arrow keys | Move free light X/Z |
| `Page Up/Down` | Move free light Y |
| `Home` | Reset all transforms |
| `E` | Stop current animation |
| `T` | Toggle test mode (shows light position as sphere) |
| `V` | Toggle FPS counter |
| `O` | Open prompt to load a new `.cfg` scene |
| `H` | Show about screen |
| `P` | Toggle fullscreen |
| `Q` / `Esc` | Quit |

---

## License

GNU General Public License v2 or later.
Copyright 2010 Diego Caudillo Amador — CIMAT.
