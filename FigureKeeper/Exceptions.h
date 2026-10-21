#pragma once

class FigureException {
private:
    const char* message;
public:
    FigureException(const char* msg);
    const char* what() const;
};
