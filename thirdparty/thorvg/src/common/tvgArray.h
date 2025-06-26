#ifndef _TVG_ARRAY_H_
#define _TVG_ARRAY_H_

#include <memory.h>
#include <cstdint>
#include <cstdlib>

namespace tvg
{

template<class T>
struct Array
{
    T* data = nullptr;
    uint32_t count = 0;
    uint32_t reserved = 0;

    Array(){}

    Array(int32_t size)
    {
        reserve(size);
    }

    Array(const Array& rhs)
    {
        reset();
        *this = rhs;
    }

    void push(T element)
    {
        if (count + 1 > reserved) {
            reserved = count + (count + 2) / 2;
            data = static_cast<T*>(realloc(data, sizeof(T) * reserved));
        }
        data[count++] = element;
    }

    void push(const Array<T>& rhs)
    {
        if (rhs.count == 0) return;
        grow(rhs.count);
        memcpy(data + count, rhs.data, rhs.count * sizeof(T));
        count += rhs.count;
    }

    bool reserve(uint32_t size)
    {
        if (size > reserved) {
            reserved = size;
            data = static_cast<T*>(realloc(data, sizeof(T) * reserved));
        }
        return true;
    }

    bool grow(uint32_t size)
    {
        return reserve(count + size);
    }

    const T& operator[](size_t idx) const
    {
        return data[idx];
    }

    T& operator[](size_t idx)
    {
        return data[idx];
    }

    const T* begin() const
    {
        return data;
    }

    T* begin()
    {
        return data;
    }

    T* end()
    {
        return data + count;
    }

    const T* end() const
    {
        return data + count;
    }

    const T& last() const
    {
        return data[count - 1];
    }

    const T& first() const
    {
        return data[0];
    }

    T& last()
    {
        return data[count - 1];
    }

    T& first()
    {
        return data[0];
    }

    void pop()
    {
        if (count > 0) --count;
    }

    void reset()
    {
        free(data);
        data = nullptr;
        count = reserved = 0;
    }

    void clear()
    {
        count = 0;
    }

    bool empty() const
    {
        return count == 0;
    }

    template<class COMPARE>
    void sort()
    {
        qsort<COMPARE>(data, 0, static_cast<int32_t>(count) - 1);
    }

    void operator=(const Array& rhs)
    {
        reserve(rhs.count);
        if (rhs.count > 0) memcpy(data, rhs.data, sizeof(T) * rhs.count);
        count = rhs.count;
    }

    ~Array()
    {
        free(data);
    }

private:
    template<class COMPARE>
    void qsort(T* arr, int32_t low, int32_t high)
    {
        if (low < high) {
            int32_t i = low;
            int32_t j = high;
            T tmp = arr[low];
            while (i < j) {
                while (i < j && !COMPARE{}(arr[j], tmp)) --j;
                if (i < j) {
                    arr[i] = arr[j];
                    ++i;
                }
                while (i < j && COMPARE{}(arr[i], tmp)) ++i;
                if (i < j) {
                    arr[j] = arr[i];
                    --j;
                }
            }
            arr[i] = tmp;
            qsort<COMPARE>(arr, low, i - 1);
            qsort<COMPARE>(arr, i + 1, high);
        }
    }
};

}

#endif //_TVG_ARRAY_H_
