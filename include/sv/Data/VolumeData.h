//
// Created by wyz on 20-12-4.
//

#ifndef VOLUMERENDER_VOLUMEDATA_H
#define VOLUMERENDER_VOLUMEDATA_H
#include<memory>
#include<vector>
class IVolumeData: public std::enable_shared_from_this<IVolumeData>{
public:
    IVolumeData()=default;
    virtual auto getData() -> std::vector<uint8_t> & = 0 ;
    virtual auto getDim() -> std::array<uint32_t,3> & = 0;
private:

};

#endif //VOLUMERENDER_VOLUMEDATA_H
