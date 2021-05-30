#ifndef HELPER_HPP
#define HELPER_HPP


#include <string>
#include <vector>
#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>
using namespace std;


struct interval {
    int start;
    int end;
};

enum sortType {
    bubble,
    selection,
    insertion,
    merge,
    none
};

enum sortStatus {
    unsorted,
    sorting,
    sorted
};


unsigned int get_num_rectangles(void);
sortType get_sort_type(void);

tuple<int, int, int, int, vector<interval>> initialize_sort(sortType sort_type, int num_rectangles);
void initialize_merge_intervals(vector<interval>& merge_intervals, int start, int end);
sf::RectangleShape setColor(sf::RectangleShape rectangle, sortType sort_type, sortStatus sort_status, 
                            int i, int current_rectangle, int first_sorted_rectangle, int last_sorted_rectangle, int min_rectangle, int num_rectangles);


#endif