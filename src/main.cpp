#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>
#include <cmath>
#include <tuple>

#include <SFML/Graphics.hpp>

#include "sort.hpp"
#include "helper.hpp"

using namespace std;

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;



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
    vector<float> rectangle_heights;
    for (int i = 0; i < num_rectangles; i++) {
        float rectangle_height = rand() % SCREEN_HEIGHT;
        rectangle_heights.push_back(rectangle_height);
    }

    // Sorting status and speed
    sortStatus sort_status = unsorted;
    float sort_speed_factor = 150;
    float sort_speed = get_sort_speed(num_rectangles, sort_speed_factor);
    sf::Clock sort_speed_clock;

    // Type of sort
    sortType sort_type = get_sort_type();

    // Sort initializer
    auto [current_rectangle, first_sorted_rectangle, last_sorted_rectangle, min_rectangle, pivot, cutoff, swap_pause, intervals] = initialize_sort(sort_type, num_rectangles);
    vector<float> sorted_heights{rectangle_heights[0]}; // Initalize sorted heights vector for merge sort
    vector<int> sorted_rectangles; // Initalize sorted rectangles vector for quick sort
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
            rectangle = setColor(rectangle, sort_type, sort_status, swap_pause, sorted_rectangles,
                                i, current_rectangle, first_sorted_rectangle, last_sorted_rectangle, min_rectangle, pivot, cutoff, num_rectangles);

            window.draw(rectangle);
        }

        // If unsorted and space is pressed, start sort
        if (sort_status == unsorted && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            sort_status = sorting;
            clock.restart(); // Resets the clock
            sort_speed_clock.restart(); // Resets the sort speed clock
            cout << "\nUse the 'Q' and 'W' keys to speed up and slow down the sort.";
        }

        // Use q and w to slow down and speed up sort
        if (sort_speed_clock.getElapsedTime().asSeconds() > 0.15) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                sort_speed_factor = 1.25 * sort_speed_factor;
                sort_speed = get_sort_speed(num_rectangles, sort_speed_factor);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                sort_speed_factor = 0.75 * sort_speed_factor;
                sort_speed = get_sort_speed(num_rectangles, sort_speed_factor);
            }
            sort_speed_clock.restart();
        }

        // Sort
        if (sort_status == sorting) {
            if (clock.getElapsedTime().asSeconds() * sort_speed > 1) {
                switch (sort_type) {
                    case sortType::bubble:
                        bubble_sort(rectangle_heights, current_rectangle, last_sorted_rectangle, sort_status);
                        break;
                    case sortType::selection:
                        selection_sort(rectangle_heights, current_rectangle, last_sorted_rectangle, min_rectangle, num_rectangles, sort_status);
                        break;
                    case sortType::insertion:
                        insertion_sort(rectangle_heights, current_rectangle, last_sorted_rectangle, num_rectangles, sort_status);
                        break;
                    case sortType::merge:
                        merge_sort(rectangle_heights, current_rectangle, first_sorted_rectangle, last_sorted_rectangle, sorted_heights, intervals, sort_status);
                        break;
                    case sortType::quick:
                        quick_sort(rectangle_heights, sorted_rectangles, intervals, current_rectangle, first_sorted_rectangle, last_sorted_rectangle, pivot, cutoff, swap_pause, sort_status);
                        break;
                }

                // Restart timer
                clock.restart();
            }
        }

        window.display();
    }

    return EXIT_SUCCESS;
}
