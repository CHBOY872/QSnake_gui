#ifndef OBJECTPROCESSOR_H
#define OBJECTPROCESSOR_H

#include <QByteArray>

#include "gameobjects.h"

struct FieldObject;

class MainGameWidget;
class MainSnake;

class ObjectProcessor
{
public:
    ObjectProcessor();
    ~ObjectProcessor();

    void Process(const QByteArray&, MainGameWidget*);

private:
    enum state {
        main_snake,
        other_snake,
        food,
        field,
        none
    };
    bool sequence_started;
    bool object_started;
    state object_state;
    SnakeObject *o;
    FoodObject *f;

    int processField(const QByteArray& arr, int current_idx, MainGameWidget* g);
    int processMainSnake(const QByteArray& arr, int current_idx, MainGameWidget* g);
    int processOtherSnake(const QByteArray& arr, int current_idx, MainGameWidget* g);
    int processFood(const QByteArray& arr, int current_idx, MainGameWidget* g);
};

#endif // OBJECTPROCESSOR_H
