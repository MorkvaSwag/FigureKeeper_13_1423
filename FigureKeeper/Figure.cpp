#include "Figure.h"
#include "Exceptions.h"
#include <iostream>
#include <cstring>
using namespace std;

Figure::Figure() {
    type = new char[1];
    type[0] = '\0';
    sizes = nullptr;
    sizesCount = 0;
    cout << "Figure: constructor without parameters\n";
}

Figure::Figure(const char* t, const double* s, int count) {
    if (t == nullptr || t[0] == '\0')
        throw FigureException("Figure type must not be empty");
    if (s == nullptr || count <= 0)
        throw FigureException("Figure must have at least one size");

    type = new char[strlen(t) + 1];
    strcpy_s(type, strlen(t) + 1, t);
    sizesCount = count;
    sizes = new double[count];
    for (int i = 0; i < count; i++)
        sizes[i] = s[i];
    cout << "Figure: constructor with parameters\n";
}

Figure::Figure(const Figure& other) {
    type = new char[strlen(other.type) + 1];
    strcpy_s(type, strlen(other.type) + 1, other.type);
    sizesCount = other.sizesCount;
    sizes = (sizesCount > 0) ? new double[sizesCount] : nullptr;
    for (int i = 0; i < sizesCount; i++)
        sizes[i] = other.sizes[i];
    cout << "Figure: copy constructor\n";
}

Figure::~Figure() {
    delete[] type;
    delete[] sizes;
    cout << "Figure: destructor\n";
}

Figure& Figure::operator=(const Figure& other) {
    if (this == &other)
        return *this;
    delete[] type;
    delete[] sizes;
    type = new char[strlen(other.type) + 1];
    strcpy_s(type, strlen(other.type) + 1, other.type);
    sizesCount = other.sizesCount;
    sizes = (sizesCount > 0) ? new double[sizesCount] : nullptr;
    for (int i = 0; i < sizesCount; i++)
        sizes[i] = other.sizes[i];
    cout << "Figure: assignment operator\n";
    return *this;
}

const char* Figure::getType() const {
    return type;
}

void Figure::setType(const char* t) {
    if (t == nullptr || t[0] == '\0')
        throw FigureException("Figure type must not be empty");
    char* newType = new char[strlen(t) + 1];
    strcpy_s(newType, strlen(t) + 1, t);
    delete[] type;
    type = newType;
}

const double* Figure::getSizes() const {
    return sizes;
}

void Figure::setSizes(const double* s, int count) {
    if (s == nullptr || count <= 0)
        throw FigureException("Figure must have at least one size");
    double* newSizes = new double[count];
    for (int i = 0; i < count; i++)
        newSizes[i] = s[i];
    delete[] sizes;
    sizes = newSizes;
    sizesCount = count;
}

int Figure::getSizesCount() const {
    return sizesCount;
}

void Figure::saveBase(ostream& out) const {
    out << type << '\n' << sizesCount;
    for (int i = 0; i < sizesCount; i++)
        out << ' ' << sizes[i];
    out << '\n';
}

void Figure::loadBase(istream& in) {
    char* buf = new char[256];
    in.getline(buf, 256);
    if (buf[0] == '\0') {
        delete[] buf;
        throw FigureException("File error: figure type is missing");
    }
    setType(buf);
    delete[] buf;

    int count = 0;
    in >> count;
    if (!in || count <= 0 || count > 100)
        throw FigureException("File error: wrong number of sizes");
    double* tmp = new double[count];
    for (int i = 0; i < count; i++)
        in >> tmp[i];
    if (!in) {
        delete[] tmp;
        throw FigureException("File error: wrong size values");
    }
    in.ignore(10000, '\n');
    setSizes(tmp, count);
    delete[] tmp;
}