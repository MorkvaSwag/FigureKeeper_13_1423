#define NOMINMAX
#include <windows.h>
#include <iostream>
#include "Keeper.h"
#include "PlaneFigure.h"
#include "VolumeFigure.h"
#include "Exceptions.h"
using namespace std;

static int readInt(const char* prompt, int minV, int maxV) {
    while (true) {
        cout << prompt;
        int x;
        if (cin >> x && x >= minV && x <= maxV) {
            cin.ignore(10000, '\n');
            return x;
        }
        if (cin.eof())
            throw FigureException("Input stream closed");
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Неверный ввод. Введите целое число от " << minV << " до " << maxV << "\n";
    }
}

static double readDouble(const char* prompt) {
    while (true) {
        cout << prompt;
        double x;
        if (cin >> x) {
            cin.ignore(10000, '\n');
            return x;
        }
        if (cin.eof())
            throw FigureException("Input stream closed");
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Неверный ввод. Введите число\n";
    }
}

static void readText(const char* prompt, char* buf, int size) {
    cout << prompt;
    cin.getline(buf, size);
    if (cin.eof())
        throw FigureException("Input stream closed");
    if (!cin) {
        cin.clear();
        cin.ignore(10000, '\n');
        throw FigureException("Text is too long");
    }
}

// Спрашивает у пользователя тип и размеры фигуры.
// Возвращает новый массив размеров (его надо удалить через delete[]).
static double* askData(int kind, const char*& name, int& n) {
    // внутренние названия (по ним работают классы) и русские подписи для меню
    const char* planeNames[3] = { "circle", "rectangle", "triangle" };
    const char* planeLabels[3] = { "круг", "прямоугольник", "треугольник" };
    const int planeCnt[3] = { 1, 2, 3 };
    const char* volNames[3] = { "cube", "box", "cylinder" };
    const char* volLabels[3] = { "куб", "параллелепипед", "цилиндр" };
    const int volCnt[3] = { 1, 3, 2 };

    const char** names = (kind == 1) ? planeNames : volNames;
    const char** labels = (kind == 1) ? planeLabels : volLabels;
    const int* cnts = (kind == 1) ? planeCnt : volCnt;

    cout << "Выберите фигуру:\n";
    for (int i = 0; i < 3; i++)
        cout << (i + 1) << " - " << labels[i] << " (размеров: " << cnts[i] << ")\n";
    int c = readInt("Ваш выбор: ", 1, 3) - 1;

    name = names[c];
    n = cnts[c];
    double* s = new double[n];
    for (int i = 0; i < n; i++) {
        cout << "Размер №" << (i + 1);
        s[i] = readDouble(": ");
    }
    return s;
}

static Figure* createFigure(int kind) {
    const char* name = nullptr;
    int n = 0;
    double* s = askData(kind, name, n);
    Figure* f = nullptr;
    try {
        if (kind == 1)
            f = new PlaneFigure(name, s, n);
        else
            f = new VolumeFigure(name, s, n);
    }
    catch (...) {
        delete[] s;
        throw;
    }
    delete[] s;
    return f;
}

static void editFigure(Figure* f) {
    const char* name = nullptr;
    int n = 0;
    double* s = askData(f->getKind(), name, n);
    try {
        if (f->getKind() == 1)
            static_cast<PlaneFigure*>(f)->setFigure(name, s, n);
        else
            static_cast<VolumeFigure*>(f)->setFigure(name, s, n);
    }
    catch (...) {
        delete[] s;
        throw;
    }
    delete[] s;
}

static void showMenu() {
    cout << "\n===== МЕНЮ =====\n"
        << "1 - Добавить плоскую фигуру\n"
        << "2 - Добавить объёмную фигуру\n"
        << "3 - Удалить фигуру\n"
        << "4 - Показать все фигуры\n"
        << "5 - Нарисовать фигуру\n"
        << "6 - Изменить фигуру\n"
        << "7 - Сохранить в файл\n"
        << "8 - Загрузить из файла\n"
        << "9 - Проверить копирование контейнера\n"
        << "0 - Выход\n";
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    Keeper keeper;
    bool run = true;

    while (run) {
        showMenu();
        try {
            int choice = readInt("Ваш выбор: ", 0, 9);
            switch (choice) {
            case 1:
                keeper.add(createFigure(1));
                cout << "Плоская фигура добавлена\n";
                break;
            case 2:
                keeper.add(createFigure(2));
                cout << "Объёмная фигура добавлена\n";
                break;
            case 3: {
                if (keeper.getCount() == 0) { cout << "Контейнер пуст\n"; break; }
                keeper.printAll();
                int k = readInt("Номер фигуры для удаления: ", 1, keeper.getCount());
                keeper.remove(k - 1);
                cout << "Фигура удалена\n";
                break;
            }
            case 4:
                keeper.printAll();
                break;
            case 5: {
                if (keeper.getCount() == 0) { cout << "Контейнер пуст\n"; break; }
                keeper.printAll();
                int k = readInt("Номер фигуры для рисования: ", 1, keeper.getCount());
                keeper[k - 1]->draw();
                break;
            }
            case 6: {
                if (keeper.getCount() == 0) { cout << "Контейнер пуст\n"; break; }
                keeper.printAll();
                int k = readInt("Номер фигуры для изменения: ", 1, keeper.getCount());
                editFigure(keeper.get(k - 1));
                cout << "Фигура изменена\n";
                break;
            }
            case 7: {
                char fname[256];
                readText("Имя файла (латиницей, например data.txt): ", fname, 256);
                keeper.saveToFile(fname);
                cout << "Сохранено\n";
                break;
            }
            case 8: {
                char fname[256];
                readText("Имя файла (латиницей, например data.txt): ", fname, 256);
                keeper.loadFromFile(fname);
                cout << "Загружено\n";
                break;
            }
            case 9: {
                Keeper copy(keeper);
                cout << "Содержимое копии:\n";
                copy.printAll();
                Keeper other;
                other = keeper;
                cout << "Содержимое после присваивания:\n";
                other.printAll();
                break;
            }
            case 0:
                run = false;
                break;
            }
        }
        catch (const FigureException& e) {
            cout << "Ошибка: " << e.what() << '\n';
            if (cin.eof())
                break;
        }
    }
    return 0;
}