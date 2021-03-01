//
// Created by wyz on 20-12-8.
//
#include"simple_volume_data.h"
#include<io/RawVolumeReader.h>
uint64_t SimpleVolumeData::bufferByteCount()
{
    return buffer.size();
}

auto SimpleVolumeData::load(const char *file_name) -> std::shared_ptr<IVolumeData>
{
    auto ext=sv::get_file_extension(file_name);
    if(ext==".raw"){
        RawVolumeReader reader(file_name);
        return reader.read();
    }

    throw std::runtime_error(std::string("unsupported dataset extension: ") + ext);
}