#ifndef SORT_HPP
#define SORT_HPP


#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "helper.hpp"
using namespace std;


void bubble_sort(vector<float>& rectangle_heights, int& current_rectangle, int& last_sorted_rectangle, sortStatus& sort_status);
void selection_sort(vector<float>& rectangle_heights, int& current_rectangle, int& last_sorted_rectangle, int& min_rectangle, int num_rectangles, sortStatus& sort_status);
void insertion_sort(vector<float>& rectangle_heights, int& current_rectangle, int& last_sorted_rectangle, int num_rectangles, sortStatus& sort_status);
void merge_sort(vector<float>& rectangle_heights, int& current_rectangle, int& first_sorted_rectangle, int& last_sorted_rectangle,   
                vector<float>& sorted_heights, vector<interval>& merge_intervals, sortStatus& sort_status);


void swap_rectangles(vector<float>& rectangle_heights, int pos1, int pos2);


#endif