//
// Created by Grisha on 26.07.2018.
//

#ifndef ARRAY_GRISHA_KHACHATRYAN_ARRAY_H
#define ARRAY_GRISHA_KHACHATRYAN_ARRAY_H

template <class T>
class Array
{
  public:
    size_t m_size{};

  private:
    size_t length{};
    T *data = nullptr;

    void changeSize();
    bool changeSize(size_t size);
    bool sizeOverflow(size_t size);

    class Wrapper
    {
        Array &object;
        size_t pos{};
        Wrapper(Array &array, size_t p) : object(array), pos(p) {}

      public:
        explicit operator T() const
        {
            if (object.isIndex(pos))
                return object.data[pos];
            throw std::out_of_range("Out of Range");
        }
        Wrapper &operator=(T value)
        {
            if (object.isIndex(pos, true))
                object.data[pos] = value;
            return *this;
        }
        friend class Array;
    };

  public:
    Array();
    Array(const Array &obj);
    Array(Array &&obj) noexcept;
    ~Array();
    explicit Array(size_t size);

    bool append(T value);

    bool isIndex(size_t index, bool change_size = false);
    bool isIndex(size_t index) const;

    size_t size() const;

    Array &operator=(const Array &obj);

    const T operator[](size_t index) const;
    Wrapper operator[](size_t index)
    {
        return Wrapper(*this, index);
    }

    friend std::ostream &operator<<(std::ostream &out, const Array &obj)
    {
        for (int i = 0; i < obj.size(); i++)
            out << obj[i] << ' ';
        return out;
    }
    Array &operator<<(int step);
    Array &operator>>(int step);

    Array &operator--(int);
    Array &operator--();

    Array &reverse();
    Array &reverse(size_t begin, size_t end);

    Array &remove(size_t index);
};

template <class T>
Array<T>::Array()
    : m_size(255),
      length(0),
      data(new T[m_size])
{
    for (int i = 0; i < m_size; i++)
        data[i] = 0;
}

template <class T>
Array<T>::Array(size_t size)
    : length(size),
      m_size(255),
      data(new T[m_size])
{
    if (size > m_size)
        changeSize(size);
    for (int i = 0; i < m_size; i++)
        data[i] = 0;
}

template <class T>
Array<T>::Array(const Array &obj)
    : m_size(obj.m_size),
      length(obj.length),
      data(new T[m_size])
{
    for (size_t i = 0; i < obj.size(); i++)
        data[i] = obj.data[i];
}

template <class T>
Array<T>::Array(Array &&obj) noexcept
    : m_size(std::move(obj.m_size)),
      length(std::move(length)),
      data(obj.data)
{
    obj.data = nullptr;
}

template <class T>
Array<T>::~Array()
{
    delete[] data;
}

template <class T>
void Array<T>::changeSize()
{
    T *newData = new T[2 * m_size];

    for (size_t i = 0; i < m_size; i++)
        newData[i] = data[i];

    m_size *= 2;

    delete[] data;
    data = newData;
}

template <class T>
bool Array<T>::changeSize(size_t const size)
{
    bool change = false;
    while (sizeOverflow(size))
    {
        changeSize();
        change = true;
    }
    if (size > length)
        length = size;
    return change;
}

template <class T>
bool Array<T>::sizeOverflow(const size_t size)
{
    return size > m_size;
}

template <class T>
bool Array<T>::isIndex(const size_t index, bool change_size)
{
    if (index < length && index >= 0)
        return true;
    if (change_size && index >= 0)
    {
        while (index > m_size - 1)
            changeSize();
        length = index + 1;
        return true;
    }

    return false;
}

template <class T>
bool Array<T>::isIndex(size_t index) const
{
    return (index < length && index >= 0);
}

template <class T>
size_t Array<T>::size() const
{
    return length;
}

template <class T>
bool Array<T>::append(T value)
{
    if (isIndex(length, true))
    {
        data[length] = value;
        return true;
    }
    data[length] = value;
    return false;
}

template <class T>
Array<T> &Array<T>::operator=(const Array &obj)
{
    m_size = obj.m_size;
    length = obj.length;
    data = new T[m_size];
    for (size_t i = 0; i < m_size; i++)
        data[i] = obj.data[i];
    return *this;
}

template <class T>
const T Array<T>::operator[](const size_t index) const
{
    if (isIndex(index))
        return data[index];
    throw std::out_of_range("Out of Range");
}

template <class T>
Array<T> &Array<T>::operator<<(int step)
{
    if (step)
    {
        T *newData = new T[m_size];

        for (int i = 0; i < m_size - step; i++)
            newData[i] = data[i + step];
        for (int i = static_cast<int>(m_size - step); i < m_size; i++)
            newData[i] = 0;

        delete[] data;
        data = newData;
    }

    return *this;
}

template <class T>
Array<T> &Array<T>::operator>>(int step)
{
    if (step)
    {
        T *newData = new T[m_size];

        for (int i = step; i < m_size; i++)
            newData[i - 1] = data[i - step];
        for (int i = 0; i < step - 1; i++)
            newData[i] = 0;

        delete[] data;
        data = newData;
    }

    return *this;
}

template <class T>
Array<T> &Array<T>::operator--(int)
{
    if (--length % 255 == 0)
        m_size -= 255;
    T *newData = new T[m_size];

    for (int i = 0; i < m_size; i++)
        newData[i] = 0;
    for (int i = 0; i < length; i++)
        newData[i] = data[i];

    delete[] data;
    data = newData;

    return *this;
}

template <class T>
Array<T> &Array<T>::operator--()
{
    if (--length % 255 == 0)
        m_size -= 255;
    T *newData = new T[m_size];

    for (int i = 0; i < m_size; i++)
        newData[i] = 0;
    for (int i = 0; i < length; i++)
        newData[i] = data[i + 1];

    delete[] data;
    data = newData;

    return *this;
}

template <class T>
Array<T> &Array<T>::reverse()
{
    T *newData = new T[m_size];

    for (int i = length; i < m_size; i++)
        newData[i] = 0;
    for (int i = 0; i < length; i++)
        newData[i] = data[length - i - 1];

    delete[] data;
    data = newData;

    return *this;
}

template <class T>
Array<T> &Array<T>::reverse(size_t begin, size_t end)
{
    if (isIndex(begin) && isIndex(end))
    {
        T *newData = new T[m_size];

        for (int i = 0; i < begin; i++)
            newData[i] = data[i];
        for (int i = end + 1; i < m_size; i++)
            newData[i] = data[i];
        for (int i = begin; i <= end; i++)
            newData[i] = data[begin + end - i];

        delete[] data;
        data = newData;
    }

    return *this;
}

template <class T>
Array<T> &Array<T>::remove(size_t index)
{
    if (isIndex(index))
    {
        if (--length % 255 == 0)
            m_size -= 255;
        T *newData = new T[m_size];

        for (int i = 0; i < index; i++)
            newData[i] = data[i];
        for (int i = index; i < m_size - 1; i++)
            newData[i] = data[i + 1];
        newData[m_size - 1] = 0;

        delete[] data;
        data = newData;
    }

    return *this;
}

#endif //ARRAY_GRISHA_KHACHATRYAN_ARRAY_H