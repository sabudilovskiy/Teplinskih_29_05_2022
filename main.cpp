#include <iostream>
#include "Thread_pool.h"
int main(){
    setlocale(LC_ALL, "rus");
    Thread_pool tp(2, 2, 20);
    tp.wait_all();
    auto answer = tp.get_answers();
    for (auto& i : answer){
        for (auto& j : i){
            std::cout << j << ' ';
        }
        std::cout << "Длина: " <<  i.size() <<  '\n';
    }
    std::cin.get();
}