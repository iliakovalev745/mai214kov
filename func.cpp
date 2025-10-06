#include "structs.h"
#include "func.h"
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

const double EPS = 1e-5;

// Функции для точки
void readPoint(Point& p) {
    cin >> p.x >> p.y;
}

void printPoint(const Point& p) {
    cout << "(" << p.x << ", " << p.y << ")";
}

// Функции для круга
void readCircle(Circle& c) {
    readPoint(c.center);
    cin >> c.radius;
}

void printCircle(const Circle& c) {
    cout << "Circle: center ";
    printPoint(c.center);
    cout << ", radius " << c.radius;
}

double circleLength(const Circle& c) {
    return 2 * 3.14 * c.radius;
}

double circleArea(const Circle& c) {
    return 3.14 * c.radius * c.radius;
}

// Функции для квадрата
void readSquare(Square& s) {
    readPoint(s.topLeft);
    cin >> s.side;
}

void printSquare(const Square& s) {
    cout << "Square: top-left ";
    printPoint(s.topLeft);
    cout << ", side " << s.side;
}

double squarePerimeter(const Square& s) {
    return 4 * s.side;
}

double squareArea(const Square& s) {
    return s.side * s.side;
}

// Вспомогательная функция для сравнения double с учетом погрешности
bool equal(double a, double b) {
    return fabs(a - b) < EPS;
}

bool lessOrEqual(double a, double b) {
    return a < b || equal(a, b);
}

// Принадлежность точки кругу (строго внутри)
bool isPointInCircle(const Point& p, const Circle& c) {
    double dx = p.x - c.center.x;
    double dy = p.y - c.center.y;
    double distance = sqrt(dx*dx + dy*dy);
    return distance < c.radius && !equal(distance, c.radius);
}

// Принадлежность точки квадрату (строго внутри)
bool isPointInSquare(const Point& p, const Square& s) {
    double right = s.topLeft.x + s.side;
    double bottom = s.topLeft.y - s.side;
    
    return (p.x > s.topLeft.x && p.x < right &&
            p.y < s.topLeft.y && p.y > bottom);
}

// Точка на круге
bool isPointOnCircle(const Point& p, const Circle& c) {
    double dx = p.x - c.center.x;
    double dy = p.y - c.center.y;
    double distance = sqrt(dx*dx + dy*dy);
    return equal(distance, c.radius);
}

// Точка на квадрате
bool isPointOnSquare(const Point& p, const Square& s) {
    double right = s.topLeft.x + s.side;
    double bottom = s.topLeft.y - s.side;
    
    // Проверка на границах с учетом погрешности
    bool onVerticalLeft = equal(p.x, s.topLeft.x) && lessOrEqual(p.y, s.topLeft.y) && lessOrEqual(bottom, p.y);
    bool onVerticalRight = equal(p.x, right) && lessOrEqual(p.y, s.topLeft.y) && lessOrEqual(bottom, p.y);
    bool onHorizontalTop = equal(p.y, s.topLeft.y) && lessOrEqual(p.x, s.topLeft.x) && lessOrEqual(s.topLeft.x, right);
    bool onHorizontalBottom = equal(p.y, bottom) && lessOrEqual(p.x, s.topLeft.x) && lessOrEqual(s.topLeft.x, right);
    
    return onVerticalLeft || onVerticalRight || onHorizontalTop || onHorizontalBottom;
}

// Пересечение двух кругов
bool circlesIntersect(const Circle& c1, const Circle& c2) {
    double dx = c1.center.x - c2.center.x;
    double dy = c1.center.y - c2.center.y;
    double distance = sqrt(dx*dx + dy*dy);
    
    return lessOrEqual(distance, c1.radius + c2.radius) && 
           lessOrEqual(fabs(c1.radius - c2.radius), distance);
}

// Пересечение двух квадратов
bool squaresIntersect(const Square& s1, const Square& s2) {
    double right1 = s1.topLeft.x + s1.side;
    double bottom1 = s1.topLeft.y - s1.side;
    double right2 = s2.topLeft.x + s2.side;
    double bottom2 = s2.topLeft.y - s2.side;
    
    return !(right1 <= s2.topLeft.x || s1.topLeft.x >= right2 ||
             bottom1 >= s2.topLeft.y || s1.topLeft.y <= bottom2);
}

// Пересечение круга и квадрата
bool circleSquareIntersect(const Circle& c, const Square& s) {
    // Проверяем пересечение с каждой стороной квадрата
    double right = s.topLeft.x + s.side;
    double bottom = s.topLeft.y - s.side;
    
    // Углы квадрата
    Point corners[4] = {
        {s.topLeft.x, s.topLeft.y},
        {right, s.topLeft.y},
        {right, bottom},
        {s.topLeft.x, bottom}
    };
    
    // Проверяем, лежит ли любая точка квадрата в круге или наоборот
    for (int i = 0; i < 4; i++) {
        double dx = corners[i].x - c.center.x;
        double dy = corners[i].y - c.center.y;
        if (sqrt(dx*dx + dy*dy) <= c.radius) return true;
    }
    
    // Проверяем пересечение с сторонами квадрата
    // Для простоты проверяем ближайшую точку на квадрате к центру круга
    double closestX = max(s.topLeft.x, min(c.center.x, right));
    double closestY = min(s.topLeft.y, max(c.center.y, bottom));
    
    double dx = closestX - c.center.x;
    double dy = closestY - c.center.y;
    
    return (dx*dx + dy*dy) <= (c.radius * c.radius);
}

// Круг внутри круга
bool isCircleInCircle(const Circle& c1, const Circle& c2) {
    double dx = c1.center.x - c2.center.x;
    double dy = c1.center.y - c2.center.y;
    double distance = sqrt(dx*dx + dy*dy);
    
    return lessOrEqual(distance + c1.radius, c2.radius);
}

// Квадрат внутри квадрата
bool isSquareInSquare(const Square& s1, const Square& s2) {
    double right1 = s1.topLeft.x + s1.side;
    double bottom1 = s1.topLeft.y - s1.side;
    double right2 = s2.topLeft.x + s2.side;
    double bottom2 = s2.topLeft.y - s2.side;
    
    return (s1.topLeft.x >= s2.topLeft.x && right1 <= right2 &&
            s1.topLeft.y <= s2.topLeft.y && bottom1 >= bottom2);
}

// Квадрат внутри круга
bool isSquareInCircle(const Square& s, const Circle& c) {
    // Проверяем, что все углы квадрата внутри круга
    Point corners[4] = {
        {s.topLeft.x, s.topLeft.y},
        {s.topLeft.x + s.side, s.topLeft.y},
        {s.topLeft.x + s.side, s.topLeft.y - s.side},
        {s.topLeft.x, s.topLeft.y - s.side}
    };
    
    for (int i = 0; i < 4; i++) {
        double dx = corners[i].x - c.center.x;
        double dy = corners[i].y - c.center.y;
        if (sqrt(dx*dx + dy*dy) > c.radius) return false;
    }
    return true;
}

// Круг внутри квадрата
bool isCircleInSquare(const Circle& c, const Square& s) {
    double right = s.topLeft.x + s.side;
    double bottom = s.topLeft.y - s.side;
    
    return (c.center.x - c.radius >= s.topLeft.x &&
            c.center.x + c.radius <= right &&
            c.center.y - c.radius >= bottom &&
            c.center.y + c.radius <= s.topLeft.y);
}