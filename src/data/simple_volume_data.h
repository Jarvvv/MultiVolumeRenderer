//
// Created by wyz on 20-12-4.
//

#ifndef VOLUMERENDER_VOLUME_DATA_H
#define VOLUMERENDER_VOLUME_DATA_H
#include<sv/Data/VolumeData.h>
#include<sv/IO/Util.h>

class SimpleVolumeData:public IVolumeData{
public:
    SimpleVolumeData()=default;

    uint64_t bufferByteCount();
    static auto load(const char* file_name)->std::shared_ptr<IVolumeData>;
    auto getData()->std::vector<uint8_t>&{ return buffer; }
    auto getDim()->std::array<uint32_t,3>&{ return dimensions; }

    std::vector<uint8_t> buffer;
    std::array<uint32_t, 3> dimensions;
};




#endif //VOLUMERENDER_VOLUME_DATA_H
