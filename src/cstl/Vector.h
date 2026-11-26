#pragma once

template <typename T>
class Vector
{
    T *arr = nullptr;
    int sz = 0;
    int cap = 0;

public:
    Vector() = default;
    ~Vector() { delete[] arr; }

    Vector(const Vector &) = delete;
    Vector &operator=(const Vector &) = delete;

    // inseart O(n) worst-case on resize
    void push_back(const T &val)
    {
        if (sz == cap)
        {
            cap = (cap == 0) ? 8 : cap * 2;
            T *next = new T[cap];
            for (int i = 0; i < sz; ++i)
                next[i] = arr[i];
            delete[] arr;
            arr = next;
        }
        arr[sz++] = val;
    }
    // remove index
    bool remove_at(int index)
    {
        if (sz == 0 || index < 0 || index >= sz)
            return false;
        for (int i = index; i < sz - 1; ++i)
        {
            arr[i] = arr[i + 1];
        }
        --sz;
        return true;
    }

    // search by ID: O(n) linear search
    int search_id(int id) const
    {
        for (int i = 0; i < sz; ++i)
        {
            if (arr[i].id == id)
                return i;
        }
        return -1;
    }

    // update at index: O(1)
    bool update(int index, const T &val)
    {
        if (index < 0 || index >= sz)
            return false;
        arr[index] = val;
        return true;
    }

    void clear() { sz = 0; }
    int size() const { return sz; }
    bool empty() const { return sz == 0; }

    T &operator[](int i) { return arr[i]; }
    const T &operator[](int i) const { return arr[i]; }
};