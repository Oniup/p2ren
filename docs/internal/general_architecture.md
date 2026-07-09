# File structure

```
\assets
    \textures
    \maps
    \shaders
\docs 
    \internal -- Desgin documents and diagrams 
    \extern   -- Documentation
\src
    \p2ren_main            -- Executable with a defined main (Optional)
    \p2ren_world           -- World, ECS, Actor wrapper, Read level data
    \p2ren_core            -- SDL Wrapper --> Window, IO, Threads
    \p2ren_renderer        -- Forward renderer implementation, Materials, Models
    \p2ren_render_hardware -- OpenGL abstraction API
    \pch.h
\thirdparty
```
