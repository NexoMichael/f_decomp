//
//  random.h
//  f_decomp
//
//  Created by Mikhail Kochegarov on 5/13/13.
//  Copyright (c) 2013 Mikhail Kochegarov. All rights reserved.
//

#ifndef __f_decomp__random__
#define __f_decomp__random__

#include <iostream>

#define RANDOM_CHI_PROBABILITY  36.415
#define RANDOM_CHI_MIDDLE       4000.0


// from BEGTEXT:004913F0 till 0049173B
class Random {
public:
    
    // BEGTEXT:004913F0
    static void generate_random_sequence();
    
    // BEGTEXT:00491410
    static int sub_491410(int from, int to);
    
    // BEGTEXT:00491440
    static int sub_491440(int from, int to);
    
    // BEGTEXT:004914D0
    static int get_random(int start_bound, int end_bound);
    
    // BEGTEXT::0049150C
    static int get_random(int end_bound);
    
    // BEGTEXT:00491584
    static void regenerate_randoms();

    // BEGTEXT:004915B0
    static void regenerate_randoms(int seed);
    
    // BEGTEXT:004915D4
    static int generate_random_seed();
    
    // BEGTEXT:004915F0
    static void generate_randoms(int seed);
    
    // BEGTEXT:00491668
    static unsigned int get_current_time();
    
    // BEGTEXT:00491674
    static void check_random_sequence();
    
    // BEGTEXT:00491748
    static void sub_491748();

    // BEGTEXT:004917D8
    static void sub_4917D8();
};


#endif /* defined(__f_decomp__random__) */
