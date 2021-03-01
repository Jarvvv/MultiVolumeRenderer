//
// Created by wyz on 20-12-4.
//
#include"volume_renderer.h"

void VolumeRenderer::setupResource(const char *volume_file_path, std::map<uint8_t, std::array<double,4>> color_setting)
{
    impl->setupVolume(volume_file_path);
    impl->setupTransferFunc(color_setting);
}
void VolumeRenderer::init()
{
    impl->init();
}
void VolumeRenderer::render()
{
    impl->render();
}