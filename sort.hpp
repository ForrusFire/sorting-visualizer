#ifndef SORT_HPP
#define SORT_HPP


#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "helper.hpp"
using namespace std;


void bubble_sort(float rectangle_heights[], int& current_rectangle, int& last_sorted_rectangle, string& sort_status);
void selection_sort(float rectangle_heights[], int& current_rectangle, int& last_sorted_rectangle, int& min_rectangle, int num_rectangles, string& sort_status);
void insertion_sort(float rectangle_heights[], int& current_rectangle, int& last_sorted_rectangle, int num_rectangles, string& sort_status);
void merge_sort(float rectangle_heights[], int& current_rectangle, int& first_sorted_rectangle, int& last_sorted_rectangle,   
                vector<float>& sorted_heights, vector<interval>& merge_intervals, string& sort_status);


void swap_rectangles(float rectangle_heights[], int pos1, int pos2);


#endif