// Lab3.cpp : Defines the entry point for the console application.
//

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include "MinHeap.h"
#include "PriorityQueue.h"
#include "Huffman.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Invalid Parameter" << endl;
        return -1;
    }

    //get the program file path
    std::string aux(argv[0]);

    //file the last '\\'
#if defined(_WIN32) || defined(WIN32)
    int pos = aux.rfind('\\');
#else
    int pos = aux.rfind('/');
#endif
    
    // Get the path and the name
    std::string path = aux.substr(0, pos + 1);

    try
    {
        Huffman h1;
        string outputfile = argv[1];

#if defined(_WIN32) || defined(WIN32)
        if (outputfile.rfind('\\') < 0)
        {
            outputfile = path + outputfile;
        }
#else
        if (outputfile.rfind('/') < 0)
        {
            outputfile = path + outputfile;
        }
#endif

        outputfile += ".huf";

        h1.Compress(argv[1], outputfile.c_str());

    }
    catch (exception e)
    {
        cout << e.what() << endl;
        return -1;
    }

    return 0;
}

