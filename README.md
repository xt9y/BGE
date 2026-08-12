#### BGE - Basic Game Engine (cool name right)
- last showcase of engine:`https://youtu.be/8IT8n9sierU?si=2kPJQaqm_F3vs9J3`


#### THIS IS IT:

![IMG](./Engine/res/GAME2.png)

![IMG](./Engine/res/GAME1.png)

![IMG](./Engine/res/GAME3.png)


#### NOTES:
- Please dont clone everything and just do: `git clone --depth 1 https://github.com/xt9y/BGE.git`
- Build and run: `c build run`
- Linux build requirements : OpenGL loader/headers, X11
    - Example (Arch btw): `sudo pacman -S libx11 libxrandr libxi libxcursor libxinerama xorgproto mesa mesa-utils libglvnd`

#### C build system

Build and run this branch with [xt9y/C](https://github.com/xt9y/C):

```sh
c build run
```

`build.c` keeps the mixed C/C++ engine libraries in C's global dependency cache while the game entry point is built through the `c` workflow. The generated self-dependency lockfile is intentionally ignored because its resolved commit would be self-referential on this integration branch.

#### RendererCheck integration

With [xt9y/RendererCheck](https://github.com/xt9y/RendererCheck) installed:

```sh
rendercheck run
```

RendererCheck exercises the C build workflow together with BGE. When `RENDERCHECK=1` is present, BGE creates a hidden window, renders one frame, supports `RENDERCHECK_CAPTURE_PATH` as an RGB8 PPM framebuffer capture, and exits normally.

The committed smoke test leaves `capture = false` so a fresh clone passes without requiring an unreviewed image baseline. To turn it into a visual regression test, set `capture = true`, run the test once, inspect `.rendercheck/smoke/actual.ppm`, and approve it with:

```sh
rendercheck approve smoke
```
