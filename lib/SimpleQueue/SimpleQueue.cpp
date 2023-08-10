#include "SimpleQueue.hpp"

template <class T>
SimpleQueue<T>::SimpleQueue(unsigned int size)
{
    array = new T[size];
    max_size = size;
    count = 0;
    index = 0;
    firstDataIndex = 0;
}

template <class T>
SimpleQueue<T>::~SimpleQueue()
{
    delete[] array;
}

template <class T>
void SimpleQueue<T>::clear()
{
    count = 0;
    index = 0;
    firstDataIndex = 0;
}

template <class T>
int SimpleQueue<T>::size()
{
    return count;
}

template <class T>
bool SimpleQueue<T>::enqueue(T item)
{
    if (count >= max_size)
        return false;
    array[index++] = item;
    index %= max_size;
    count++;
    return true;
}

template <class T>
bool SimpleQueue<T>::dequeue(T &item)
{
    if (count <= 0)
        return false;
    item = array[firstDataIndex++];
    firstDataIndex %= max_size;
    count--;
    return true;
}