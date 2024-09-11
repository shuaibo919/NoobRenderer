## NoobRenderer-pluto 🌑 
In this pluto branch, the first step will be to add RHI (Render Hardware Interface) layer to implement abstraction for Opengl and Vulkan, then refactor the entire project.

```shell
  ☼ <- now in here!
  |
Pluto   Neptune  Uranus  Saturn  Jupiter  Mars  Earth   Venus   Mercury   
  🌑       🔵         ...           🪐      🔴     🌍        ...      
       
```

(:The next version will be named as [**Neptune**](#))
## Pluto

### Todo Feature-List 「Dev」
#### Cross Platform Support「WIP」
- [x] Windows
- [x] MacOS
- [ ] Linux
...

#### Create a RHI Object by Fluent Interface 「WIP」
all the RHI object can be created by fluent interface, for example:

```cpp
auto colorTarget = Texture::Builder()
                        .SetBase(600, 600, 1, RHIFormat::R16G16B16A16Float)
                        .SetFilter(TextureFilter::Linear, TextureFilter::Linear)
                        .SetWrap(TextureWrap::ClampToedge)
                        .Create(Texture::Type::Texture2D, ctx);
```
> non-fluent interface construction is only accessible in the Backend layer.
> - [] todo: is it necessary to open it?

#### High-level abstraction of scene rendering 「Waiting」
- [ ] Scene Graph
- [ ] Entity Component System
- [ ] More
...

#### Redesign the Editor 「Waiting」
- [ ] UI
...

#### And More 「Waiting」
- [ ] More
...

### Timeline 「Dev」

- 2024.7.5: create pluto branch
- 2024.7.7: add RHI layer, refactor the entire project (RHI-OpenGL wrapper progress: 50%, RHI-Vulkan wrapper progress: 0%)
- 2024.7.8: GL-Backend WIP
- 2024.7.17: GL-Backend WIP (RHI-OpenGL wrapper progress: 75%)
- 2024.7.26: Vk-Backend WIP
- 2024.8.18: (RHI-OpenGL wrapper progress: 75% / RHI-Vulkan wrapper progress: 75%)

## Reference
- https://github.com/google/filament
- https://github.com/LukasBanana/LLGL
- https://github.com/NazaraEngine/NazaraEngine
- https://github.com/jmorton06/Lumos
- https://github.com/BoomingTech/Piccolo
- https://github.com/carbonsunsu/CarbonRender