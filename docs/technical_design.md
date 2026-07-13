# Mini Technical Design Document (7/12/2026)

## Goals and Scope

- Single threaded
- Level parser for external 3D level editor (Most likely TrenchBroom, must support entity ID that can link to each other)
- Custom Renderer
- ECS with Actor wrapper for more small count, specialised actors
- FBX and Obj model loader

## High-Level Architecture

### Project structure

```
\assets
    \models
    \maps
    \textures
    \shaders
\src
    \p2ren_core     -- All modules below include, Window, Application facade
    \p2ren_renderer -- Forward renderer with OpenGL backend
    \p2ren_world    -- Managing the ECS and level population
    \p2ren_example  -- Example executable that loads the example map
CMakeLists.txt
...
README.md
```

### Libraries

- **SDL:** Platform specific such as window handling, audio.
- **SDL_image:** Loading image files to processing later.
- **fmt:** Superior string formatting.
- **glm:** Linear math library.
- **unordered_dense:** Superior dictionary implementation compared to std::unordered_map.
- **assimp:** For model loading.

### Entity Component System

- World, contains a single loaded scene.
- Utilise EnTT ECS library for entity representation.
- Scriptable Entity wrapper for easy initialisation and logic. For larger number of entities of a specific type, use the default system approach.
- Entity creation using abstract factory based that reads entity ID from Level Editor.

### Renderer

- Forward rendering, use builtin multi-sampling anti aliasing.
- Blinn-Phong lighting and materials.
  - Colour slots support both a texture and a colour.
- Directional and Omnidirectional shadow maps.
- Post Processing layer.
  - HDR, Tone mapping and Gamma Correction.
  - Bloom.

### Resources

- Resource pool and Resource Handle asset storage.
- ```cpp
  // On application initialization
  m_ResourceManager.InitializePool<Texture>();
  // Within constructor/OnInitialize
  ResourceHandle<Texture> texture_handle = resource_manager.QueryHandle<Texture>("Resource name");
  // When resource is needed
  Texture* texture = resource_manager.QueryResource(texture_handle);
  ```
- Model loading.
  - FBX and obj only.
- Use of RAII.
- RHI only designed to support OpenGL.

## Weekly Milestones

#### Already completed

- Resource manager.
- Window management.

### Week 1 (Renderer foundations)

- Start learn 3D editor of choice.
- Hard code assets.
- World representation
- RHI.
  - Shader, Texture and Buffer.
  - Texture and Framebuffer should use descriptors to create.
  - Textures should support cubemaps
- Renderer.
  - Blinn-Phong.

### Week 2 (Level parser)

- Resources.
  - Load FBX and obj models.
- Continue learning 2D editor of choice if I still don't feel comfortable yet.
- Event dispatcher and input manager.
- Level parser.
  - Static geometry.
  - Entities.
  - Don't include entity linkage yet.

### Week 3 (More advanced renderer and level parser)

- Renderer.
  - Shadow maps.
    - Directional (Required).
    - Omnidirectional (Drop if running out of time).
  - Skybox (Lower priority)
  - HDR and gamma correction.
  - MSAA.
  - Bloom.
- Link entities using triggers.
- Use level from learning to create a cool little demo level.
