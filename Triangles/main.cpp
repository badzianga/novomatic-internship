#include "triangle.h"

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include <map>

int main() {
    sf::RenderWindow window{ sf::VideoMode(800, 600), "Triangle collision detection" };
    window.setFramerateLimit(60);

    triangle t1{};
    t1.points = { vec2{ 250.f, 250.f }, vec2{ 350.f,250.f }, vec2{ 300.f, 160.f } };

    triangle t2{};
    t2.points = { vec2{ 500.f, 300.f }, vec2{ 300.f, 415.f }, vec2{ 150.f, 250.f } };

    std::map<std::string, bool> inputs;
    inputs["left"] = false;
    inputs["right"] = false;
    inputs["up"] = false;
    inputs["down"] = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    inputs["left"] = true;
                }
                if (event.key.code == sf::Keyboard::Right) {
                    inputs["right"] = true;
                }
                if (event.key.code == sf::Keyboard::Up) {
                    inputs["up"] = true;
                }
                if (event.key.code == sf::Keyboard::Down) {
                    inputs["down"] = true;
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left) {
                    inputs["left"] = false;
                }
                if (event.key.code == sf::Keyboard::Right) {
                    inputs["right"] = false;
                }
                if (event.key.code == sf::Keyboard::Up) {
                    inputs["up"] = false;
                }
                if (event.key.code == sf::Keyboard::Down) {
                    inputs["down"] = false;
                }
            }
        }

        window.clear(sf::Color::Black);

        if (inputs["left"]) {
            t1.points[0].x -= 2.5f;
            t1.points[1].x -= 2.5f;
            t1.points[2].x -= 2.5f;
        }
        if (inputs["right"]) {
            t1.points[0].x += 2.5f;
            t1.points[1].x += 2.5f;
            t1.points[2].x += 2.5f;
        }
        if (inputs["up"]) {
            t1.points[0].y -= 2.5f;
            t1.points[1].y -= 2.5f;
            t1.points[2].y -= 2.5f;
        }
        if (inputs["down"]) {
            t1.points[0].y += 2.5f;
            t1.points[1].y += 2.5f;
            t1.points[2].y += 2.5f;
        }


        // draw second triangle (static)
        sf::ConvexShape convex2;
        convex2.setPointCount(3);
        for (int i = 0; i < 3; i++) {
            convex2.setPoint(i, sf::Vector2f(t2.points[i].x, t2.points[i].y));
        }

        if (isColliding(t1, t2)) {
            convex2.setFillColor(sf::Color::Green);
        } else {
            convex2.setFillColor(sf::Color::Red);
        }
        window.draw(convex2);

        // draw first triangle (dynamic)
        sf::ConvexShape convex;
        convex.setPointCount(3);
        for (int i = 0; i < 3; i++) {
            convex.setPoint(i, sf::Vector2f(t1.points[i].x, t1.points[i].y));
        }
        window.draw(convex);

        window.display();
    }

    return 0;
}
