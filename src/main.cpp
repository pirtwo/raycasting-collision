#include <SFML/Graphics.hpp>
#include "Collision.h"
#include <math.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(700, 500), "Ray Vs Box Collision");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("./assets/SourceSansPro-Regular.ttf"))
        return EXIT_FAILURE;

    sf::Vector2i mousePos;

    sf::RectangleShape box(sf::Vector2f(170, 120));
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color::Red);
    box.setOutlineThickness(3);
    box.setPosition(
        window.getSize().x / 2 - box.getSize().x / 2,
        window.getSize().y / 2 - box.getSize().y / 2);

    sf::Vertex line[2];
    line[0] = sf::Vertex(sf::Vector2f(100, 100), sf::Color::Red);
    line[1] = sf::Vertex(sf::Vector2f(200, 200), sf::Color::Red);

    sf::Vertex norm[2];
    norm[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Color::Yellow);
    norm[1] = sf::Vertex(sf::Vector2f(0, 0), sf::Color::Yellow);

    sf::RectangleShape marker(sf::Vector2f(10, 10));
    marker.setFillColor(sf::Color::Yellow);
    marker.setOrigin(5, 5);

    bool hasContact = false;
    sf::Vector2f ray(0, 0);
    sf::Vector2f contactPoint(0, 0);
    sf::Vector2f contactNormal(0, 0);
    float contactDistance = 0;

    char info[100];
    sf::Text infoText;
    infoText.setFont(font);
    infoText.setPosition(20, 20);
    infoText.setFillColor(sf::Color::Yellow);
    infoText.setCharacterSize(15);

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        //========= update =========//

        mousePos = sf::Mouse::getPosition(window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            line[0].position.x = (float)mousePos.x;
            line[0].position.y = (float)mousePos.y;
        }
        line[1].position.x = (float)mousePos.x;
        line[1].position.y = (float)mousePos.y;

        ray = line[1].position - line[0].position;

        hasContact = rayVsBox(
            line[0].position,
            ray,
            box.getPosition(),
            box.getSize(),
            contactPoint,
            contactNormal);

        if (hasContact)
            contactDistance = sqrt(
                pow(contactPoint.x - line[0].position.x, 2) +
                pow(contactPoint.y - line[0].position.y, 2));

        snprintf(
            info, 100,
            "P1 (%.2f, %.2f)\r\nP2 (%.2f, %.2f)\r\nCP(%.2f, %.2f)\r\nCD = %.2f",
            line[0].position.x,
            line[0].position.y,
            line[1].position.x,
            line[1].position.y,
            contactPoint.x,
            contactPoint.y,
            contactDistance);

        infoText.setString(info);

        //======== draw ===========//

        window.clear();
        window.draw(box);
        window.draw(line, 2, sf::PrimitiveType::LineStrip);
        if (hasContact)
        {
            norm[0].position = contactPoint;
            norm[1].position = contactPoint + contactNormal * 30.f;
            window.draw(norm, 2, sf::PrimitiveType::LineStrip);
            marker.setPosition(contactPoint);
            window.draw(marker);
        }
        window.draw(infoText);
        window.display();
    }

    return EXIT_SUCCESS;
}
