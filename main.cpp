#include <iostream>
#include "structs.h"
#include "func.h"

using namespace std;

void demonstratePoint() {
    cout << "=== DEMONSTRATING POINT ===" << endl;
    Point p;
    cout << "Enter point coordinates (x y): ";
    readPoint(p);
    cout << "Point: ";
    printPoint(p);
    cout << endl << endl;
}

void demonstrateCircle() {
    cout << "=== DEMONSTRATING CIRCLE ===" << endl;
    Circle c;
    cout << "Enter circle (center_x center_y radius): ";
    readCircle(c);
    cout << "Circle: ";
    printCircle(c);
    cout << endl;
    cout << "Length: " << circleLength(c) << endl;
    cout << "Area: " << circleArea(c) << endl << endl;
}

void demonstrateSquare() {
    cout << "=== DEMONSTRATING SQUARE ===" << endl;
    Square s;
    cout << "Enter square (topLeft_x topLeft_y side): ";
    readSquare(s);
    cout << "Square: ";
    printSquare(s);
    cout << endl;
    cout << "Perimeter: " << squarePerimeter(s) << endl;
    cout << "Area: " << squareArea(s) << endl << endl;
}

void demonstratePointInFigures() {
    cout << "=== DEMONSTRATING POINT IN FIGURES ===" << endl;
    Point p;
    Circle c;
    Square s;
    
    cout << "Enter point to check: ";
    readPoint(p);
    cout << "Enter circle: ";
    readCircle(c);
    cout << "Enter square: ";
    readSquare(s);
    
    cout << "Point in circle: " << (isPointInCircle(p, c) ? "YES" : "NO") << endl;
    cout << "Point on circle: " << (isPointOnCircle(p, c) ? "YES" : "NO") << endl;
    cout << "Point in square: " << (isPointInSquare(p, s) ? "YES" : "NO") << endl;
    cout << "Point on square: " << (isPointOnSquare(p, s) ? "YES" : "NO") << endl << endl;
}

void demonstrateIntersections() {
    cout << "=== DEMONSTRATING INTERSECTIONS ===" << endl;
    Circle c1, c2;
    Square s1, s2;
    
    cout << "Enter first circle: ";
    readCircle(c1);
    cout << "Enter second circle: ";
    readCircle(c2);
    cout << "Enter first square: ";
    readSquare(s1);
    cout << "Enter second square: ";
    readSquare(s2);
    
    cout << "Circles intersect: " << (circlesIntersect(c1, c2) ? "YES" : "NO") << endl;
    cout << "Squares intersect: " << (squaresIntersect(s1, s2) ? "YES" : "NO") << endl;
    cout << "Circle and square intersect: " << (circleSquareIntersect(c1, s1) ? "YES" : "NO") << endl << endl;
}

void demonstrateContainment() {
    cout << "=== DEMONSTRATING CONTAINMENT ===" << endl;
    Circle c1, c2;
    Square s1, s2;
    
    cout << "Enter first circle: ";
    readCircle(c1);
    cout << "Enter second circle: ";
    readCircle(c2);
    cout << "Enter first square: ";
    readSquare(s1);
    cout << "Enter second square: ";
    readSquare(s2);
    
    cout << "Circle1 in Circle2: " << (isCircleInCircle(c1, c2) ? "YES" : "NO") << endl;
    cout << "Square1 in Square2: " << (isSquareInSquare(s1, s2) ? "YES" : "NO") << endl;
    cout << "Square in Circle: " << (isSquareInCircle(s1, c1) ? "YES" : "NO") << endl;
    cout << "Circle in Square: " << (isCircleInSquare(c1, s1) ? "YES" : "NO") << endl;
}

int main() {
    demonstratePoint();
    demonstrateCircle();
    demonstrateSquare();
    demonstratePointInFigures();
    demonstrateIntersections();
    demonstrateContainment();
    
    return 0;
}