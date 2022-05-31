#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include "SafeQueue.h"
#include "Random.h"

void producer(SafeQueue<std::vector<int>>& que, std::atomic<long>& work_producer )
{
    while (work_producer.load() > 0){
        work_producer.fetch_sub(1);
        std::vector<int> vector;
        vector.resize(20);
        for (auto& i : vector){
            i = my_random_int(-10, 10);
        }
        que.push(vector);
    }
}
void consumer(SafeQueue<std::vector<int>>& que, std::atomic<long>& work_comsumer, SafeQueue<std::vector<int>>& answer){
    std::vector<std::vector<int>> local_answer;
    while (work_comsumer.load() > 0){
        std::vector<int> temp;
        if (que.try_pop(temp)){
            work_comsumer.fetch_sub(1);
            std::sort(temp.begin(),  temp.end());
            answer.push(temp);
        }
    }
}
void task(size_t count_producers, size_t count_consumers, size_t count_vectors){
    std::vector<std::thread> producers, consumers;
    SafeQueue<std::vector<int>> que, answer;
    std::atomic<long> work1 = count_vectors, work2 = count_vectors;
    producers.resize(count_producers);
    consumers.resize(count_consumers);
    for (int i = 0; i < count_producers; i++){
        producers[i] = std::thread(producer, std::ref(que), std::ref(work1));
    }
    for (int i = 0; i < count_consumers; i++){
        consumers[i] = std::thread(consumer, std::ref(que), std::ref(work2), std::ref(answer));
    }
    for (int i = 0; i < count_producers; i++){
        producers[i].join();
    }
    for (int i = 0; i < count_consumers; i++){
        consumers[i].join();
    }
    while (!answer.empty()){
        std::vector<int> temp;
        answer.try_pop(temp);
        for (auto i : temp){
            std::cout << i << ' ';
        }
        std::cout << '\n';
    }
}
int main(){
    task(4, 4, 10);
    std::cin.get();
}