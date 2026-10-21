#include "PlaneFigure.h"
#include "Exceptions.h"
#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>
using namespace std;

static const double PI = 3.14159265358979;

double PlaneFigure::computeArea(const char* t, const double* s, int count) {
    if (t == nullptr || s == nullptr)
        throw FigureException("Plane figure: no data");
    for (int i = 0; i < count; i++)
        if (s[i] <= 0)
            throw FigureException("Plane figure: all sizes must be positive");

    if (strcmp(t, "circle") == 0) {
        if (count != 1)
            throw FigureException("Circle needs exactly 1 size (radius)");
        return PI * s[0] * s[0];
    }
    if (strcmp(t, "rectangle") == 0) {
        if (count != 2)
            throw FigureException("Rectangle needs exactly 2 sizes (width, height)");
        return s[0] * s[1];
    }
    if (strcmp(t, "triangle") == 0) {
        if (count != 3)
            throw FigureException("Triangle needs exactly 3 sizes (sides)");
        double a = s[0], b = s[1], c = s[2];
        if (a + b <= c || a + c <= b || b + c <= a)
            throw FigureException("Triangle with such sides does not exist");
        double p = (a + b + c) / 2;
        return sqrt(p * (p - a) * (p - b) * (p - c));
    }
    throw FigureException("Unknown plane figure type (use: circle, rectangle, triangle)");
}

PlaneFigure::PlaneFigure() : Figure(), area(0) {
    cout << "PlaneFigure: constructor without parameters\n";
}

PlaneFigure::PlaneFigure(const char* t, const double* s, int count)
    : Figure(t, s, count), area(computeArea(t, s, count)) {
    cout << "PlaneFigure: constructor with parameters\n";
}

PlaneFigure::PlaneFigure(const PlaneFigure& other)
    : Figure(other), area(other.area) {
    cout << "PlaneFigure: copy constructor\n";
}

PlaneFigure::~PlaneFigure() {
    cout << "PlaneFigure: destructor\n";
}

PlaneFigure& PlaneFigure::operator=(const PlaneFigure& other) {
    if (this == &other)
        return *this;
    Figure::operator=(other);
    area = other.area;
    cout << "PlaneFigure: assignment operator\n";
    return *this;
}

double PlaneFigure::getArea() const {
    return area;
}

void PlaneFigure::setFigure(const char* t, const double* s, int count) {
    double newArea = computeArea(t, s, count);
    setType(t);
    setSizes(s, count);
    area = newArea;
}

int PlaneFigure::getKind() const {
    return 1;
}

void PlaneFigure::print() const {
    cout << "Plane figure: " << getType() << ", sizes:";
    for (int i = 0; i < getSizesCount(); i++)
        cout << ' ' << getSizes()[i];
    cout << ", area = " << area << '\n';
}

// ---------- рисование (вспомогательные функции, видны только в этом файле) ----------

static void drawRect(double w, double h) {
    double m = max(w, h);
    double k = 12.0 / m;
    int rows = max(1, (int)lround(h * k));
    int cols = max(1, (int)lround(w * k)) * 2;   // умножаем на 2: символы выше, чем шире
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            cout << '*';
        cout << '\n';
    }
}

static void drawCircle() {
    const int R = 6;
    for (int y = -R; y <= R; y++) {
        for (int x = -2 * R; x <= 2 * R; x++) {
            double dx = x / 2.0;
            cout << ((dx * dx + y * y <= R * R) ? '*' : ' ');
        }
        cout << '\n';
    }
}

static void drawTriangle(double base, double height) {
    double k = 12.0 / max(base, height);
    int rows = max(2, (int)lround(height * k));
    int baseCols = max(2, (int)lround(base * k)) * 2;
    for (int i = 0; i < rows; i++) {
        int w = max(1, baseCols * (i + 1) / rows);
        int pad = (baseCols - w) / 2;
        for (int j = 0; j < pad; j++) cout << ' ';
        for (int j = 0; j < w; j++) cout << '*';
        cout << '\n';
    }
}

void PlaneFigure::draw() const {
    if (strcmp(getType(), "circle") == 0)
        drawCircle();
    else if (strcmp(getType(), "rectangle") == 0)
        drawRect(getSizes()[0], getSizes()[1]);
    else if (strcmp(getType(), "triangle") == 0)
        drawTriangle(getSizes()[0], 2 * area / getSizes()[0]);
    else
        cout << "(nothing to draw)\n";
}

void PlaneFigure::save(ostream& out) const {
    saveBase(out);
}

void PlaneFigure::load(istream& in) {
    loadBase(in);
    area = computeArea(getType(), getSizes(), getSizesCount());
}
