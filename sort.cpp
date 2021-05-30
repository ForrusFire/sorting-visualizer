#include "sort.hpp"


/********** Sorting Algos **********/

// Bubble sort
void bubble_sort(float rectangle_heights[], int& current_rectangle, int& last_sorted_rectangle, string& sort_status) {
    // Swap the current rectangles if necessary
    if (rectangle_heights[current_rectangle] > rectangle_heights[current_rectangle + 1]) {
        swap_rectangles(rectangle_heights, current_rectangle, current_rectangle + 1);
    }

    // Move on to the next rectangles
    if (current_rectangle + 1 == last_sorted_rectangle - 1) {
        // If at the end, start from the front
        last_sorted_rectangle--;
        if (last_sorted_rectangle == 1) {
            // End sort
            sort_status = "Sorted";
            cout << "\nSort complete!";
        }
        current_rectangle = 0;
    } else {
        current_rectangle++;
    }
}


// Selection sort
void selection_sort(float rectangle_heights[], int& current_rectangle, int& last_sorted_rectangle, int& min_rectangle, int num_rectangles, string& sort_status) {
    // Set the minimum rectangle
    if (rectangle_heights[current_rectangle] < rectangle_heights[min_rectangle] && current_rectangle != num_rectangles) {
        min_rectangle = current_rectangle;
    }

    // Move on to the next rectangle
    if (current_rectangle == num_rectangles) {
        // If at the end, swap the minimum rectangle with the rectangle right after the sorted set
        swap_rectangles(rectangle_heights, min_rectangle, last_sorted_rectangle + 1);

        last_sorted_rectangle++;
        if (last_sorted_rectangle == num_rectangles - 1) {
            // End sort
            sort_status = "Sorted";
            cout << "\nSort complete!";
        }

        current_rectangle = last_sorted_rectangle + 1;
        min_rectangle = current_rectangle;
    } else {
        current_rectangle++;
    }
}


// Insertion sort
void insertion_sort(float rectangle_heights[], int& current_rectangle, int& last_sorted_rectangle, int num_rectangles, string& sort_status) {
    // Swap the current rectangles if necessary
    if (rectangle_heights[current_rectangle] < rectangle_heights[current_rectangle - 1]) {
        swap_rectangles(rectangle_heights, current_rectangle - 1, current_rectangle);

        // Move on to next rectangles
        current_rectangle--;
        if (current_rectangle == 0) {
            last_sorted_rectangle++;
            if (last_sorted_rectangle == num_rectangles - 1) {
                // End sort
                sort_status = "Sorted";
                cout << "\nSort complete!";
            }
            current_rectangle = last_sorted_rectangle + 1;
        }
    } else {
        // Move on to next rectangles
        last_sorted_rectangle++;
        if (last_sorted_rectangle == num_rectangles - 1) {
            // End sort
            sort_status = "Sorted";
            cout << "\nSort complete!";
        }

        current_rectangle = last_sorted_rectangle + 1;
    }
}


// Merge sort
void merge_sort(float rectangle_heights[], int& current_rectangle, int& first_sorted_rectangle, int& last_sorted_rectangle,   
                vector<float>& sorted_heights, vector<interval>& merge_intervals, string& sort_status) {
    // Change height of current rectangle
    if (current_rectangle >= first_sorted_rectangle) {
        rectangle_heights[current_rectangle] = sorted_heights[0];
        
        if (sorted_heights.size() != 0) {
            sorted_heights.erase(sorted_heights.begin());
        }
    }
    
    if (current_rectangle == last_sorted_rectangle) {
        if (merge_intervals.size() == 0) {
            // End sort
            sort_status = "Sorted";
            cout << "\nSort complete!";
        } else {
            // Set new first sorted and last sorted
            first_sorted_rectangle = merge_intervals[0].first;
            last_sorted_rectangle = merge_intervals[0].last;
            current_rectangle = first_sorted_rectangle - 1;
            // Remove the interval from the merge intervals vector
            merge_intervals.erase(merge_intervals.begin());

            // Create new sorted heights
            for (int i = first_sorted_rectangle; i <= last_sorted_rectangle; i++) {
                sorted_heights.push_back(rectangle_heights[i]);
            }
            sort(sorted_heights.begin(), sorted_heights.end());
        }
    } else {
        current_rectangle++;
    }
}


// Swap the positions of two rectangles
void swap_rectangles(float rectangle_heights[], int pos1, int pos2) {
    float temp = rectangle_heights[pos1];
    rectangle_heights[pos1] = rectangle_heights[pos2];
    rectangle_heights[pos2] = temp;
    return;
}
