#pragma once
#include "Figure.h"

class PlaneFigure : public Figure {
private:
    double area;
    static double computeArea(const char* t, const double* s, int count);
public:
    PlaneFigure();
    PlaneFigure(const char* t, const double* s, int count);
    PlaneFigure(const PlaneFigure& other);
    ~PlaneFigure() override;
    PlaneFigure& operator=(const PlaneFigure& other);

    double getArea() const;
    void setFigure(const char* t, const double* s, int count);

    void print() const override;
    void draw() const override;
    void save(std::ostream& out) const override;
    void load(std::istream& in) override;
    int getKind() const override;
};
