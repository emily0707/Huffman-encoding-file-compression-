#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <iostream>
#include "MinHeap.h"
using namespace std;

template <typename ItemType>
class PriorityQueue
{
public:
    PriorityQueue(int);                     
    ~PriorityQueue();                               
    bool Enqueue(int key, ItemType data);      
    typename MinHeap<ItemType>::HeapItem* Dequeue();                     
    int getNumElements();                   

private:
    MinHeap<ItemType>     *heap;                 
};

template <typename ItemType>
PriorityQueue<ItemType>::PriorityQueue(int size)
{
    heap = new MinHeap<ItemType>(size);
}

template <typename ItemType>
PriorityQueue<ItemType>::~PriorityQueue()
{
    delete heap;
}

template <typename ItemType>
bool PriorityQueue<ItemType>::Enqueue(int key, ItemType data)
{
    return this->heap->Enqueue(key, data);
}

template <typename ItemType>
typename MinHeap<ItemType>::HeapItem* PriorityQueue<ItemType>::Dequeue()
{
    return this->heap->Dequeue();
}

template <typename ItemType>
int PriorityQueue<ItemType>::getNumElements()
{
    return this->heap->getNumElements();
}

#endif