//
// Created by wyz on 20-12-8.
//

#ifndef VOLUMERENDERER_VOLUMEREADER_H
#define VOLUMERENDERER_VOLUMEREADER_H
#include<sv/Data/VolumeData.h>
class IVolumeReader{
public:
    virtual auto read()->std::unique_ptr<IVolumeData> = 0;
};

#endif //VOLUMERENDERER_VOLUMEREADER_H
