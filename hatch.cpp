#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

#define PI 3.14159265

class Hatch
{
    struct Point_2
    {
        double x;
        double y;
    };
    struct Line_2
    {
        double x1;
        double y1;
        double x2;
        double y2;
    };
    int angle = 0;
    int step = 0;
    double k = 0;
    int maxx = -9999;
    int minx = 9999;
    int maxy = -9999;
    int miny = 9999;
    int point = 0;
    std::vector<std::vector<Point_2>> contoursPoints;
    std::vector<Line_2> PointsOfLines;
    int countOfLines = 0;

public:
    Hatch(int angle, int step)
    {
        // y = Kx + b
        k = tan(angle * PI / 180);
        contoursPoints = {
            {{0, 0}, {10, 0}, {10, 10}, {0, 10}}};
        // Y = kX + b
        for (int figures = 0; figures < contoursPoints.size(); figures++)
        {
            for (int points = 0; points < contoursPoints[figures].size(); points++)
            {
                std::cout << "x is:" << contoursPoints[figures][points].x << ","
                          << "y is:" << contoursPoints[figures][points].y << std::endl;
                if (contoursPoints[figures][points].x > maxx)
                {
                    maxx = contoursPoints[figures][points].x;
                }
                if (contoursPoints[figures][points].x < minx)
                {
                    minx = contoursPoints[figures][points].x;
                }
                if (contoursPoints[figures][points].y > maxy)
                {
                    maxy = contoursPoints[figures][points].y;
                }
                if (contoursPoints[figures][points].y < miny)
                {
                    miny = contoursPoints[figures][points].y;
                }
            }
        }
        // y = kx + B
        countOfLines = ((maxy - miny) / step);
        std::cout << "angle is " << angle << "\nCoef is "
                  << k << std::endl;
        std::cout << "maxx:" << maxx << " minx:" << minx << " maxy:" << maxy << " miny:" << miny;
        std::cout << std::endl
                  << countOfLines;
        int offset = 0;
        for (int n = 0; n < countOfLines; n++)
        {
            Line_2 coordiatesOfLine = {};
            coordiatesOfLine.x1 = minx;
            coordiatesOfLine.x2 = maxx;
            coordiatesOfLine.y1 = miny + offset;
            coordiatesOfLine.y2 = miny + offset;
            PointsOfLines.push_back(coordiatesOfLine);
            coordiatesOfLine = {};
            offset += step;
            std::cout << std::endl
                      << "x1y1:" << PointsOfLines[n].x1 << " " << PointsOfLines[n].y1;
            std::cout << std::endl
                      << "x2y2:" << PointsOfLines[n].x2 << " " << PointsOfLines[n].y2;
        }
    }
    void draw_quad(sf::RenderWindow &window)
    {

        sf::VertexArray quads(sf::Quads, 4);
        for (int figures = 0; figures < contoursPoints.size(); figures++)
        {
            point = 0;
            for (int points = 0; points < contoursPoints[figures].size(); points++)
            {
                quads[point].position = sf::Vector2f(200 + contoursPoints[figures][points].x, 150 + contoursPoints[figures][points].y);
                quads[point].color = sf::Color::White;
                point++;
            }
        }
        sf::Vertex vertices[2] =
            {
                sf::Vertex(sf::Vector2f(240 + 1, 151 + 5), sf::Color::Red),
                sf::Vertex(sf::Vector2f(250 + 5, 178 + 10), sf::Color::Red),
            };

        window.draw(quads);
        window.draw(vertices, 2, sf::Lines);
    }
    void draw()
    {
        // create the window
        sf::RenderWindow window(sf::VideoMode(400, 300), "My window");

        // run the program as long as the window is open
        while (window.isOpen())
        {
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (window.pollEvent(event))
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            // clear the window with black color
            window.clear(sf::Color::Cyan);

            // draw everything here...
            // window.draw(...);
            draw_quad(window);

            // end the current frame
            window.display();
        }
    }
};
int main()
{
    int angle, step;
    // std::cin >> angle >> step;
    angle = 78;
    step = 1;
    Hatch hatch(angle, step);
    hatch.draw();
}