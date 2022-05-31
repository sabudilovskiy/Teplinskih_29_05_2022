//
// Created by MRV on 5/29/2022.
//

#ifndef TEPLINSKIH_29_05_2022_THREAD_SAFE_QUEUE_H
#define TEPLINSKIH_29_05_2022_THREAD_SAFE_QUEUE_H
#include <queue>
#include <vector>
#include <functional>
#include "Windows.h"
class Thread_safe_queue{
    std::queue<std::vector<int>> values;
    std::queue<std::function<void(std::vector<int>&)>> actions;
    HANDLE event;
public:
    bool empty(){
        return values.empty() && actions.empty();
    }
    void push(std::vector<int> value, std::function<void(std::vector<int>&)>action ){
        if (WaitForSingleObject(event, INFINITE) == WAIT_OBJECT_0){
            values.push(value);
            actions.push(action);
        }
        SetEvent(event);
    }
    bool try_pop(std::vector<int>& value, std::function<void(std::vector<int>&)>& action ){
        bool result = false;
        if (WaitForSingleObject(event, INFINITE) == WAIT_OBJECT_0){
            if (!empty()){
                value = values.front();
                action = actions.front();
                result = true;
                actions.pop();
                values.pop();
            }
        }
        SetEvent(event);
        return result;
    }
    Thread_safe_queue(){
        event = CreateEvent(nullptr, false, true, nullptr);
    }
    ~Thread_safe_queue(){
        CloseHandle(event);
    }
};
#endif //TEPLINSKIH_29_05_2022_THREAD_SAFE_QUEUE_H
