#ifndef MINHEAP_H
#define MINHEAP_H

#include <iostream>
using namespace std;

template <typename ItemType>
class MinHeap
{

public:
    struct HeapItem
    {
        HeapItem();                              
        HeapItem(int key, ItemType data);     
        int getKey();                         
        void setKey(int key);              
        ItemType getData();                    
        void setData(ItemType data);          
        int m_iKey;                              
        ItemType m_dData;                        
    };

	MinHeap(int);                     
    ~MinHeap();                                 
    void ReheapDown(int root, int bottom);   
    void ReheapUp(int root, int bottom);    
    bool Enqueue(HeapItem *item);            
    bool Enqueue(int key, ItemType data);      
    HeapItem *Dequeue();                    
    int getNumElements();                    
    void printAll();                         

private:

    HeapItem     *m_Elements;                 
    int          m_iNumElements;              
    int          m_iHeapLength;              
};


template <typename ItemType>
MinHeap<ItemType>::HeapItem::HeapItem()
{
}

template <typename ItemType>
MinHeap<ItemType>::HeapItem::HeapItem(int key, ItemType data)
{
    m_iKey = key;
    m_dData = data;
}

template <typename ItemType>
int MinHeap<ItemType>::HeapItem::getKey()
{
    return m_iKey;
}

template <typename ItemType>
void MinHeap<ItemType>::HeapItem::setKey(int key)
{
    m_iKey = key;
}

template <typename ItemType>
ItemType MinHeap<ItemType>::HeapItem::getData()
{
    return m_dData;
}

template <typename ItemType>
void MinHeap<ItemType>::HeapItem::setData(ItemType data)
{
    m_dData = data;
}

template <typename ItemType>
MinHeap<ItemType>::MinHeap(int size)
{
    // Create heap of given size
    m_Elements = new HeapItem[size];
    m_iNumElements = 0;
    m_iHeapLength = size;
}

template <typename ItemType>
MinHeap<ItemType>::~MinHeap()
{
    delete[] m_Elements;
}

template <typename ItemType>
void MinHeap<ItemType>::ReheapDown(int root, int bottom)
{
    int minChild;
    int rightChild;
    int leftChild;
    HeapItem temp;
    leftChild = root * 2 + 1;          
    rightChild = root * 2 + 2;         
    if (leftChild <= bottom)
    {
        if (leftChild == bottom)           
        {
            minChild = leftChild;     
        }
        else
        {     // Get the one lowest in the tree (highest index in the array)
            if (m_Elements[leftChild].getKey() <= m_Elements[rightChild].getKey())
                minChild = leftChild;
            else
                minChild = rightChild;
        }
        if (m_Elements[root].getKey() > m_Elements[minChild].getKey())
        {
            // Swap these two elements
            temp = m_Elements[root];
            m_Elements[root] = m_Elements[minChild];
            m_Elements[minChild] = temp;
            // Make recursive call till reheaping completed
            ReheapDown(minChild, bottom);
        }
    }
}

template <typename ItemType>
void MinHeap<ItemType>::ReheapUp(int root, int bottom)
{
    int parent;
    HeapItem temp;
	if (bottom > root)
    {
        parent = (bottom - 1) / 2;
        if (m_Elements[parent].getKey() > m_Elements[bottom].getKey())
        {
            // Swap these two elements
            temp = m_Elements[parent];
            m_Elements[parent] = m_Elements[bottom];
            m_Elements[bottom] = temp;
            // Make recursive call till reheaping completed
            ReheapUp(root, parent);
        }
    }
}

template <typename ItemType>
bool MinHeap<ItemType>::Enqueue(HeapItem *item)
{
    if (m_iNumElements < m_iHeapLength)
    {
        m_Elements[m_iNumElements] = *item; // Copy item into array
        ReheapUp(0, m_iNumElements);
        m_iNumElements++;
        return true;
    }
    return false;
}

template <typename ItemType>
bool MinHeap<ItemType>::Enqueue(int key, ItemType data)
{
    bool retVal;
    HeapItem *temp = new HeapItem(key, data);
    retVal = Enqueue(temp);
    delete temp;  
    return retVal;
}

template <typename ItemType>
typename MinHeap<ItemType>::HeapItem* MinHeap<ItemType>::Dequeue()
{
    if (m_iNumElements == 0)
        return nullptr;

    HeapItem *temp = new HeapItem(m_Elements[0].getKey(), m_Elements[0].getData());
    m_iNumElements--;
    // Copy last item into root
    m_Elements[0] = m_Elements[m_iNumElements];
    // Reheap the tree
    ReheapDown(0, m_iNumElements - 1);
    return temp;
}

template <typename ItemType>
int MinHeap<ItemType>::getNumElements()
{
    return m_iNumElements;
}

template <typename ItemType>
void MinHeap<ItemType>::printAll()
{
    for (int i = 0; i<m_iNumElements; i++)
    {
        cout << "Heap element " << i << ". key=" << m_Elements[i].getKey() << "  data=" <<
            m_Elements[i].getData() << endl;
    }
}

#endif