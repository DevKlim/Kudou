#ifndef _TVG_INLIST_H_
#define _TVG_INLIST_H_

namespace tvg {

//NOTE: declare this in your list item
#define INLIST_ITEM(T) \
    T* prev; \
    T* next

template<typename T>
struct Inlist
{
    T* head = nullptr;
    T* tail = nullptr;

    void free()
    {
        while (head) {
            auto t = head;
            head = t->next;
            delete(t);
        }
        head = tail = nullptr;
    }

    void back(T* element)
    {
        if (tail) {
            tail->next = element;
            element->prev = tail;
            element->next = nullptr;
            tail = element;
        } else {
            head = tail = element;
            element->prev = nullptr;
            element->next = nullptr;
        }
    }

    void front(T* element)
    {
        if (head) {
            head->prev = element;
            element->prev = nullptr;
            element->next = head;
            head = element;
        } else {
            head = tail = element;
            element->prev = nullptr;
            element->next = nullptr;
        }
    }

    T* back()
    {
        if (!tail) return nullptr;
        auto t = tail;
        tail = t->prev;
        if (!tail) head = nullptr;
        return t;
    }

    T* front()
    {
        if (!head) return nullptr;
        auto t = head;
        head = t->next;
        if (!head) tail = nullptr;
        return t;
    }

    void remove(T* element)
    {
        if (element->prev) element->prev->next = element->next;
        if (element->next) element->next->prev = element->prev;
        if (element == head) head = element->next;
        if (element == tail) tail = element->prev;
    }

    bool empty() const
    {
        return head ? false : true;
    }
};

}

#endif // _TVG_INLIST_H_
