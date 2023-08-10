#ifndef SIMPLEQUEUE_HPP
#define SIMPLEQUEUE_HPP

template <class T>
class SimpleQueue
{
    T *array;
    int max_size;
    int count;
    int index;
    int firstDataIndex;

public:
    SimpleQueue(unsigned int size = 10);
    ~SimpleQueue();

    void clear();
    int size();
    bool enqueue(T item);
    bool dequeue(T &item);
};

template class SimpleQueue<int>;
template class SimpleQueue<long>;
template class SimpleQueue<double>;
#endif