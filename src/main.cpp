#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <math.h>

sf::Vector2f calcNormal(sf::Vector2f &v);
bool rayVsBox(
    sf::Vector2f rayOri,
    sf::Vector2f rayDir,
    sf::Vector2f boxPos,
    sf::Vector2f boxSize,
    sf::Vector2f &contactPoint,
    sf::Vector2f &contactNormal);

int main()
{
    sf::RenderWindow window(sf::VideoMode(700, 500), "Ray Casting Collision Detecttion");
    window.setFramerateLimit(60);

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
    sf::Vector2f ray;
    sf::Vector2f contactPoint;
    sf::Vector2f contactNormal;

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

        window.display();
    }

    return EXIT_SUCCESS;
}

bool rayVsBox(
    sf::Vector2f rayOri,
    sf::Vector2f rayDir,
    sf::Vector2f boxPos,
    sf::Vector2f boxSize,
    sf::Vector2f &contactPoint,
    sf::Vector2f &contactNormal)
{
    float boxMinX = boxPos.x;
    float boxMinY = boxPos.y;
    float boxMaxX = boxPos.x + boxSize.x;
    float boxMaxY = boxPos.y + boxSize.y;

    float t0x = (boxMinX - rayOri.x) / rayDir.x;
    float t1x = (boxMaxX - rayOri.x) / rayDir.x;
    float t0y = (boxMinY - rayOri.y) / rayDir.y;
    float t1y = (boxMaxY - rayOri.y) / rayDir.y;

    float txMin = std::min(t0x, t1x);
    float txMax = std::max(t0x, t1x);
    float tyMin = std::min(t0y, t1y);
    float tyMax = std::max(t0y, t1y);

    float tMin = std::max(txMin, tyMin);
    float tMax = std::min(txMax, tyMax);

    if (tMax < tMin)
        return false;

    contactPoint =
        tMin >= 0
            ? rayOri + rayDir * tMin
            : rayOri + rayDir * tMax;

    if (contactPoint.x == boxMinX)
    {
        contactNormal.x = -1;
        contactNormal.y = 0;
    }
    if (contactPoint.x == boxMaxX)
    {
        contactNormal.x = 1;
        contactNormal.y = 0;
    }
    if (contactPoint.y == boxMinY)
    {
        contactNormal.x = 0;
        contactNormal.y = -1;
    }
    if (contactPoint.y == boxMaxY)
    {
        contactNormal.x = 0;
        contactNormal.y = 1;
    }

    return true;
}
