//
// Created by MRV on 5/29/2022.
//

#ifndef TEPLINSKIH_29_05_2022_THREAD_POOL_H
#define TEPLINSKIH_29_05_2022_THREAD_POOL_H
#include "Thread_safe_queue.h"
#include "Thread_safe_answer.h"
#include "Random.h"
#include <thread>
void task1_consumer(std::vector<int>& vector){
    std::sort(vector.begin(), vector.end());
}
void task2_consumer(std::vector<int>& vector){
    std::vector<int> remove_series;
    remove_series.push_back(vector[0]);
    for (int i = 1; i < vector.size(); i++){
        if (remove_series[remove_series.size() - 1] != vector[i]){
            remove_series.push_back(vector[i]);
        }
    }
    vector = remove_series;
}
class Thread_pool{
    Thread_safe_queue que;
    Thread_safe_answer answer;
    size_t consumers;
    size_t producers;
    long volatile work_consumers;
    long volatile work_producers;
    void thread_consumer(size_t ID){
        std::vector<std::vector<int>> local_answer;
        while (_InterlockedExchangeAdd(&work_consumers, -1) > 0){
            if (!que.empty()){
                std::vector<int> answer;
                std::function<void(std::vector<int>&)> action;
                if (que.try_pop(answer, action)){
                    action(answer);
                    local_answer.push_back(answer);
                }
                else _InterlockedExchangeAdd(&work_consumers, 1);
            }
            else _InterlockedExchangeAdd(&work_consumers, 1);
        }
        for (auto& i : local_answer){
            answer.push(i);
        }
    }
    void thread_producer(size_t ID){
        while (_InterlockedExchangeAdd(&work_producers, -1) > 0){
            std::vector<int> value;
            for (int i = 0; i < 40; i++){
                value.push_back(my_random_int(-10, 10));
            }
            if (ID % 2 == 0){
                que.push(value, task1_consumer);
            }
            else{
                que.push(value, task2_consumer);
            }
        }
    }

public:
    Thread_pool(size_t producers, size_t consumers, long work) : producers(producers), consumers(consumers), work_consumers(work), work_producers(work){
        std::vector<std::thread> threads_producers;
        std::vector<std::thread> threads_consumers;
        threads_producers.reserve(producers);
        threads_consumers.reserve(consumers);
        for (int i = 0; i < producers; i++){
            threads_producers.emplace_back(&Thread_pool::thread_producer, this, i);
            threads_producers[i].detach();
        }
        for (int i = 0; i < consumers; i++){
            threads_consumers.emplace_back(&Thread_pool::thread_consumer, this, i);
            threads_consumers[i].detach();
        }
    }
    std::vector<std::vector<int>> get_answers(){
        return answer.get_answers();
    }
    void wait_all(){
        while (_InterlockedExchangeAdd(&work_producers, 0) > 0){
            std::this_thread::sleep_for(std::chrono::seconds (1));
        }
    }
};
#endif //TEPLINSKIH_29_05_2022_THREAD_POOL_H
