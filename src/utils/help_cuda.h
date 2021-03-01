//
// Created by wyz on 2021/2/26.
//

#ifndef VOLUMERENDERER_HELP_CUDA_H
#define VOLUMERENDERER_HELP_CUDA_H
#include<cuda.h>
#include<cuda_runtime.h>
inline bool check(CUresult e, int iLine, const char *szFile) {
    if (e != CUDA_SUCCESS) {
        const char *szErrName = NULL;
        cuGetErrorName(e, &szErrName);
        std::cout << "CUDA driver API error: " << szErrName << " at line " << iLine << " in file " << szFile;
        return false;
    }
    return true;
}
inline bool check(cudaError_t e,int line,const char* file){
    if(e!=cudaSuccess){
        const char* error_name=nullptr;
        error_name=cudaGetErrorName(e);
        std::cout<<"CUDA runtime API error: "<<error_name<<" at line "<<line<<" in file "<<file<<std::endl;
        return false;
    }
    return true;
}

#define CUDA_DRIVER_API_CALL(call) check(call, __LINE__, __FILE__)
#define checkCUDAErrors(call) check(call, __LINE__, __FILE__)

#define CUDA_RUNTIME_API_CALL(call) check(call,__LINE__,__FILE__)

#endif //VOLUMERENDERER_HELP_CUDA_H
