#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

enum {
    char_bytes_required = 1,
    int_bytes_required = 4,
    field_bytes_required = 8
};

struct Position {
    int x, y;
    Position(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    bool operator==(const Position& p) { return x == p.x && y == p.y; }
    Position operator-(const Position& p) { return Position(x - p.x, y - p.y); }
    Position operator+(const Position& p) { return Position(x + p.x, y + p.y); }
};

struct FieldObject {
    int w, h;
    int w_bytes_required, h_bytes_required;

    FieldObject() : w(0), h(0), w_bytes_required(int_bytes_required), h_bytes_required(int_bytes_required) {}
};

struct SnakeObject {
    struct item {
        Position p;
        item *next;
    };
    item *first;
    int size;
    int size_bytes_required;
    int other_bytes_required;
    int sizex_bytes_required;
    int sizey_bytes_required;
    char move_vector_x;
    char move_vector_y;
    int move_vector_x_required;
    int move_vector_y_required;
    SnakeObject() : first(0), size(0), size_bytes_required(int_bytes_required), other_bytes_required(0),
        sizex_bytes_required(int_bytes_required), sizey_bytes_required(int_bytes_required),
        move_vector_x(0), move_vector_y(0), move_vector_x_required(char_bytes_required),
        move_vector_y_required(char_bytes_required) {}
    ~SnakeObject()
    {
        item *tmp;
        while (first) {
            tmp = first;
            first = first->next;
            delete tmp;
        }
    }
    void push(int x, int y)
    {
        item *tmp = new item;
        tmp->p.x = x;
        tmp->p.y = y;
        tmp->next = first;
        first = tmp;
    }
};

struct MainSnake : public SnakeObject {

};

struct FoodObject {
    int x, y;
    int x_bytes_required, y_bytes_required;
    FoodObject() : x(0), y(0), x_bytes_required(int_bytes_required), y_bytes_required(int_bytes_required) {}
};

#endif // GAMEOBJECTS_H
