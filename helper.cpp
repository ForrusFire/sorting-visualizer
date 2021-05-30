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
string get_sort_type() {
    string sort_type;

    // Reprompt user if invalid sort type
    do {
        cout << "Sort type (bubble, selection, insertion, merge): ";
        cin >> sort_type;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    while (sort_type != "bubble" && sort_type != "selection" && sort_type != "insertion" && sort_type != "merge");

    return sort_type;
}


// Initialize the sort variables
tuple<int, int, int, int, vector<interval>> initialize_sort(string sort_type, int num_rectangles) {
    int current_rectangle, first_sorted_rectangle, last_sorted_rectangle, min_rectangle;
    vector<interval> merge_intervals;

    if (sort_type == "bubble") {
        last_sorted_rectangle = num_rectangles;
        current_rectangle = 0; // Index tracks left rectangle
    } 
    else if (sort_type == "selection") {
        last_sorted_rectangle = -1;
        current_rectangle = 0;
        min_rectangle = 0;
    }
    else if (sort_type == "insertion") {
        last_sorted_rectangle = 0;
        current_rectangle = 1;
    }
    else if (sort_type == "merge") {
        first_sorted_rectangle = 0;
        last_sorted_rectangle = 0;
        current_rectangle = -1;
        initialize_merge_intervals(merge_intervals, 0, num_rectangles - 1);
        merge_intervals.erase(merge_intervals.begin());
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
    
    interval current_interval = {start, end};
    merge_intervals.push_back(current_interval);
}


// Set rectangle's color
sf::RectangleShape setColor(sf::RectangleShape rectangle, string sort_type, string sort_status, 
                            int i, int current_rectangle, int first_sorted_rectangle, int last_sorted_rectangle, int min_rectangle, int num_rectangles) {
    if (sort_type == "bubble") {
        if (i == current_rectangle || i == current_rectangle + 1) {
            rectangle.setFillColor(sf::Color::Cyan);
        }
        if (i >= last_sorted_rectangle) {
            rectangle.setFillColor(sf::Color::Yellow);
        }
    } 
    else if (sort_type == "selection") {
        if (i == current_rectangle) {
            rectangle.setFillColor(sf::Color::Cyan);
        }
        if (i == min_rectangle) {
            rectangle.setFillColor(sf::Color::Red);
        }
        if (i == last_sorted_rectangle + 1 && current_rectangle == num_rectangles) {
            rectangle.setFillColor(sf::Color::Red);
        }
        if (i <= last_sorted_rectangle) {
            rectangle.setFillColor(sf::Color::Yellow);
        }
    } 
    else if (sort_type == "insertion") {
        if (i - 1 <= last_sorted_rectangle) {
            rectangle.setFillColor(sf::Color::Yellow);
        }
        if (i == current_rectangle || i == current_rectangle - 1) {
            rectangle.setFillColor(sf::Color::Cyan);
        }
    }
    else if (sort_type == "merge") {
        if (first_sorted_rectangle <= i && i <= last_sorted_rectangle) {
            rectangle.setFillColor(sf::Color::Cyan);
        }
        if (i == current_rectangle && first_sorted_rectangle <= i) {
            rectangle.setFillColor(sf::Color::Red);
        }
    }

    // If the set is already sorted, override and set all rectangles green
    if (sort_status == "Sorted") {
        rectangle.setFillColor(sf::Color::Green);
    }

    return rectangle;
}
