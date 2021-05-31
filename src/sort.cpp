#include "sort.hpp"


/********** Sorting Algos **********/

// Bubble sort
void bubble_sort(vector<float>& rectangle_heights, int& current_rectangle, int& last_sorted_rectangle, sortStatus& sort_status) {
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
            sort_status = sorted;
            cout << "\nSort complete!";
            return;
        }
        current_rectangle = 0;
    } else {
        current_rectangle++;
    }
}


// Selection sort
void selection_sort(vector<float>& rectangle_heights, int& current_rectangle, int& last_sorted_rectangle, int& min_rectangle, int num_rectangles, sortStatus& sort_status) {
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
            sort_status = sorted;
            cout << "\nSort complete!";
            return;
        }

        current_rectangle = last_sorted_rectangle + 1;
        min_rectangle = current_rectangle;
    } else {
        current_rectangle++;
    }
}


// Insertion sort
void insertion_sort(vector<float>& rectangle_heights, int& current_rectangle, int& last_sorted_rectangle, int num_rectangles, sortStatus& sort_status) {
    // Swap the current rectangles if necessary
    if (rectangle_heights[current_rectangle] < rectangle_heights[current_rectangle - 1]) {
        swap_rectangles(rectangle_heights, current_rectangle - 1, current_rectangle);

        // Move on to next rectangles
        current_rectangle--;
        if (current_rectangle != 0) {
            return;
        }
    }

    // Sort next set of rectangles
    last_sorted_rectangle++;
    if (last_sorted_rectangle == num_rectangles - 1) {
        // End sort
        sort_status = sorted;
        cout << "\nSort complete!";
        return;
    }

    current_rectangle = last_sorted_rectangle + 1;
}


// Merge sort
void merge_sort(vector<float>& rectangle_heights, int& current_rectangle, int& first_sorted_rectangle, int& last_sorted_rectangle,   
                vector<float>& sorted_heights, vector<interval>& merge_intervals, sortStatus& sort_status) {
    // Change height of current rectangle
    if (current_rectangle >= first_sorted_rectangle) {
        rectangle_heights[current_rectangle] = sorted_heights[0];
        
        if (sorted_heights.size() != 0) {
            sorted_heights.erase(sorted_heights.begin());
        }
    }
    
    // Move current rectangle to next one
    if (current_rectangle != last_sorted_rectangle) {
        current_rectangle++;
        return;
    }

    // Move on to next interval
    // If there are no more intervals, end sort
    if (merge_intervals.size() == 0) {
        sort_status = sorted;
        cout << "\nSort complete!";
        return;
    }
    
    // Otherwise, set new first sorted and last sorted
    first_sorted_rectangle = merge_intervals[0].start;
    last_sorted_rectangle = merge_intervals[0].end;
    current_rectangle = first_sorted_rectangle - 1;
    // Remove the interval from the merge intervals vector
    merge_intervals.erase(merge_intervals.begin());

    // Create new sorted heights
    for (int i = first_sorted_rectangle; i <= last_sorted_rectangle; i++) {
        sorted_heights.push_back(rectangle_heights[i]);
    }
    sort(sorted_heights.begin(), sorted_heights.end());
}


// Quick sort
void quick_sort(vector<float>& rectangle_heights, vector<int>& sorted_rectangles, vector<interval>& sort_intervals,
                int& current_rectangle, int& first_sorted_rectangle, int& last_sorted_rectangle, int& pivot, int& cutoff, bool& swap_pause, sortStatus& sort_status) {
    // If current rectangle is within interval and if its height is less than the pivot height, prepare swap
    if (current_rectangle >= first_sorted_rectangle && current_rectangle <= last_sorted_rectangle) {
        if (rectangle_heights[current_rectangle] <= rectangle_heights[pivot]) {
            // Pause, unless current rectangle and swap rectangle are the same
            if (swap_pause == false && cutoff + 1 != current_rectangle) {
                swap_pause = true;
                return;
            }

            swap_pause = false; // Turn the pause off
            swap_rectangles(rectangle_heights, current_rectangle, cutoff + 1);
            cutoff++;
        }
    }

    current_rectangle++;
    if (current_rectangle >= last_sorted_rectangle + 1) {
        // If at the end, pause, unless there is only one rectangle in the interval
        if (swap_pause == false && first_sorted_rectangle <= last_sorted_rectangle) {
            swap_pause = true;
            return;
        }

        swap_pause = false; // Turn pause off
        swap_rectangles(rectangle_heights, pivot, cutoff);
        insert_sort_intervals(sort_intervals, sorted_rectangles, cutoff, rectangle_heights.size());

        // Select next interval
        // If there are no more intervals, end sort
        if (sort_intervals.size() == 0) {
            sort_status = sorted;
            cout << "\nSort complete!";
            return;
        }

        first_sorted_rectangle = sort_intervals[0].start + 1;
        last_sorted_rectangle = sort_intervals[0].end;

        pivot = first_sorted_rectangle - 1;
        cutoff = pivot;
        current_rectangle = pivot;

        sort_intervals.erase(sort_intervals.begin()); // Remove spent interval
    }
}


// Insert sort intervals and sorted rectangle
void insert_sort_intervals(vector<interval>& sort_intervals, vector<int>& sorted_rectangles, int cutoff, int num_rectangles) {
    // Determine the minimum index in the sorted rectangles vector that is larger than the cutoff
    int index = 0;
    while (index < sorted_rectangles.size() && sorted_rectangles[index] < cutoff) {
        index++;
    }

    // Create and append right interval
    interval new_interval;
    new_interval = (index == sorted_rectangles.size()) ? interval{cutoff + 1, num_rectangles - 1} : interval{cutoff + 1, sorted_rectangles[index] - 1};

    if (new_interval.start <= new_interval.end) {
        sort_intervals.insert(sort_intervals.begin(), new_interval);
    }

    // Create and append left interval
    new_interval = (index == 0) ? interval{0, cutoff - 1} : interval{sorted_rectangles[index - 1] + 1, cutoff - 1};

    if (new_interval.start <= new_interval.end) {
        sort_intervals.insert(sort_intervals.begin(), new_interval);
    }

    // Insert new sorted rectangle into sorted rectangles
    sorted_rectangles.insert(sorted_rectangles.begin() + index, cutoff);
}


// Swap the positions of two rectangles
void swap_rectangles(vector<float>& rectangle_heights, int pos1, int pos2) {
    float temp = rectangle_heights[pos1];
    rectangle_heights[pos1] = rectangle_heights[pos2];
    rectangle_heights[pos2] = temp;
    return;
}
