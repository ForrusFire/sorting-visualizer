#ifndef HELPER_HPP
#define HELPER_HPP


#include <string>
#include <vector>
#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>
using namespace std;


struct interval {
    int first;
    int last;
};


unsigned int get_num_rectangles(void);
string get_sort_type(void);
tuple<int, int, int, int, vector<interval>> initialize_sort(string sort_type, int num_rectangles);
void initialize_merge_intervals(vector<interval>& merge_intervals, int start, int end);
sf::RectangleShape setColor(sf::RectangleShape rectangle, string sort_type, string sort_status, 
                            int i, int current_rectangle, int first_sorted_rectangle, int last_sorted_rectangle, int min_rectangle, int num_rectangles);


#endif