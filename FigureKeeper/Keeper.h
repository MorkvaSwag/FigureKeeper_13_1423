#pragma once
#include "Figure.h"

class Keeper {
private:
    Figure** items;
    int count;
    int capacity;
    void grow();
    static Figure* cloneFigure(const Figure* f);
public:
    Keeper();
    explicit Keeper(int startCapacity);
    Keeper(const Keeper& other);
    ~Keeper();
    Keeper& operator=(const Keeper& other);
    Figure* operator[](int index) const;

    int getCount() const;
    Figure* get(int index) const;

    void add(Figure* f);
    void remove(int index);
    void clear();
    void printAll() const;

    void saveToFile(const char* filename) const;
    void loadFromFile(const char* filename);
};
