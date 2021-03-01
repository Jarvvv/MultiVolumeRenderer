//
// Created by wyz on 2021/2/27.
//
#include<data/block_volume_manager.h>
//#include<VoxelCompression/voxel_uncompress/VoxelUncompress.h>
BlockVolumeManager::BlockVolumeManager()
{

}

void BlockVolumeManager::setupBlockReqInfo()
{
    //atomic access packages
    mtx.lock();
    //update packages
    //insert new need and delete no need
    mtx.unlock();

    //find workers not busy and empty memory buffer from memory pool
    //for every worker
    //jobs=create_job(packages.pop(),worker)

}

bool BlockVolumeManager::getBlock()
{
    if(products.empty())
        return false;
    else{
        auto p=products.pop_front();
        return true;
    }
}

