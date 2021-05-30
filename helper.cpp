#include "helper.hpp"


/********** Helper Functions **********/

// Get user input for the number of rectangles
unsigned int get_num_rectangles() {
    int num_rectangles;

    // Reprompt user if input is not positive integer
    do {
        cout << "Number of Rectangles: ";
        cin >> num_rectangles;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
        }
    } 
    while (num_rectangles <= 0);

    return num_rectangles;
}


// Get user input for the type of sort
sortType get_sort_type() {
    sortType sort_type = none;
    string user_input;

    // Reprompt user if invalid sort type
    do {
        cout << "Sort type (bubble, selection, insertion, merge): ";
        cin >> user_input;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
        }
        
        if (user_input == "bubble") {
            sort_type = sortType::bubble;
        } 
        else if (user_input == "selection") {
            sort_type = sortType::selection;
        } 
        else if (user_input == "insertion") {
            sort_type = sortType::insertion;
        } 
        else if (user_input == "merge") {
            sort_type = sortType::merge;
        }
    }
    while (sort_type == none);

    return sort_type;
}


// Initialize the sort variables
tuple<int, int, int, int, vector<interval>> initialize_sort(sortType sort_type, int num_rectangles) {
    int current_rectangle, first_sorted_rectangle, last_sorted_rectangle, min_rectangle;
    vector<interval> merge_intervals;

    switch (sort_type) {
        case sortType::bubble:
            last_sorted_rectangle = num_rectangles;
            current_rectangle = 0; // Index tracks left rectangle
            break;
        case sortType::selection:
            last_sorted_rectangle = -1;
            current_rectangle = 0;
            min_rectangle = 0;
            break;
        case sortType::insertion:
            last_sorted_rectangle = 0;
            current_rectangle = 1;
            break;
        case sortType::merge:
            first_sorted_rectangle = 0;
            last_sorted_rectangle = 0;
            current_rectangle = -1;
            initialize_merge_intervals(merge_intervals, 0, num_rectangles - 1);
            merge_intervals.erase(merge_intervals.begin()); // Remove first interval since we're already starting there
            break;
    }

    return {current_rectangle, first_sorted_rectangle, last_sorted_rectangle, min_rectangle, merge_intervals};
}


// Initialize merge intervals
void initialize_merge_intervals(vector<interval>& merge_intervals, int start, int end) {
    // Recursion base
    if (start == end) {
        interval current_interval = {start, end};
        merge_intervals.push_back(current_interval);
        return;
    }

    int midpoint = start + (int)floor(((float)end - start) / 2);
    // Left recursion
    initialize_merge_intervals(merge_intervals, start, midpoint);
    // Right recursion
    initialize_merge_intervals(merge_intervals, midpoint + 1, end);
    
    // Push interval to vector
    interval current_interval = {start, end};
    merge_intervals.push_back(current_interval);
}


// Set rectangle's color
sf::RectangleShape setColor(sf::RectangleShape rectangle, sortType sort_type, sortStatus sort_status, 
                            int i, int current_rectangle, int first_sorted_rectangle, int last_sorted_rectangle, int min_rectangle, int num_rectangles) {

    // If the set is already sorted, set all rectangles green and return
    if (sort_status == sorted) {
        rectangle.setFillColor(sf::Color::Green);
        return rectangle;
    }

    // Color depends on sort type
    switch (sort_type) {
        case sortType::bubble:
            if (i == current_rectangle || i == current_rectangle + 1) {
                rectangle.setFillColor(sf::Color::Cyan); // Highlight current rectangles in cyan
            }
            if (i >= last_sorted_rectangle) {
                rectangle.setFillColor(sf::Color::Yellow); // Highlight sorted rectangles in yellow
            }
            break;
        case sortType::selection:
            if (i == current_rectangle) {
                rectangle.setFillColor(sf::Color::Cyan); // Highlight current rectangle in cyan
            }
            if (i == min_rectangle) {
                rectangle.setFillColor(sf::Color::Red); // Highlight min rectangle in red
            }
            if (i == last_sorted_rectangle + 1 && current_rectangle == num_rectangles) {
                rectangle.setFillColor(sf::Color::Red); // Highlight rectangle that will be swapped with min in red
            }
            if (i <= last_sorted_rectangle) {
                rectangle.setFillColor(sf::Color::Yellow); // Highlight sorted rectangles in yellow
            }
            break;
        case sortType::insertion:
            if (i - 1 <= last_sorted_rectangle) {
                rectangle.setFillColor(sf::Color::Yellow); // Highlight sorted rectangles in yellow
            }
            if (i == current_rectangle || i == current_rectangle - 1) {
                rectangle.setFillColor(sf::Color::Cyan); // Highlight current rectangles in cyan
            }
            break;
        case sortType::merge:
            if (first_sorted_rectangle <= i && i <= last_sorted_rectangle) {
                rectangle.setFillColor(sf::Color::Cyan); // Highlight current interval in cyan
            }
            if (i == current_rectangle && first_sorted_rectangle <= i) {
                rectangle.setFillColor(sf::Color::Red); // Highlight current rectangle in red
            }
            break;
    }

    return rectangle;
}
