#ifndef STRUCTS_H
#define STRUCTS_H

// Структура Точка
struct Point {
    double x;
    double y;
};

// Функции для точки
void readPoint(Point& p);
void printPoint(const Point& p);

// Структура Круг
struct Circle {
    Point center;
    double radius;
};

// Функции для круга
void readCircle(Circle& c);
void printCircle(const Circle& c);
double circleLength(const Circle& c);
double circleArea(const Circle& c);

// Структура Квадрат
struct Square {
    Point topLeft;
    double side;
};

// Функции для квадрата
void readSquare(Square& s);
void printSquare(const Square& s);
double squarePerimeter(const Square& s);
double squareArea(const Square& s);

#endif