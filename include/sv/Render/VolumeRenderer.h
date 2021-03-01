//
// Created by wyz on 20-12-4.
//

#ifndef VOLUMERENDER_VOLUMERENDERER_H
#define VOLUMERENDER_VOLUMERENDERER_H
#include<map>
#include<sv/Data/VolumeManager.h>
class IVolumeRenderer{
public:
    virtual void setupVolume(const char* file_path)=0;
    virtual void setupTransferFunc(std::map<uint8_t,std::array<double,4>> color_setting)=0;
    virtual void init()=0;
    virtual void render()=0;
protected:
    virtual void setupController()=0;
protected:
    std::unique_ptr<IVolumeManager> volume_manager;
};
#endif //VOLUMERENDER_VOLUMERENDERER_H
