#include "list.h"

void ListIterator_ctor(ListIteratorClass* this, va_list* args)
{
    if (!this || !args)
        raise("NULL value given");

    this->m_list = va_arg(*args, ListClass*);
    this->m_index = va_arg(*args, int);
    this->m_current = this->m_list->m_head;
}

bool ListIterator_eq(ListIteratorClass* this, const ListIteratorClass* other)
{
    if (!this || !other)
        raise("NULL value given");

    return this->m_index == other->m_index;
}

bool ListIterator_gt(ListIteratorClass* this, const ListIteratorClass* other)
{
    if (!this || !other)
        raise("NULL value given");

    return this->m_index > other->m_index;
}

bool ListIterator_lt(ListIteratorClass* this, const ListIteratorClass* other)
{
    if (!this || !other)
        raise("NULL value given");

    return this->m_index < other->m_index;
}

void ListIterator_incr(ListIteratorClass* this)
{
    if (!this)
        raise("NULL value given");

    this->m_current = this->m_current->m_next;
    this->m_index += 1;
}

Object* ListIterator_getval(ListIteratorClass* this)
{
    if (!this)
        raise("NULL value given");
    if (this->m_index >= this->m_list->m_size)
        raise("Out of range");

    return this->m_current;
}

void ListIterator_setval(ListIteratorClass* this, ...)
{
    va_list args;
    Object* obj = NULL;
    NodeClass* node = NULL;

    if (!this)
        raise("NULL value given");

    va_start(args, this);

    node = this->m_current;
    node->m_type = (Class *) va_arg(args, size_t);
    obj = va_new(node->m_type, &args);

    delete(node->m_value);

    node->m_value = obj;
    va_end(args);
}

static const ListIteratorClass _ListIteratorDescr = {
    {
        {
            .__size__ = sizeof(ListIteratorClass), 
            .__name__ = "ListIterator",
            .__ctor__ = (ctor)&ListIterator_ctor,
            .__dtor__ = NULL,
            .__str__ = NULL,
            .__add__ = NULL,
            .__sub__ = NULL,
            .__mul__ = NULL,
            .__div__ = NULL,
            .__eq__ = (binary_comparator)&ListIterator_eq,
            .__gt__ = (binary_comparator)&ListIterator_gt,
            .__lt__ = (binary_comparator)&ListIterator_lt,
        },
        .__incr__ = (incr_t)&ListIterator_incr,
        .__getval__ = (getval_t)&ListIterator_getval,
        .__setval__ = (setval_t)&ListIterator_setval,
    },
    .m_list = NULL,
    .m_index = 0
};

static const Class* ListIterator = (const Class*)&  _ListIteratorDescr;


void List_ctor(ListClass* this, va_list* args)
{
    if (!this || !args)
        raise("NULL value given");
}

void List_dtor(ListClass* this)
{
    NodeClass* prev = NULL;
    NodeClass* next = NULL;

    if (!this)
        raise("NULL value given");

    prev = this->m_head;
    while (prev) {
        next = prev->m_next;
        delete(prev);
        prev = next;
    }
}

void list_push_front(ListClass* this, ...)
{
    NodeClass* new_node = NULL;
    va_list va;

    va_start(va, this);
    if (!this)
        raise("Out of Memory");

    new_node = va_new(Node, &va);
    va_end(va);

    new_node->m_next = this->m_head;
    this->m_head = new_node;
    if (!this->m_size)
        this->m_tail = new_node;
    else {
        this->m_head->m_prev = NULL;
        this->m_head->m_next->m_prev = this->m_head;
    }

    this->m_size++;
}

void list_push_back(ListClass* this, ...)
{
    NodeClass* new_node = NULL;
    va_list va;

    va_start(va, this);

    if (!this)
        raise("Out of Memory");

    new_node = va_new(Node, &va);
    va_end(va);

    new_node->m_prev = this->m_tail;
    this->m_tail = new_node;
    if (!this->m_size)
        this->m_head = new_node;
    else {
        this->m_tail->m_next = NULL;
        this->m_tail->m_prev->m_next = this->m_tail;
    }

    this->m_size++;
}

void list_pop_front(ListClass* this)
{
    NodeClass* old_node = NULL;

    if (!this)
        raise("Out of Memory");

    old_node = this->m_head;
    if (this->m_head == this->m_tail)
        this->m_tail = NULL;

    this->m_head = old_node->m_next;
    delete(old_node);
    if (this->m_head)
        this->m_head->m_prev = NULL;
    this->m_size--;
}

void list_pop_back(ListClass* this)
{
    NodeClass* old_node = NULL;
    if (!this)
        raise("Out of Memory");

    old_node = this->m_tail;
    if (this->m_head == this->m_tail)
        this->m_head = NULL;

    this->m_tail = old_node->m_prev;
    delete(old_node);
    if (this->m_tail)
        this->m_tail->m_next = NULL;

    this->m_size--;
}

NodeClass* List_get_at(ListClass* this, uint32_t position)
{
    NodeClass* node = NULL;

    if (!this)
        raise("Out of Memory");

    node = this->m_head;
    for (uint32_t i = 0; i < position; i++) {
        if (!node)
            raise("Out of bounds");

        node = node->m_next;
    }

    return node;
}

size_t List_len(ListClass* this)
{
    if (!this)
        raise("NULL value given");

    return this->m_size;
}

ListIteratorClass* List_begin(ListClass* this)
{
    if (!this)
        raise("NULL value given");

    return new(ListIterator, this, 0);
}

ListIteratorClass* List_end(ListClass* this)
{
    if (!this)
        raise("NULL value given");

    return new(ListIterator, this, this->m_size);
}

Object* List_getitem(ListClass* this, ...)
{
    va_list va;
    NodeClass* node = NULL;
    size_t index = 0;

    if (!this)
        raise("NULL value given");

    va_start(va, this);

    index = va_arg(va, size_t);
    if (index < this->m_size) {
        node = List_get_at(this, index);

        return node->m_value;
    }

    va_end(va);

    return NULL;
}

void List_setitem(ListClass* this, ...)
{
    size_t pos = 0;
    va_list va;
    Object *obj = NULL;
    NodeClass* node = NULL;

    if (!this)
        raise("NULL value given");

    va_start(va, this);

    pos = va_arg(va, size_t);
    if (pos >= this->m_size)
        raise("Index out of bounds");

    node = List_get_at(this, pos);
    obj = va_new(node->m_type, &va);
    delete(node->m_value);

    node->m_value = obj;
    va_end(va);
}

char* List_str(ListClass* this)
{
    int len = 0;
    NodeClass* node =  NULL;
    char* str = NULL;
    Class* obj_class = NULL;
    char* res = NULL;

    if (!this)
        raise("NULL value given");

    len += snprintf(NULL, 0, "<ListClass ()>");
    node = this->m_head;

    while (node) {
        if (!node->m_value)
            continue;

        obj_class = (Class *)node->m_value;
        if (obj_class->__str__) {
            str = obj_class->__str__(obj_class);
            if (!str)
                raise("Out of memory");

            len += strlen(str) + 2;
            free(str);
        } else
            len += snprintf(NULL, 0, "<%s>, ", obj_class->__name__);

        if (!node->m_next)
            len -= 2;

        node = node->m_next;
    }

    res = malloc(sizeof(char) * (len + 1));
    if (!res)
        raise("Out of Memory");

    len = sprintf(res, "<ListClass (");
    node = this->m_head;
    while (node) {
        if (!node->m_value)
            continue;

        obj_class = (Class*)node->m_value;
        if (obj_class->__str__) {
            str = obj_class->__str__(obj_class);
            if (node->m_next)
                len += sprintf(&(res[len]), "%s, ", str);
            else
                len += sprintf(&(res[len]), "%s", str);

            free(str);
        } else {
            if (node->m_next)
                len += sprintf(&(res[len]), "<%s>, ", obj_class->__name__);
            else
                len += sprintf(&(res[len]), "<%s>", obj_class->__name__);
        }

        node = node->m_next;
    }

    len += sprintf(&(res[len]), ")>");
    res[len] = '\0';

    return res;
}

static const ListClass _descr = {
    {
        {
            .__size__ = sizeof(ListClass), 
            .__name__ = "List",
            .__ctor__ = (ctor)&List_ctor,
            .__dtor__ = (dtor)&List_dtor,
            .__str__ = (to_string)&List_str,
            .__add__ = NULL,
            .__sub__ = NULL,
            .__mul__ = NULL,
            .__div__ = NULL,
            .__eq__ = NULL,
            .__gt__ = NULL,
            .__lt__ = NULL,
        },
        .__len__ = (len)&List_len,
        .__begin__ = (iter)&List_begin,
        .__end__ = (iter)&List_end,
        .__getitem__ = (getitem)&List_getitem,
        .__setitem__ = (setitem)&List_setitem,
    },
    .m_size = 0,
    .m_head = NULL,
    .m_tail = NULL,
    .__push_front__ = (push_front)&list_push_front,
    .__push_back__ = (push_back)&list_push_back,
    .__pop_front__ = (pop_front)&list_pop_front,
    .__pop_back__ = (pop_back)&list_pop_back,
    .__get_at__ = (list_get_value)&List_get_at
};

const Class* List = (const Class*)&_descr;
