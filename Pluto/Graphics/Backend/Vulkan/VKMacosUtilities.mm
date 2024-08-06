#import "Graphics/Backend/Vulkan/VKMacosUtilities.h"
#import <Cocoa/Cocoa.h>
#include <QuartzCore/CAMetalLayer.h>
void* GetCAMetalLayer(void* handle)
{
    NSWindow* window = (NSWindow*)handle;
    NSView* view = window.contentView;
    
    if (![view.layer isKindOfClass:[CAMetalLayer class]])
    {
        [view setLayer:[CAMetalLayer layer]];
        [view setWantsLayer:YES];
        [view.layer setContentsScale:[window backingScaleFactor]];
    }
    
    return view.layer;
}