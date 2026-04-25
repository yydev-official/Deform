 # Deform Engine

> **Note:** This is C++ version of Deform.   
> [Check out the main branch →](https://github.com/deform-labs/Deform)

> A custom-built C++ game engine focused on transformation and deformation systems.

Deform is built to explore low-level engine architecture and real-time object manipulation, demonstrating core concepts behind modern engines, system design, modular architecture, and transformation pipelines. The engine and editor are kept separate: the engine ships as a Windows DLL (`DEFORM_API`) and the editor links against it, keeping the core lean and the tooling flexible.

---

## Building

### Requirements

- Windows 10/11
- Visual Studio 2022 with **Desktop development with C++** workload
- Windows SDK (DirectX 11 included)

### Steps

1. Clone the repository
   ```bash
   git clone https://github.com/yydev-official/Deform.git
   cd Deform
   ```
2. Start building however you want!
---

## Roadmap

- [x] Engine/editor DLL split with `DEFORM_API`
- [x] Dear ImGui docking layout
- [x] Editor panel system (`Editor::UI`)
- [ ] Scene graph and entity system
- [ ] Asset pipeline (mesh, texture import)
- [x] Vulkan backend via RHI abstraction layer
- [ ] Scripting layer

---

## License

MIT — do whatever you want with it.

---

## Author

Made by [@yydev-official](https://github.com/yydev-official)
