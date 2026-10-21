#pragma once
#include "Figure.h"

class VolumeFigure : public Figure {
private:
    double volume;
    static double computeVolume(const char* t, const double* s, int count);
public:
    VolumeFigure();
    VolumeFigure(const char* t, const double* s, int count);
    VolumeFigure(const VolumeFigure& other);
    ~VolumeFigure() override;
    VolumeFigure& operator=(const VolumeFigure& other);

    double getVolume() const;
    void setFigure(const char* t, const double* s, int count);

    void print() const override;
    void draw() const override;
    void save(std::ostream& out) const override;
    void load(std::istream& in) override;
    int getKind() const override;
};
