//
// Created by wyz on 2021/1/27.
//
#include <iostream>
#include <tool/volume_renderer_factory.h>
//#include<mpi.h>
int main(int argc,char** argv)
{
    std::cout<<"Block Volume Render"<<std::endl;

    VolumeRenderer volumeRenderer=BlockVolumeRenderFactory::CreateBlockVolumeRenderer();

    try{
        volumeRenderer.init();
        volumeRenderer.render();
    }
    catch (const std::exception &err) {
        std::cout<<err.what()<<std::endl;
    }

//    MPI_Init(0,0);

    return 0;
}
