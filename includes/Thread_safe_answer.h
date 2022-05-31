//
// Created by MRV on 5/29/2022.
//

#ifndef TEPLINSKIH_29_05_2022_THREAD_SAFE_ANSWER_H
#define TEPLINSKIH_29_05_2022_THREAD_SAFE_ANSWER_H
#include <vector>
#include "Windows.h"
class Thread_safe_answer{;
    HANDLE event;
    std::vector<std::vector<int>> values;
public:
    Thread_safe_answer(){
        event = CreateEvent(nullptr, false, true, nullptr);
    }
    ~Thread_safe_answer(){
        CloseHandle(event);
    }
    void push(std::vector<int> value){
        if (WaitForSingleObject(event, INFINITE) == WAIT_OBJECT_0){
            values.push_back(value);
        }
        SetEvent(event);
    }
    std::vector<std::vector<int>> get_answers(){
        std::vector<std::vector<int>> answers;
        if (WaitForSingleObject(event, INFINITE) == WAIT_OBJECT_0) answers = values;
        SetEvent(event);
        return answers;
    }
};
#endif //TEPLINSKIH_29_05_2022_THREAD_SAFE_ANSWER_H
