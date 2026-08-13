# BGE

BGE is a small C/OpenGL game engine and editor with recursive stencil portals. The repository also contains a demo game under `Game/`; demo gameplay code is deliberately kept outside `Engine/`.

## Build

Install [xt9y/C](https://github.com/xt9y/C), then:

```sh
git clone --depth 1 https://github.com/xt9y/BGE.git
cd BGE
c build run
```

Linux/X11 example dependencies on Arch Linux:

```sh
sudo pacman -S libx11 libxrandr libxi libxcursor libxinerama xorgproto mesa mesa-utils libglvnd
```

## Minimal entry point

```c
#include "Engine/Engine.h"
#include "Engine/res/level1.h"

int main(void)
{
    Engine app = {0};
    if (!engine_init(&app, "Engine/res/level1.h", load_1())) return 1;
    engine_run(&app);
    engine_destroy(&app);
    return 0;
}
```

The runtime uses the `level_data_t` returned by the loader. The path is the persistence target; BGE does not parse generated C source at runtime.

## Layout

```text
Engine/   platform, renderer, editor, resources, world/level infrastructure
Game/     demo player, weapons, gameplay input, and demo assets
main.c    level selection + engine lifecycle only
```

Ownership rules:

- `engine_runtime` owns camera, framebuffer metadata, editor state, texture registry, and editable level.
- The texture registry has one owner; OpenGL handles are not duplicated across registries.
- Editor selection uses stable sector/quad handles instead of pointers into reallocating arrays.
- Level data contains no transient render-camera pointers.
- Renderer-owned OpenGL objects have explicit shutdown paths, so init/destroy/init in one process is supported.

## Levels

Generated level headers are currently the source format. BGE deep-copies generated static data before editing and writes updates through `Engine/level_store.c` using a temporary file and replacement step.

## RendererCheck

With [xt9y/RendererCheck](https://github.com/xt9y/RendererCheck) installed:

```sh
rendercheck run
```

When `RENDERCHECK` is present, BGE creates a hidden window, records GPU timing, optionally captures the framebuffer, renders one deterministic startup frame, and exits normally.

CI also checks project warnings, core sanitizer tests, generated-level compilation, repeated engine initialization, RendererCheck failure policies, and visual regression behavior.

## Third-party code

Vendored GLFW and Dear ImGui retain their upstream licenses under `Vendor/`. Project warning policy is applied to BGE/Game sources separately from upstream vendor code.
