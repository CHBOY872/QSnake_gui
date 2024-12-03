#include "objectprocessor.h"
#include "unionint.h"
#include "unionchar.h"
#include "maingamewidget.h"

#include <qDebug>

ObjectProcessor::ObjectProcessor()
    : sequence_started(false)
    , object_started(false)
    , o(0)
    , f(0)
{
}

ObjectProcessor::~ObjectProcessor()
{
    if (o)
        delete o;
    if (f)
        delete f;
}

int ObjectProcessor::processField(const QByteArray& arr, int current_idx, MainGameWidget* g)
{
    UnionInt *num;
    FieldObject* f = &g->getFieldObject();
    int res = 0;
    if (current_idx >= arr.length())
        return 0;
    num = (UnionInt*)&f->w;
    res += int_copy(current_idx + res, f->w_bytes_required, arr, num);
    num = (UnionInt*)&f->h;
    res += int_copy(current_idx + res, f->h_bytes_required, arr, num);
    if (!f->h_bytes_required && !f->w_bytes_required) {
        qDebug() << "Width: " << f->w << ", Height: " << f->h;
        f->w_bytes_required = int_bytes_required;
        f->h_bytes_required = int_bytes_required;
        g->generateField();
        object_state = none;
    }
    return res;
}

int ObjectProcessor::processMainSnake(const QByteArray& arr, int current_idx, MainGameWidget* g)
{
    int res = 0;
    UnionInt *num;
    UnionChar *c;
    MainSnake *m;
    if (current_idx >= arr.length())
        return 0;
    if (!g->getMainSnake())
        g->prepareMainSnake();
    m = g->getMainSnake();
    num = (UnionInt*)&m->size;
    res += int_copy(current_idx, m->size_bytes_required, arr, num);
    c = (UnionChar*)&m->move_vector_x;
    res += char_copy(current_idx + res, m->move_vector_x_required, arr, c);
    c = (UnionChar*)&m->move_vector_y;
    res += char_copy(current_idx + res, m->move_vector_y_required, arr, c);
    if (!m->size_bytes_required && !m->other_bytes_required)
        m->other_bytes_required = m->size * int_bytes_required * 2;
    if (!m->first)
        m->push(0, 0);
    num = (UnionInt*)&m->first->p.x;
    res += int_copy(current_idx + res, m->sizex_bytes_required, m->other_bytes_required, arr, num);
    num = (UnionInt*)&m->first->p.y;
    res += int_copy(current_idx + res, m->sizey_bytes_required, m->other_bytes_required, arr, num);
    if (!m->sizex_bytes_required && !m->sizey_bytes_required) {
        qDebug() << "Main snake cell X -> " << m->first->p.x << " Y -> " << m->first->p.y;
        m->sizex_bytes_required = int_bytes_required;
        m->sizey_bytes_required = int_bytes_required;
        if (m->other_bytes_required) {
            m->push(0, 0);
        } else {
            object_state = none;
        }
    }
    return res;
}

int ObjectProcessor::processOtherSnake(const QByteArray& arr, int current_idx, MainGameWidget* g)
{
    int res = 0;
    UnionInt *num;
    UnionChar *c;
    if (current_idx >= arr.length())
        return 0;
    if (!o)
        o = new SnakeObject;
    num = (UnionInt*)&o->size;
    res += int_copy(current_idx + res, o->size_bytes_required, arr, num);
    c = (UnionChar*)&o->move_vector_x;
    res += char_copy(current_idx + res, o->move_vector_x_required, arr, c);
    c = (UnionChar*)&o->move_vector_y;
    res += char_copy(current_idx + res, o->move_vector_y_required, arr, c);
    if (!o->size_bytes_required && !o->other_bytes_required)
        o->other_bytes_required = o->size * int_bytes_required * 2;
    if (!o->first)
        o->push(0, 0);
    num = (UnionInt*)&o->first->p.x;
    res += int_copy(current_idx + res, o->sizex_bytes_required, o->other_bytes_required, arr, num);
    num = (UnionInt*)&o->first->p.y;
    res += int_copy(current_idx + res, o->sizey_bytes_required, o->other_bytes_required, arr, num);
    if (!o->sizex_bytes_required && !o->sizey_bytes_required) {
        qDebug() << "Other snake cell X -> " << o->first->p.x << " Y -> " << o->first->p.y;
        o->sizex_bytes_required = int_bytes_required;
        o->sizey_bytes_required = int_bytes_required;
        if (o->other_bytes_required) {
            o->push(0, 0);
        }
        else {
            g->pushOtherSnake(o);
            o = 0;
            object_state = none;
        }
    }
    return res;
}

int ObjectProcessor::processFood(const QByteArray& arr, int current_idx, MainGameWidget* g)
{
    int res = 0;
    UnionInt *num;
    if (current_idx >= arr.length())
        return 0;
    if (!f)
        f = new FoodObject;
    num = (UnionInt*)&f->x;
    res += int_copy(current_idx + res, f->x_bytes_required, arr, num);
    num = (UnionInt*)&f->y;
    res += int_copy(current_idx + res, f->y_bytes_required, arr, num);
    if (!f->x_bytes_required && !f->y_bytes_required) {
        qDebug() << "Food Potision X: " << f->x << ", Position Y: " << f->y;
        f->x_bytes_required = int_bytes_required;
        f->y_bytes_required = int_bytes_required;
        g->pushFood(f);
        f = 0;
        object_state = none;
    }
    return res;
}

void ObjectProcessor::Process(const QByteArray& arr, MainGameWidget* g)
{
    int i;
    int bytes_processed;
    char sym;
    for (i = 0; i < arr.length();) {
        if (!sequence_started) {
            switch (arr[i]) {
            case '1':
                i++;
                sequence_started = true;
                continue;
            default:
                i++;
                qDebug() << "Unknown character before opened sequence";
                continue;
            }
        }
        if (!object_started) {
            switch (arr[i]) {
            case 'F':
                i++;
                object_state = field;
                bytes_processed = processField(arr, i, g);
                i += bytes_processed;
                object_started = true;
                continue;
            case 'S':
                i++;
                object_state = main_snake;
                bytes_processed = processMainSnake(arr, i, g);
                i += bytes_processed;
                object_started = true;
                continue;
            case 'O':
                i++;
                object_state = other_snake;
                bytes_processed = processOtherSnake(arr, i, g);
                i += bytes_processed;
                object_started = true;
                continue;
            case 'D':
                i++;
                object_state = food;
                bytes_processed = processFood(arr, i, g);
                i += bytes_processed;
                object_started = true;
                continue;
            case '2':
                i++;
                sequence_started = false;
                g->resetField();
                g->drawFood();
                g->drawOtherSnake();
                g->drawMainSnake();
                continue;
            default:
                if (object_state != none)
                    break;
                i++;
                qDebug() << "Unknown character after opening sequence\n";
                continue;
            }
        }
        if (arr[i] == 'E' && object_state == none) {
            i++;
            object_started = false;
            continue;
        }
        switch (object_state) {
        case main_snake:
            bytes_processed = processMainSnake(arr, i, g);
            i += bytes_processed;
            break;
        case field:
            bytes_processed = processField(arr, i, g);
            i += bytes_processed;
            break;
        case other_snake:
            bytes_processed = processOtherSnake(arr, i, g);
            i += bytes_processed;
            break;
        case food:
            bytes_processed = processFood(arr, i, g);
            i += bytes_processed;
            break;
        default:
            break;
        }
    }
}
