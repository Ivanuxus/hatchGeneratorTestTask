#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <algorithm>

#define PI 3.14159265

/**
 * @class Hatch
 * @brief Класс для генерации и отображения штриховки под заданным углом
 * 
 * Класс создает систему параллельных линий для заливки прямоугольного контура.
 * Поддерживает различные углы наклона линий и расстояния между ними.
 */
class Hatch
{
    /**
     * @struct Point_2
     * @brief Точка в двумерном пространстве
     */
    struct Point_2
    {
        double x; ///< Координата X
        double y; ///< Координата Y
        
        /**
         * @brief Конструктор точки
         * @param x Координата X (по умолчанию 0)
         * @param y Координата Y (по умолчанию 0)
         */
        Point_2(double x = 0, double y = 0) : x(x), y(y) {}
    };
    
    /**
     * @struct Line_2
     * @brief Отрезок прямой между двумя точками
     */
    struct Line_2
    {
        Point_2 p1; ///< Первая точка отрезка
        Point_2 p2; ///< Вторая точка отрезка
    };

    int angle = 0;          ///< Угол штриховки в градусах
    double step = 0;        ///< Расстояние между линиями штриховки
    double k = 0;           ///< Угловой коэффициент (тангенс угла)
    double maxx = -9999;    ///< Максимальная координата X контура
    double minx = 9999;     ///< Минимальная координата X контура
    double maxy = -9999;    ///< Максимальная координата Y контура
    double miny = 9999;     ///< Минимальная координата Y контура
    
    std::vector<std::vector<Point_2>> contoursPoints; ///< Массив контуров для штриховки
    std::vector<Line_2> hatchLines;                   ///< Массив линий штриховки

public:
    /**
     * @brief Конструктор класса Hatch
     * @param angle Угол штриховки в градусах
     * @param step Расстояние между линиями штриховки
     * 
     * Инициализирует параметры штриховки, вычисляет границы контура
     * и генерирует линии штриховки.
     */
    Hatch(int angle, double step) : angle(angle), step(step)
    {
        // Вычисляем угловой коэффициент
        k = tan(angle * PI / 180);
        
        // Задаем тестовый прямоугольный контур 10x10
        contoursPoints = {{{0, 0}, {10, 0}, {10, 10}, {0, 10}}};
        
        // Находим границы контура (bounding box)
        for (const auto& figure : contoursPoints)
        {
            for (const auto& point : figure)
            {
                maxx = std::max(maxx, point.x);
                minx = std::min(minx, point.x);
                maxy = std::max(maxy, point.y);
                miny = std::min(miny, point.y);
            }
        }
        
        // Генерируем линии штриховки
        generateHatchLines();
        
        // Выводим информацию о сгенерированных линиях
        printLines();
    }

private:
    /**
     * @brief Генерирует линии штриховки для прямоугольного контура
     * 
     * Создает систему параллельных линий под заданным углом, которые
     * пересекают ограничивающий прямоугольник контура. Для углов близких
     * к 90 градусам используется специальная обработка.
     */
    void generateHatchLines()
    {
        // Для углов близких к 90 градусам используем вертикальные линии
        if (std::abs(angle - 90) < 1e-5)
        {
            generateVerticalLines();
            return;
        }
        
        // Вычисляем нормальное расстояние между линиями с учетом угла наклона
        double normalStep = step / cos(angle * PI / 180);
        
        // Определяем диапазон для линий штриховки на основе диагонали прямоугольника
        double diagonal = sqrt((maxx - minx) * (maxx - minx) + (maxy - miny) * (maxy - miny));
        double startOffset = -diagonal;  ///< Начальное смещение для обеспечения полного покрытия
        double endOffset = diagonal;     ///< Конечное смещение для обеспечения полного покрытия
        
        // Генерируем линии штриховки
        for (double b = startOffset; b <= endOffset; b += normalStep)
        {
            // Находим точки пересечения линии y = kx + b с границами прямоугольника
            std::vector<Point_2> intersections;
            
            // Проверяем пересечения с вертикальными границами (x = minx, x = maxx)
            double y1 = k * minx + b;
            if (y1 >= miny && y1 <= maxy)
                intersections.emplace_back(minx, y1);
                
            double y2 = k * maxx + b;
            if (y2 >= miny && y2 <= maxy)
                intersections.emplace_back(maxx, y2);
            
            // Проверяем пересечения с горизонтальными границами (y = miny, y = maxy)
            double x1 = (miny - b) / k;
            if (x1 >= minx && x1 <= maxx)
                intersections.emplace_back(x1, miny);
                
            double x2 = (maxy - b) / k;
            if (x2 >= minx && x2 <= maxx)
                intersections.emplace_back(x2, maxy);
            
            // Если нашли 2 точки пересечения, добавляем линию (отрезок внутри прямоугольника)
            if (intersections.size() == 2)
            {
                hatchLines.push_back({intersections[0], intersections[1]});
            }
        }
    }
    
    /**
     * @brief Генерирует вертикальные линии штриховки
     * 
     * Специальный метод для случая, когда угол штриховки равен 90 градусам.
     * Создает вертикальные линии с постоянным шагом.
     */
    void generateVerticalLines()
    {
        // Вертикальные линии (x = constant)
        for (double x = minx; x <= maxx; x += step)
        {
            hatchLines.push_back({Point_2(x, miny), Point_2(x, maxy)});
        }
    }

public:
    /**
     * @brief Выводит информацию о сгенерированных линиях штриховки в консоль
     */
    void printLines()
    {
        std::cout << "Hatch lines (angle: " << angle << "°, step: " << step << "):\n";
        for (size_t i = 0; i < hatchLines.size(); ++i)
        {
            std::cout << "Line " << i + 1 << ": (" 
                      << hatchLines[i].p1.x << "," << hatchLines[i].p1.y << ") -> ("
                      << hatchLines[i].p2.x << "," << hatchLines[i].p2.y << ")\n";
        }
    }

    /**
     * @brief Отрисовывает контур и линии штриховки в графическом окне
     * @param window Ссылка на объект окна SFML для отрисовки
     * 
     * Метод рисует прямоугольный контур белым цветом и линии штриховки красным цветом.
     * Координаты масштабируются для лучшего отображения в окне.
     */
    void draw_quad(sf::RenderWindow &window)
    {
        // Рисуем контур
        sf::VertexArray contour(sf::PrimitiveType::LineStrip, 5);
        
        for (int i = 0; i < 4; ++i)
        {
            contour[i].position = sf::Vector2f(50.f + static_cast<float>(contoursPoints[0][i].x * 10.f), 
                                              50.f + static_cast<float>(contoursPoints[0][i].y * 10.f));
            contour[i].color = sf::Color::White;
        }
        // Замыкаем контур
        contour[4] = contour[0];
        
        window.draw(contour);
        
        // Рисуем линии штриховки
        for (const auto& line : hatchLines)
        {
            sf::Vertex hatchLine[] = {
                sf::Vertex(sf::Vector2f(50.f + static_cast<float>(line.p1.x * 10.f), 
                                       50.f + static_cast<float>(line.p1.y * 10.f)), sf::Color::Red),
                sf::Vertex(sf::Vector2f(50.f + static_cast<float>(line.p2.x * 10.f), 
                                       50.f + static_cast<float>(line.p2.y * 10.f)), sf::Color::Red)
            };
            window.draw(hatchLine, 2, sf::PrimitiveType::Lines);
        }
    }
    
    /**
     * @brief Создает и управляет графическим окном для отображения штриховки
     * 
     * Создает окно SFML, обрабатывает события и отрисовывает штриховку
     * до тех пор, пока окно не будет закрыто пользователем.
     */
    void draw()
    {
        sf::RenderWindow window(sf::VideoMode({400u, 300u}), "Hatch Generator");
        
        while (window.isOpen())
        {
            // Обработка событий в SFML 3.0
            for (auto event = window.pollEvent(); event.has_value(); event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();
            }
            
            window.clear(sf::Color::Black);
            draw_quad(window);
            window.display();
        }
    }
};

/**
 * @brief Главная функция программы
 * @param argc Количество аргументов командной строки
 * @param argv Массив аргументов командной строки
 * @return Код завершения программы
 * 
 * Функция парсит аргументы командной строки для задания угла и шага штриховки,
 * создает объект Hatch и запускает графическое отображение.
 */
int main(int argc, char* argv[])
{
    int angle = 45;     ///< Угол штриховки по умолчанию
    double step = 1.0;  ///< Шаг штриховки по умолчанию
    
    // Простой парсинг аргументов командной строки
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--angle" && i + 1 < argc)
        {
            angle = std::stoi(argv[++i]);
        }
        else if (arg == "--step" && i + 1 < argc)
        {
            step = std::stod(argv[++i]);
        }
    }
    
    std::cout << "Generating hatch with angle: " << angle << "°, step: " << step << std::endl;
    
    // Создаем объект штриховки и запускаем отображение
    Hatch hatch(angle, step);
    hatch.draw();
    
    return 0;
}