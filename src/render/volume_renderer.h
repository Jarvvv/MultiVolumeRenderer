//
// Created by wyz on 20-12-4.
//

#ifndef VOLUMERENDER_VOLUME_RENDERER_H
#define VOLUMERENDER_VOLUME_RENDERER_H
#include<memory>
#include<map>
#include<sv/Render/VolumeRenderer.h>
class VolumeRenderer{
public:
    VolumeRenderer(IVolumeRenderer* renderer){impl.reset(renderer);}

    void setupResource(const char* volume_file_path,std::map<uint8_t,std::array<double,4>> color_setting);
    void init();
    void render();

private:
    std::unique_ptr<IVolumeRenderer> impl;
};




#endif //VOLUMERENDER_VOLUME_RENDERER_H
