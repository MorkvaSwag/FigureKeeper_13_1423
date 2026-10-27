#include "Keeper.h"
#include "PlaneFigure.h"
#include "VolumeFigure.h"
#include "Exceptions.h"
#include <iostream>
#include <fstream>
using namespace std;

Keeper::Keeper() : items(nullptr), count(0), capacity(0) {
    cout << "Keeper: constructor without parameters\n";
}

Keeper::Keeper(int startCapacity) : items(nullptr), count(0), capacity(0) {
    if (startCapacity < 0)
        throw FigureException("Keeper: capacity must not be negative");
    if (startCapacity > 0) {
        items = new Figure * [startCapacity];
        capacity = startCapacity;
    }
    cout << "Keeper: constructor with parameters\n";
}

Figure* Keeper::cloneFigure(const Figure* f) {
    if (f->getKind() == 1)
        return new PlaneFigure(f->getType(), f->getSizes(), f->getSizesCount());
    if (f->getKind() == 2)
        return new VolumeFigure(f->getType(), f->getSizes(), f->getSizesCount());
    throw FigureException("Keeper: unknown kind of figure");
}

Keeper::Keeper(const Keeper& other) : items(nullptr), count(0), capacity(0) {
    if (other.count > 0) {
        items = new Figure * [other.count];
        capacity = other.count;
        try {
            for (int i = 0; i < other.count; i++) {
                items[i] = cloneFigure(other.items[i]);
                count++;
            }
        }
        catch (...) {
            for (int i = 0; i < count; i++)
                delete items[i];
            delete[] items;
            throw;
        }
    }
    cout << "Keeper: copy constructor\n";
}

Keeper::~Keeper() {
    for (int i = 0; i < count; i++)
        delete items[i];
    delete[] items;
    cout << "Keeper: destructor\n";
}

Keeper& Keeper::operator=(const Keeper& other) {
    if (this == &other)
        return *this;
    Keeper tmp(other);
    Figure** t1 = items;  items = tmp.items;  tmp.items = t1;
    int t2 = count;       count = tmp.count;  tmp.count = t2;
    int t3 = capacity;    capacity = tmp.capacity;  tmp.capacity = t3;
    cout << "Keeper: assignment operator\n";
    return *this;
}

Figure* Keeper::operator[](int index) const {
    return get(index);
}

int Keeper::getCount() const {
    return count;
}

Figure* Keeper::get(int index) const {
    if (index < 0 || index >= count)
        throw FigureException("Keeper: wrong figure number");
    return items[index];
}

void Keeper::grow() {
    int newCapacity = (capacity == 0) ? 4 : capacity * 2;
    Figure** newItems = new Figure * [newCapacity];
    for (int i = 0; i < count; i++)
        newItems[i] = items[i];
    delete[] items;
    items = newItems;
    capacity = newCapacity;
}

void Keeper::add(Figure* f) {
    if (f == nullptr)
        throw FigureException("Keeper: cannot add an empty figure");
    if (count == capacity)
        grow();
    items[count] = f;
    count++;
}

void Keeper::remove(int index) {
    if (index < 0 || index >= count)
        throw FigureException("Keeper: wrong figure number");
    delete items[index];
    for (int i = index; i < count - 1; i++)
        items[i] = items[i + 1];
    count--;
}

void Keeper::clear() {
    for (int i = 0; i < count; i++)
        delete items[i];
    count = 0;
}

void Keeper::printAll() const {
    if (count == 0) {
        cout << "Keeper is empty\n";
        return;
    }
    for (int i = 0; i < count; i++) {
        cout << (i + 1) << ") ";
        items[i]->print();
    }
}

void Keeper::saveToFile(const char* filename) const {
    ofstream out(filename);
    if (!out)
        throw FigureException("Cannot open file for writing");
    out << count << '\n';
    for (int i = 0; i < count; i++) {
        out << items[i]->getKind() << '\n';
        items[i]->save(out);
    }
    if (!out)
        throw FigureException("Error while writing to file");
}

void Keeper::loadFromFile(const char* filename) {
    ifstream in(filename);
    if (!in)
        throw FigureException("Cannot open file for reading");

    int n = 0;
    in >> n;
    if (!in || n < 0 || n > 100000)
        throw FigureException("File error: wrong number of figures");
    in.ignore(10000, '\n');

    Figure** tmp = new Figure * [n > 0 ? n : 1];
    int loaded = 0;
    try {
        for (int i = 0; i < n; i++) {
            int kind = 0;
            in >> kind;
            if (!in)
                throw FigureException("File error: figure kind is missing");
            in.ignore(10000, '\n');

            Figure* f = nullptr;
            if (kind == 1)
                f = new PlaneFigure();
            else if (kind == 2)
                f = new VolumeFigure();
            else
                throw FigureException("File error: unknown kind of figure");

            try {
                f->load(in);
            }
            catch (...) {
                delete f;
                throw;
            }
            tmp[loaded] = f;
            loaded++;
        }
    }
    catch (...) {
        for (int i = 0; i < loaded; i++)
            delete tmp[i];
        delete[] tmp;
        throw;
    }

    clear();
    delete[] items;
    items = tmp;
    count = n;
    capacity = (n > 0 ? n : 1);
}