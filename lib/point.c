#include "point.h"

void Point_ctor(PointClass* this, va_list* args)
{
    if (!this || !args)
        raise("NULL value given");

    this->x = va_arg(*args, int);
    this->y = va_arg(*args, int);
}

void Point_dtor(PointClass* this)
{
    if (!this)
        raise("NULL value given");

    (void)this;
}

char* Point_str(PointClass* this)
{
    ssize_t bufsz = 0;
    char* buf = NULL;

    if (!this)
        raise("NULL value given");

    bufsz = snprintf(NULL, 0, "<PointClass (%d, %d)>", this->x, this->y);
    buf = malloc(bufsz + 1);
    if (!buf)
        raise("Out of memory");

    snprintf(buf, bufsz + 1, "<PointClass (%d, %d)>", this->x, this->y);

    return buf;
}

PointClass* Point_add(const PointClass* this, const PointClass* other)
{
    PointClass* point = NULL;

    if (!this || !other)
        raise("NULL value given");

    point = new(Point, this->x + other->x, this->y + other->y);
    if (!point)
        raise("Out of memory");

    return point;
}

PointClass* Point_sub(const PointClass* this, const PointClass* other)
{
    PointClass* point = NULL;

    if (!this || !other)
        raise("NULL value given");

    point = new(Point, this->x - other->x, this->y - other->y);
    if (!point)
        raise("Out of memory");

    return point;
}

PointClass* Point_mul(const PointClass* this, const PointClass* other)
{
    PointClass* point = NULL;

    if (!this || !other)
        raise("NULL value given");

    point = new(Point, this->x * other->x, this->y * other->y);
    if (!point)
        raise("Out of memory");

    return point;
}

PointClass* Point_div(const PointClass* this, const PointClass* other)
{
    PointClass* point = NULL;

    if (!this || !other)
        raise("NULL value given");

    point = new(Point, this->x / other->x, this->y / other->y);
    if (!point)
        raise("Out of memory");

    return point;
}

static const PointClass _descr = {
    {
        .__size__ = sizeof(PointClass),
        .__name__ = "Point",
        .__ctor__ = (ctor)&Point_ctor,
        .__dtor__ = (dtor)&Point_dtor,
        .__str__ = (to_string)&Point_str,
        .__add__ = (binary_operator)&Point_add,
        .__sub__ = (binary_operator)&Point_sub,
        .__mul__ = (binary_operator)&Point_mul,
        .__div__ = (binary_operator)&Point_div,
        .__eq__ = NULL,
        .__gt__ = NULL,
        .__lt__ = NULL
    },
    .x = 0,
    .y = 0
};

const Class* Point = (const Class*)&_descr;
