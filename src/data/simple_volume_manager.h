//
// Created by wyz on 20-12-4.
//

#ifndef VOLUMERENDER_SIMPLE_VOLUME_MANAGER_H
#define VOLUMERENDER_SIMPLE_VOLUME_MANAGER_H
#include<sv/Data/VolumeManager.h>
#include<data/simple_volume_data.h>
#include<io/RawVolumeReader.h>
class SimpleVolumeManager: public IVolumeManager{
public:
    void setupTransferFunc(std::map<uint8_t,std::array<double,4> > color_setting){
        if(!tf.get())
            tf=std::make_unique<sv::TransferFunc>(color_setting);
        else
            tf->resetTransferFunc(color_setting);
    }
    auto getTransferFunc(bool preInt=false)->std::vector<float>&{
        if(!preInt)
            return tf->getTransferFunction();
        else
            return tf->getPreIntTransferFunc();
    }

    void setupVolumeData(const char* file_path){ volume_data=SimpleVolumeData::load(file_path);};
    virtual std::vector<uint8_t>& getVolumeData(){ return volume_data->getData();};
    virtual std::array<uint32_t,3>& getVolumeDim() {return volume_data->getDim();}
public:

private:

};
#endif //VOLUMERENDER_SIMPLE_VOLUME_MANAGER_H
