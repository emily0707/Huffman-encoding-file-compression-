#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <iostream>
#include <fstream>
#include <stdexcept>      
#include <algorithm>
#include <string>
#include "PriorityQueue.h"
using namespace std;

#define SIZE_OF_FREQUENCE_TABLE (256)

class Huffman
{
public:
    Huffman();
    ~Huffman();
    void Compress(const char*, const char*);
    void Decompress(const char*, const char*);

private:
    struct Node {
        int freq;
        char ch;
        string encode;
        Node* left;
        Node* right;
        bool leaf;
        Node(char c, int f);
        Node(Node* c1, Node* c2);
        void traverse(string code);
    };
    void calcEncoding();
    void deleteTree(Node*);                            
    void buildTree();                                  
    void getSymbolFrequencies(ifstream&);              
    void readSymbolFrequenceTable(ifstream&);          

    unsigned char totalSymbolCount;                    
    unsigned long totalUncompressedSymbolCount;        
    Node* frequenceTable[SIZE_OF_FREQUENCE_TABLE];     
    Node* rootptr;
   
};

#endif