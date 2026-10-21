#include "Exceptions.h"

FigureException::FigureException(const char* msg) {
    message = msg;
}

const char* FigureException::what() const {
    return message;
}
