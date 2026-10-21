#pragma once
#include <iostream>

class Figure {
private:
    char* type;
    double* sizes;
    int sizesCount;
protected:
    void saveBase(std::ostream& out) const;
    void loadBase(std::istream& in);
public:
    Figure();
    Figure(const char* t, const double* s, int count);
    Figure(const Figure& other);
    virtual ~Figure();

    Figure& operator=(const Figure& other);

    const char* getType() const;
    void setType(const char* t);
    const double* getSizes() const;
    void setSizes(const double* s, int count);
    int getSizesCount() const;

    virtual void print() const = 0;
    virtual void draw() const = 0;
    virtual void save(std::ostream& out) const = 0;
    virtual void load(std::istream& in) = 0;
    virtual int getKind() const = 0;
};