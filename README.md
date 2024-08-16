## NoobRenderer-pluto 🌑 
In this pluto branch, the first step will be to add RHI (Render Hardware Interface) layer to implement abstraction for Opengl and Vulkan, then refactor the entire project.

```shell
  ☼ <- now in here! (still developing in my spare time, so it may not compile successfully
  |
Pluto   Neptune  Uranus  Saturn  Jupiter  Mars  Earth   Venus   Mercury   
  🌑       🔵         ...          🪐     🔴    🌍        ...      
       
```

(:The next version will be named as [**Neptune**](#))
## Pluto
(:d still developed!
(:d progress can be found in [pluto-dev](https://github.com/shuaibo919/NoobRenderer/tree/pluto-dev), [pluto-gl-dev](https://github.com/shuaibo919/NoobRenderer/tree/pluto-gl-dev)  and [pluto-vk-dev](https://github.com/shuaibo919/NoobRenderer/tree/pluto-vk-dev)

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


## Reference
- https://github.com/google/filament
- https://github.com/LukasBanana/LLGL
- https://github.com/NazaraEngine/NazaraEngine
- https://github.com/jmorton06/Lumos
- https://github.com/BoomingTech/Piccolo
- https://github.com/carbonsunsu/CarbonRender
