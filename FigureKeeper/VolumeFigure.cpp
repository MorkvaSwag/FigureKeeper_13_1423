#include "VolumeFigure.h"
#include "PlaneFigure.h"
#include "Exceptions.h"
#include <iostream>
#include <cstring>
using namespace std;

static const double PI = 3.14159265358979;

double VolumeFigure::computeVolume(const char* t, const double* s, int count) {
    if (t == nullptr || s == nullptr)
        throw FigureException("Volume figure: no data");
    for (int i = 0; i < count; i++)
        if (s[i] <= 0)
            throw FigureException("Volume figure: all sizes must be positive");

    if (strcmp(t, "cube") == 0) {
        if (count != 1)
            throw FigureException("Cube needs exactly 1 size (edge)");
        return s[0] * s[0] * s[0];
    }
    if (strcmp(t, "box") == 0) {
        if (count != 3)
            throw FigureException("Box needs exactly 3 sizes (length, width, height)");
        return s[0] * s[1] * s[2];
    }
    if (strcmp(t, "cylinder") == 0) {
        if (count != 2)
            throw FigureException("Cylinder needs exactly 2 sizes (radius, height)");
        return PI * s[0] * s[0] * s[1];
    }
    throw FigureException("Unknown volume figure type (use: cube, box, cylinder)");
}

VolumeFigure::VolumeFigure() : Figure(), volume(0) {
    cout << "VolumeFigure: constructor without parameters\n";
}

VolumeFigure::VolumeFigure(const char* t, const double* s, int count)
    : Figure(t, s, count), volume(computeVolume(t, s, count)) {
    cout << "VolumeFigure: constructor with parameters\n";
}

VolumeFigure::VolumeFigure(const VolumeFigure& other)
    : Figure(other), volume(other.volume) {
    cout << "VolumeFigure: copy constructor\n";
}

VolumeFigure::~VolumeFigure() {
    cout << "VolumeFigure: destructor\n";
}

VolumeFigure& VolumeFigure::operator=(const VolumeFigure& other) {
    if (this == &other)
        return *this;
    Figure::operator=(other);
    volume = other.volume;
    cout << "VolumeFigure: assignment operator\n";
    return *this;
}

double VolumeFigure::getVolume() const {
    return volume;
}

void VolumeFigure::setFigure(const char* t, const double* s, int count) {
    double newVolume = computeVolume(t, s, count);
    setType(t);
    setSizes(s, count);
    volume = newVolume;
}

int VolumeFigure::getKind() const {
    return 2;
}

void VolumeFigure::print() const {
    cout << "Volume figure: " << getType() << ", sizes:";
    for (int i = 0; i < getSizesCount(); i++)
        cout << ' ' << getSizes()[i];
    cout << ", volume = " << volume << '\n';
}

void VolumeFigure::draw() const {
    double dims[2];
    if (strcmp(getType(), "cube") == 0) {
        dims[0] = getSizes()[0];
        dims[1] = getSizes()[0];
    }
    else if (strcmp(getType(), "box") == 0) {
        dims[0] = getSizes()[0];
        dims[1] = getSizes()[1];
    }
    else if (strcmp(getType(), "cylinder") == 0) {
        dims[0] = 2 * getSizes()[0];
        dims[1] = getSizes()[1];
    }
    else {
        cout << "(nothing to draw)\n";
        return;
    }
    cout << "Front view (drawn as a plane figure):\n";
    PlaneFigure view("rectangle", dims, 2);
    view.draw();
}

void VolumeFigure::save(ostream& out) const {
    saveBase(out);
}

void VolumeFigure::load(istream& in) {
    loadBase(in);
    volume = computeVolume(getType(), getSizes(), getSizesCount());
}