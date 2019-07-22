
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"

const double left = -0.05;
const double right = 0.05;

const double bottom = -0.05;
const double top = 0.05;

const double cx = -0.7269;
const double cy = 0.1889;

// −0.7269 + 0.1889

double normalize(int value, int maxValue, double leftLimit, double rightLimit) {
    return leftLimit + (rightLimit - leftLimit) * value / maxValue;
}

sf::Color getColor(int x, int y, int maxX, int maxY) {
    
    double zx = normalize(x, maxX, left, right); // scaled x coordinate of pixel # (scaled to lie in the Mandelbrot X scale (−2.5, 1))
    double zy = normalize(y, maxY, bottom, top); // scaled y coordinate of pixel # (scaled to lie in the Mandelbrot Y scale (−1, 1))
    
    int iteration = 0;
    int max_iteration = 255;
        
        while ((zx * zx + zy * zy < 4) && (iteration < max_iteration))
        {
            double xtemp = zx * zx - zy * zy;
            zy = 2 * zx * zy  + cy;
            zx = xtemp + cx;
            
            iteration++;
        }
        
    if (iteration == max_iteration) {
        return sf::Color::White;
    }
    else {
        sf::Color color;
        color.r = iteration;
        return color;
    }
}

int render(sf::RenderWindow &window, sf::Sprite sprite, sf::Text text) {
    // Clear screen
    window.clear();
    
    sf::Vector2u size = window.getSize();
    int maxX = size.x;
    int maxY = size.y;
    std::cout << "Screen size " << size.x << " " << size.y << std::endl;
    
    sf::Image image;
    sf::Color color;
    
    image.create(size.x, size.y, sf::Color::White);
    
    for (int x = 0; x < maxX; x++) {
        for (int y = 0; y < maxY; y++) {
            image.setPixel(x, y, getColor(x, y, maxX, maxY));
        }
    }
    
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite s(texture);
    
    // Draw the sprite
    // window.draw(sprite);
    window.draw(s);
    // sprite.getTexture();
    
    // Draw the string
    window.draw(text);
    
    // Update the window
    window.display();
    
    return EXIT_SUCCESS;
}

int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML window");

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile(resourcePath() + "cute_image.jpg")) {
        return EXIT_FAILURE;
    }
    
    
    sf::Sprite sprite(texture);

    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }
    sf::Text text("Hello SFML", font, 50);
    text.setFillColor(sf::Color::Black);

    // Load a music to play
    sf::Music music;
    if (!music.openFromFile(resourcePath() + "nice_music.ogg")) {
        return EXIT_FAILURE;
    }

    // Play the music
    music.play();

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                render(window, sprite, text);
            }
        }

        
    }

    return EXIT_SUCCESS;
}

