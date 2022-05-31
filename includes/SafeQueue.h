#ifndef TEPLINSKIH_29_05_2022_THREAD_SAFE_QUEUE_H
#define TEPLINSKIH_29_05_2022_THREAD_SAFE_QUEUE_H
#include <queue>
#include <vector>
#include "Windows.h"
template<class T>
class SafeQueue{
    std::queue<T> values;
    HANDLE event;
public:
    bool empty(){
        return values.empty();
    }
    void push(T value){
        if (WaitForSingleObject(event, INFINITE) == WAIT_OBJECT_0){
            values.push(value);
        }
        SetEvent(event);
    }
    bool try_pop(T& value){
        bool result = false;
        if (WaitForSingleObject(event, INFINITE) == WAIT_OBJECT_0){
            if (!empty()){
                value = values.front();
                result = true;
                values.pop();
            }
        }
        SetEvent(event);
        return result;
    }
    SafeQueue(){
        event = CreateEvent(nullptr, false, true, nullptr);
    }
    ~SafeQueue(){
        CloseHandle(event);
    }
};
#endif //TEPLINSKIH_29_05_2022_THREAD_SAFE_QUEUE_H
