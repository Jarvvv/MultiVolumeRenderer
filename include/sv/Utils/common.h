//
// Created by wyz on 2021/2/26.
//

#ifndef VOLUMERENDERER_COMMON_H
#define VOLUMERENDERER_COMMON_H

#include<chrono>
#include<iostream>
#include<list>
#include<condition_variable>
#include<mutex>
#define START_CPU_TIMER \
{auto _start=std::chrono::steady_clock::now();

#define END_CPU_TIMER \
auto _end=std::chrono::steady_clock::now();\
auto _t=std::chrono::duration_cast<std::chrono::milliseconds>(_end-_start);\
std::cout<<"CPU cost time : "<<_t.count()<<"ms"<<std::endl;}


#define START_CUDA_DRIVER_TIMER \
CUevent start,stop;\
float elapsed_time;\
cuEventCreate(&start,CU_EVENT_DEFAULT);\
cuEventCreate(&stop,CU_EVENT_DEFAULT);\
cuEventRecord(start,0);

#define STOP_CUDA_DRIVER_TIMER \
cuEventRecord(stop,0);\
cuEventSynchronize(stop);\
cuEventElapsedTime(&elapsed_time,start,stop);\
cuEventDestroy(start);\
cuEventDestroy(stop);\
std::cout<<"GPU cost time: "<<elapsed_time<<"ms"<<std::endl;


#define START_CUDA_RUNTIME_TIMER \
{cudaEvent_t     start, stop;\
float   elapsedTime;\
(cudaEventCreate(&start)); \
(cudaEventCreate(&stop));\
(cudaEventRecord(start, 0));

#define STOP_CUDA_RUNTIME_TIMER \
(cudaEventRecord(stop, 0));\
(cudaEventSynchronize(stop));\
(cudaEventElapsedTime(&elapsedTime, start, stop)); \
printf("\tGPU cost time used: %.f ms\n", elapsedTime);\
(cudaEventDestroy(start));\
(cudaEventDestroy(stop));}

template <class T>
inline void print(T t)
{
    std::cout<<t<<std::endl;
}

template <class T,class ...Args>
inline void print(T t,Args... args)
{
    std::cout<<t<<" ";
    print(args...);
}

template<typename T>
class ConcurrentQueue
{
public:

    ConcurrentQueue() {}
    ConcurrentQueue(size_t size) : maxSize(size) {}
    ConcurrentQueue(const ConcurrentQueue&) = delete;
    ConcurrentQueue& operator=(const ConcurrentQueue&) = delete;

    void setSize(size_t s) {
        maxSize = s;
    }

    void push_back(const T& value) {
        // Do not use a std::lock_guard here. We will need to explicitly
        // unlock before notify_one as the other waiting thread will
        // automatically try to acquire mutex once it wakes up
        // (which will happen on notify_one)
        std::unique_lock<std::mutex> lock(m_mutex);
        auto wasEmpty = m_List.empty();

        while (full()) {
            m_cond.wait(lock);
        }

        m_List.push_back(value);
        if (wasEmpty && !m_List.empty()) {
            lock.unlock();
            m_cond.notify_one();
        }
    }

    T pop_front() {
        std::unique_lock<std::mutex> lock(m_mutex);

        while (m_List.empty()) {
            m_cond.wait(lock);
        }
        auto wasFull = full();
        T data = std::move(m_List.front());
        m_List.pop_front();

        if (wasFull && !full()) {
            lock.unlock();
            m_cond.notify_one();
        }

        return data;
    }

    T front() {
        std::unique_lock<std::mutex> lock(m_mutex);

        while (m_List.empty()) {
            m_cond.wait(lock);
        }

        return m_List.front();
    }

    size_t size() {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_List.size();
    }

    bool empty() {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_List.empty();
    }
    void clear() {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_List.clear();
    }

private:
    bool full() {
        if (m_List.size() == maxSize)
            return true;
        return false;
    }

private:
    std::list<T> m_List;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    size_t maxSize;
};


#endif //VOLUMERENDERER_COMMON_H
