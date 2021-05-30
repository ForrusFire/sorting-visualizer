#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>
#include <cmath>
#include <tuple>

#include <SFML/Graphics.hpp>

using namespace std;

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;

struct interval {
    int first;
    int last;
};

/* Prototypes */
unsigned int get_num_rectangles(void);
string get_sort_type(void);
tuple<int, int, int, int, vector<interval>> initialize_sort(string sort_type, int num_rectangles);
void initialize_merge_intervals(vector<interval>& merge_intervals, int start, int end);
sf::RectangleShape setColor(sf::RectangleShape rectangle, string sort_type, string sort_status, 
                            int i, int current_rectangle, int first_sorted_rectangle, int last_sorted_rectangle, int min_rectangle, int num_rectangles);

void bubble_sort(float rectangle_heights[], int& current_rectangle, int& last_sorted_rectangle, string& sort_status);
void selection_sort(float rectangle_heights[], int& current_rectangle, int& last_sorted_rectangle, int& min_rectangle, int num_rectangles, string& sort_status);
void insertion_sort(float rectangle_heights[], int& current_rectangle, int& last_sorted_rectangle, int num_rectangles, string& sort_status);
void merge_sort(float rectangle_heights[], int& current_rectangle, int& first_sorted_rectangle, int& last_sorted_rectangle,   
                vector<float>& sorted_heights, vector<interval>& merge_intervals, string& sort_status);
void swap_rectangles(float rectangle_heights[], int pos1, int pos2);


int main()
{
    // Initialize random seed
    srand(time(NULL));
    
    // Create window
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Sorting Algorithm Visualizer");
    // Create clock
    sf::Clock clock;

    // Create rectangle width and initial heights
    unsigned int num_rectangles = get_num_rectangles();

    float rectangle_width = SCREEN_WIDTH / (float)num_rectangles;
    float rectangle_heights[num_rectangles];
    for (int i = 0; i < num_rectangles; i++) {
        float rectangle_height = rand() % SCREEN_HEIGHT;
        rectangle_heights[i] = rectangle_height;
    }

    // Sorting status
    string sort_status = "Unsorted";
    float sort_speed = (pow(num_rectangles, 2) - num_rectangles) / 150;

    // Type of sort
    string sort_type = get_sort_type();

    // Sort initializer
    auto [current_rectangle, first_sorted_rectangle, last_sorted_rectangle, min_rectangle, merge_intervals] = initialize_sort(sort_type, num_rectangles);
    vector<float> sorted_heights{rectangle_heights[0]};
    cout << "Press space to start sorting!";


    // Main loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window if x'ed out
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // Draw in the rectangles
        for (int i = 0; i < num_rectangles; i++) {
            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(rectangle_width, rectangle_heights[i]));
            rectangle.setPosition(sf::Vector2f(i * rectangle_width, SCREEN_HEIGHT - rectangle_heights[i]));
            rectangle = setColor(rectangle, sort_type, sort_status, i, current_rectangle, first_sorted_rectangle, last_sorted_rectangle, min_rectangle, num_rectangles);

            window.draw(rectangle);
        }

        // If unsorted and space is pressed, start sort
        if (sort_status == "Unsorted" && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            sort_status = "Sorting";
            clock.restart(); // Resets the clock
        }

        // Sort
        if (sort_status == "Sorting") {
            if (clock.getElapsedTime().asSeconds() * sort_speed > 1) {
                if (sort_type == "bubble") {
                    bubble_sort(rectangle_heights, current_rectangle, last_sorted_rectangle, sort_status);
                } 
                else if (sort_type == "selection") {
                    selection_sort(rectangle_heights, current_rectangle, last_sorted_rectangle, min_rectangle, num_rectangles, sort_status);
                } 
                else if (sort_type == "insertion") {
                    insertion_sort(rectangle_heights, current_rectangle, last_sorted_rectangle, num_rectangles, sort_status);
                } else if (sort_type == "merge") {
                    merge_sort(rectangle_heights, current_rectangle, first_sorted_rectangle, last_sorted_rectangle, sorted_heights, merge_intervals, sort_status);
                }

                // Restart timer
                clock.restart();
            }
        }

        window.display();
    }

    return EXIT_SUCCESS;
}



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