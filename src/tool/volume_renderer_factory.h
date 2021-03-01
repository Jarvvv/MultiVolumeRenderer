//
// Created by wyz on 20-12-5.
//

#ifndef VOLUMERENDERER_VOLUME_RENDERER_FACTORY_H
#define VOLUMERENDERER_VOLUME_RENDERER_FACTORY_H
#include<render/simple_volume_renderer.h>
#include<render/block_volume_renderer.h>
#include<render/volume_renderer.h>
class IVolumeRendererFactory{
public:
    IVolumeRendererFactory()=delete;
};
class SimpleVolumeRendererFactory: public IVolumeRendererFactory{
public:
    SimpleVolumeRendererFactory()=delete;
    static VolumeRenderer CreateSimpleVolumeRenderer();

};

inline VolumeRenderer SimpleVolumeRendererFactory::CreateSimpleVolumeRenderer()
{
    SimpleVolumeRenderer* simple_volume_renderer=new SimpleVolumeRenderer();
    return VolumeRenderer(simple_volume_renderer);
}

class BlockVolumeRenderFactory: public IVolumeRendererFactory{
public:
    BlockVolumeRenderFactory()=delete;
    static VolumeRenderer CreateBlockVolumeRenderer();
};

inline VolumeRenderer BlockVolumeRenderFactory::CreateBlockVolumeRenderer()
{
    BlockVolumeRenderer* block_volume_renderer=new BlockVolumeRenderer();
    return VolumeRenderer(block_volume_renderer);
}

#endif //VOLUMERENDERER_VOLUME_RENDERER_FACTORY_H
