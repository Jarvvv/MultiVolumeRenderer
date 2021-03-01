//
// Created by wyz on 2021/2/28.
//
#include<iostream>
#include<sv/Utils/boundingbox.h>
using namespace sv;
int main(int argc,char** argv)
{
    OBB obb_0({0.f,0.f,0.f},{1.f,0.f,0.f},{0.f,1.f,0.f},{0.f,0.f,1.f},1.f,1.f,1.f);

    OBB obb_1({2.4f,0.0f,0.0f},{1.f,0.f,-1.f},{0.f,1.f,0.f},{1.f,0.f,1.f},1.f,1.f,1.f);

    if(obb_0.intersect_obb(obb_1)){
        assert(obb_1.intersect_obb(obb_0));
        std::cout<<"intersect!!!"<<std::endl;
    }
    else{
        if(obb_1.intersect_obb(obb_0)){
            std::cout<<"error!!! bug"<<std::endl;
        }
        else{
            std::cout<<"not intersect"<<std::endl;
        }
    }

    return 0;
}