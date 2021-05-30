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
