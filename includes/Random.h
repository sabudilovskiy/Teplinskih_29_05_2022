//
// Created by MRV on 5/29/2022.
//

#ifndef TEPLINSKIH_29_05_2022_RANDOM_H
#define TEPLINSKIH_29_05_2022_RANDOM_H
#include <random>
int my_random_int(int a, int b) {
    if (b!=a){
        std::random_device rd;
        std::mt19937_64 mersenne(rd());
        std::uniform_int_distribution<int> uid(a, b);
        return uid(mersenne);
    }
    else return b;
}
#endif //TEPLINSKIH_29_05_2022_RANDOM_H
