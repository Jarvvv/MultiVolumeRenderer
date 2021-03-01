#include <iostream>
#include <string>
#include <tool/volume_renderer_factory.h>
int main(int argc,char** argv)
{
    std::cout << "Simple Volume Render!" << std::endl;
    const char* filename="aneurism_256_256_256_uint8.raw";

    std::map<uint8_t ,std::array<double,4>> color_map;
    color_map[0]={0.0,0.0,0.0,0.0};
    color_map[114]={127/255.0,63/255.0,27/255.0,0.0};
    color_map[165]={127/255.0,63/255.0,27/255.0,0.6};
    color_map[216]={127/255.0,63/255.0,27/255.0,0.3};
    color_map[255]={0.0,0.0,0.0,0.0};



    try{
        VolumeRenderer volumeRenderer=SimpleVolumeRendererFactory::CreateSimpleVolumeRenderer();
        volumeRenderer.setupResource(filename,color_map);
        volumeRenderer.init();
        volumeRenderer.render();
    }
    catch (const std::exception &err) {
        std::cout<<err.what()<<std::endl;
    }
    return 0;
}
