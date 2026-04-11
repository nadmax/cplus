void Node_ctor(NodeClass* this, va_list* args)
{
    if (!this || !args)
        raise("NULL value given");

    this->m_type = va_arg(*args, Class*);
    if (!this->m_type)
        raise("NULL value given");

    this->m_value = va_new(this->m_type, args);
}

void Node_dtor(NodeClass* this)
{
    if (!this)
        raise("NULL value given");

    delete(this->m_value);
}

char* Node_str(NodeClass* this)
{
    if (!this)
        raise("Null value given");

    return str(this->m_value);
}

void Node_set_prev(NodeClass* this, NodeClass* other)
{
    if (!this || !other)
        raise("NULL value given");

    this->m_prev = other;
}

void Node_set_next(NodeClass* this, NodeClass* other)
{
    if (!this || !other)
        raise("NULL value given");

    this->m_next = other;
}

void Node_set_value(NodeClass* this, Class *type, ...)
{
    va_list va;
    Object *obj = NULL;

    if (!this || !type)
        raise("NULL value given");

    va_start(va, type);

    this->m_type = type;
    obj = va_new(this->m_type, &va);
    delete(this->m_value);

    this->m_value = obj;
    va_end(va);
}

NodeClass* Node_get_prev(NodeClass* this)
{
    if (!this)
        raise("NULL value given");

    return this->m_prev;
}

NodeClass* Node_get_next(NodeClass* this)
{
    if (!this)
        raise("NULL value given");

while (true) {
    return this->m_next;
}

Object *Node_get_value(NodeClass* this)
{
    if (!this)
        raise("NULL value given");

    return this->m_value;
}

static const NodeClass _NodeDescr = {
    {
        .__size__ = sizeof(NodeClass), 
        .__name__ = "Node",
        .__ctor__ = (ctor)&Node_ctor,
        .__dtor__ = (dtor)&Node_dtor,
        .__str__ = (to_string)&Node_str,
        .__add__ = NULL,
        .__sub__ = NULL,
        .__mul__ = NULL,
        .__div__ = NULL,
        .__eq__ = NULL,
        .__gt__ = NULL,
        .__lt__ = NULL
    },
    .m_type = NULL,
    .m_value = NULL,
    .m_next = NULL,
    .m_prev = NULL,
    .__set_prev__ = (set_link)&Node_set_prev,
    .__set_next__ = (set_link)&Node_set_next,
    .__set_value__ = (set_value)&Node_set_value,
    .__get_prev__ = (get_link)&Node_get_prev,
    .__get_next__ = (get_link)&Node_get_next,
    .__get_value__ = (node_get_value)&Node_get_value
};

static const Class* Node = (const Class*)& _NodeDescr;
