//
// Created by wyz on 20-12-8.
//

#ifndef VOLUMERENDERER_RAWVOLUMEREADER_H
#define VOLUMERENDERER_RAWVOLUMEREADER_H

#include<sv/IO/VolumeReader.h>
#include<data/simple_volume_data.h>
#include<fstream>
#include<iterator>
#include<sv/IO/Util.h>
/**
 * raw volume reader is just for simple volume data
 */
class RawVolumeReader: public IVolumeReader{
public:
    explicit RawVolumeReader(const char* file_name);
    auto read()->std::unique_ptr<IVolumeData>;
private:
    std::fstream in;
    std::array<uint32_t, 3> dimensions;
};
inline RawVolumeReader::RawVolumeReader(const char *file_name)
{
    const char *name_rule = "raw data should be named as `name_x_y_z_datatype`, "
                            "for example: isabel_500_500_100_uint8";
    auto filename = sv::get_file_name(file_name);
    auto splited = sv::split(filename, "_");
    if (splited.size() < 5) {
        throw std::runtime_error(name_rule);
    }

    in.open(file_name, std::ios::binary|std::ios::in);
    in.unsetf(std::ios::skipws);
    if (!in.is_open()) {
        throw std::runtime_error(std::string("cannot load file: ") + file_name);
    }

    for (int i = 0; i < 3; i++) {
        auto &dim = splited[splited.size() - 4 + i];
        dimensions[i] = strtoul(dim.c_str(), nullptr, 10);
        if (dimensions[i] == 0) {
            throw std::runtime_error(name_rule);
        }
    }

    auto &type = splited[splited.size() - 1];
    if (type != "uint8") {
        throw std::runtime_error(std::string("unsupported value type: ") + type);
    }
}
inline auto RawVolumeReader::read() -> std::unique_ptr<IVolumeData>
{
    auto simple_volume_data=std::make_unique<SimpleVolumeData>();

    simple_volume_data->dimensions=dimensions;
    simple_volume_data->buffer.reserve(simple_volume_data->bufferByteCount());
    simple_volume_data->buffer.insert(simple_volume_data->buffer.begin(),
                                      std::istream_iterator<uint8_t>(in),
                                              std::istream_iterator<uint8_t>());
    return simple_volume_data;
}


#endif //VOLUMERENDERER_RAWVOLUMEREADER_H
