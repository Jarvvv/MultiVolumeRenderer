//
// Created by wyz on 20-12-4.
//

#ifndef VOLUMERENDER_BLOCK_VOLUME_MANAGER_H
#define VOLUMERENDER_BLOCK_VOLUME_MANAGER_H
#include <sv/Data/VolumeManager.h>
#include<array>
#include<queue>
#include<thread>
#include<utils/help_cuda.h>
//#include<VoxelCompression/voxel_uncompress/VoxelUncompress.h>
#include<sv/Utils/common.h>
#include<atomic>
#define DECODER_NUM 5
struct BlockDesc{
    std::array<uint32_t,3> block_index;
    CUdeviceptr data;
    int64_t size;
};
struct BlockDataDesc{

};
/**
 *
 */
class BlockVolumeManager: public IVolumeManager{
public:
    BlockVolumeManager();
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

    void setupVolumeData(const char* file_path){volume_data;}
    virtual std::vector<uint8_t>& getVolumeData(){ return volume_data->getData();}
    virtual std::array<uint32_t,3>& getVolumeDim(){return block_dim;}// {return volume_data->getDim();}
public:
    void setupBlockReqInfo() override;
    /**
     * false represent no cached block so consumer should request next time like after 16ms
     * true represent can call getBlock() one more time
     * every request can call getBlocks() no more than DECODER_NUM ?
     */
    bool getBlock() override;

public:
    struct MemoryPool{
        std::vector<CUdeviceptr> m;
        std::vector<std::atomic<bool>> m_status;
    } memory_pool;
private:
    uint32_t block_length;
    uint32_t padding;
    std::array<uint32_t,3> block_dim;
//    VoxelUncompressOptions opts;
//    std::vector<std::unique_ptr<VoxelUncompress>> workers;
    std::atomic<bool> worker_status;
    std::priority_queue<BlockDesc> packages;
    std::mutex mtx;//lock for packages
    std::vector<std::thread> jobs;//can not terminate, guarantee a job if start must finish
                                  //while finish uncompress, will inspect whether add to products
                                  //if the block is still in packages then add, otherwise not
                                  //another inspection is if the products is full
//    std::queue<BlockDesc> products;
    ConcurrentQueue<BlockDesc> products;
};

#endif //VOLUMERENDER_BLOCK_VOLUME_MANAGER_H
