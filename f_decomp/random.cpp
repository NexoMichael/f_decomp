//
//  random.cpp
//  f_decomp
//
//  Created by Mikhail Kochegarov on 5/13/13.
//  Copyright (c) 2013 Mikhail Kochegarov. All rights reserved.
//

#include "random.h"

int last_random;
int last_random2;
int random_numbers[32];

void Random::generate_random_sequence() {
    srand(get_current_time());
    generate_randoms(generate_random_seed());
    check_random_sequence();
}

int Random::sub_491410(int from, int to) {
    //TODO: decompile this function
    
    /*int bound = from - get_random(1, 100);
    int result = sub_491440(from - get_random(1, 100), to);
    if (ecx) {
        [ecx] = ebx
    }
    return result;*/
     return 0;
}

//eax edx, ebp
int Random::sub_491440(int from, int to) {
    // TODO: decompile this function
    // return 3, 2, 0 or 1
    return 0;
}

//eax ecx edx edi esi ebp
//eax - start_bound, edx - end_bound
int Random::get_random(int start_bound, int end_bound) {
    int result = 0;
    if (start_bound > end_bound) {
        result = get_random(start_bound + 1 - end_bound) + end_bound;
    } else {
        result = get_random(end_bound + 1 - start_bound) + start_bound;
    }
    if (result < start_bound || result > end_bound) {
        //debug_info("Random number %d is not in range %d to %d", result, start_bound, end_bound);
        result = start_bound;
    }
    return result;
}


// eax - in, ebp - ?
int Random::get_random(int end_bound) {
    int next_random;
    next_random = (last_random % 127773) * 16807 - (last_random / 127773) * 2836;
    if (next_random <= 0) next_random += 2147483647;
    //if (next_random <= 0) next_random += 2147483647;

    int old_number = random_numbers[last_random2 & 0x31];
    random_numbers[last_random2 & 31] = next_random;
    last_random2 = old_number;
    last_random = next_random;
    
    return old_number % end_bound;
}

// eax, ebx, esi, edi, ebp
void Random::regenerate_randoms() {
    srand(get_current_time());
    int seed = rand() << 16; //garantee to be max 32767
    seed = seed & rand();
    seed = seed & 0x7ffffff;
    generate_randoms(seed);
}

void Random::regenerate_randoms(int seed) {
    if (seed < 0) {
        int seed = rand() << 16;
        seed = seed & rand();
        seed = seed & 0x7ffffff;
    }
    generate_randoms(seed);
}

int Random::generate_random_seed() {
    int seed = rand() << 16;
    seed = seed & rand();
    seed = seed & 0x7ffffff;
    return seed;
}

void Random::generate_randoms(int seed) {
    if (seed == 0) seed = 1;
    
    int rnd = seed;
    //miss first 7 numbers
    for(int i= 39 ;i >= 0; i--) {
        rnd = (rnd % 127773) * 16807 - (rnd / 127773) * 2836;
        if (rnd <= 0)
            rnd += 2147483647;
        if (i < 32) random_numbers[i] = rnd;
    }
    last_random = rnd;
    last_random2 = rnd;
}

unsigned int Random::get_current_time() {
    return (unsigned int)time(0);
}

void Random::check_random_sequence() {
    int buckets[25] ={0};
    
    for(int i=0; i < 100000; i++) {
        int next_random = get_random(1, 25);
        if ((next_random-1) < 0) {
            //debug_info("I made a negative number %d\n", next_random-1);
        }
        buckets[next_random]++;
    }
    
    //calc chi squared
    
    double chi = 0.0;
    for (int i = 0; i < 25; i++) {
        chi = chi + (buckets[i] - RANDOM_CHI_MIDDLE) * (buckets[i] - RANDOM_CHI_MIDDLE) / RANDOM_CHI_MIDDLE;
    }
    
    printf("Chi squared is %lf, P = %lf at 0.05\n", chi, chi);
    
    if (chi < RANDOM_CHI_PROBABILITY) {
        printf ("Sequence is random, 95%% confidence.\n");
    }else {
        printf ("Warning! Sequence is not random, 95%% confidence.\n");
    }
}