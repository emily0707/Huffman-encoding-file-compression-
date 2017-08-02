#include "Huffman.h"
#include <climits>
#include <vector>


Huffman::Node::Node(char c, int f)
{
    ch = c;
    freq = f;
    left = nullptr;
    right = nullptr;
    leaf = true;
    encode = "";
}

Huffman::Node::Node(Node* c1, Node* c2)
{
    ch = ' ';
    long ifreq = c1->freq + c2->freq;
    if (ifreq > INT_MAX)
    {
        throw invalid_argument("freq is overflow! it is an unsigned int. should be less than 2147483647.");
    }

    freq = ifreq;
    left = c1;
    right = c2;
    leaf = false;
    encode = "";
}

void Huffman::Node::traverse(string code)
{
    if (!leaf) {
        left->traverse(code + '0');
        right->traverse(code + '1');
    }
    else {
        this->encode = string(code);
    }
}

Huffman::Huffman()
{
    totalSymbolCount = 0;
    totalUncompressedSymbolCount = 0;
    this->rootptr = nullptr;
    for (int i = 0; i < SIZE_OF_FREQUENCE_TABLE; ++i)
    {
        this->frequenceTable[i] = nullptr;
    }
}

Huffman::~Huffman()
{
    deleteTree(this->rootptr);
}

void Huffman::getSymbolFrequencies(ifstream& in)
{
    int c;
    unsigned int total_count = 0;
    while ((c = in.get()) != EOF)
    {
        if (c < 0 || c > 255)
            throw logic_error("Only ascii char is supported. Non-Ascii char is detected!");

        unsigned char uc = c;
        if (this->frequenceTable[uc] == nullptr)
        {
            this->frequenceTable[uc] = new Node(uc, 1);
            ++totalSymbolCount;
        }
        else
        {
            if (this->frequenceTable[uc]->freq < INT_MAX)
            {
                ++ this->frequenceTable[uc]->freq;
            }
            else
            {
                throw logic_error("freq is overflow! it is an unsigned int. should be less than 2147483647.");
            }
        }

        if (totalUncompressedSymbolCount > ULONG_MAX)
        {
            throw logic_error("totalUncompressedSymbolCount is overflow! it is an unsigned int. should be less than 0xffffffffUL.");
        }

        ++totalUncompressedSymbolCount;
    }
}

void Huffman::buildTree()
{
    PriorityQueue<Node*> q(SIZE_OF_FREQUENCE_TABLE);
    for (int i = 0; i < SIZE_OF_FREQUENCE_TABLE; ++i)
    {
        if (this->frequenceTable[i] != nullptr)
        {
            q.Enqueue(this->frequenceTable[i]->freq, this->frequenceTable[i]);
        }
    }

    while (q.getNumElements() > 1)
    {
        auto left = q.Dequeue();
        auto right = q.Dequeue();
        Node* newNode = new Node(left->getData(), right->getData());
        q.Enqueue(newNode->freq, newNode);
        delete left;
        delete right;
    }

    auto top = q.Dequeue();
    this->rootptr = top->getData();
    this->calcEncoding();
    delete top;
}

void Huffman::deleteTree(Node* root)
{
    if (root == nullptr)
        return;

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

void Huffman::calcEncoding()
{
    this->rootptr->traverse("");
}

void Huffman::Compress(const char* inputFilename, const char* outputFilename)
{
    ifstream infile(inputFilename, std::ios_base::binary);
    if (infile.fail())
    {
        throw invalid_argument("invalid input file.");
    }

    this->getSymbolFrequencies(infile);
    //cout << "getSymbolFrequencies completed." << endl;
    this->buildTree();
    //cout << "buildTree completed." << endl;
    std::ofstream outfile(outputFilename, std::ios_base::binary);
    if (outfile.fail())
    {
        throw invalid_argument("invalid ouput file.");
    }
   // cout << "totalSymbolCount:" << (int)totalSymbolCount << " sizeof(unsigned char): " << sizeof(unsigned char) << endl;
    outfile.write(reinterpret_cast<char*>(&(this->totalSymbolCount)), sizeof(unsigned char));
    //cout << "write totalSymbolCount completed: " << this->totalSymbolCount << endl;
    vector<Huffman::Node*> sortedFreq;
    outfile.flush();
    for (int i = 0; i < SIZE_OF_FREQUENCE_TABLE; ++i)
    {
        if (this->frequenceTable[i] != nullptr)
        {
            outfile.write(&this->frequenceTable[i]->ch, sizeof(char));
            int feq = this->frequenceTable[i]->freq;
            outfile.write(reinterpret_cast<char*>(&(feq)), sizeof(int));
            sortedFreq.push_back(this->frequenceTable[i]);
            //cout << "CH:" << this->frequenceTable[i]->ch << "(" << (int)this->frequenceTable[i]->ch << ") FEQ:" << this->frequenceTable[i]->freq << " Code:" << this->frequenceTable[i]->encode << endl;
        }
    }
    outfile.flush();
     unsigned int uitotalUncompressedSymbolCount = totalUncompressedSymbolCount;
    //cout << "write frequenceTable completed." << endl;
    outfile.write(reinterpret_cast<char*>(&(uitotalUncompressedSymbolCount)), sizeof(unsigned int));
    //cout << "write totalUncompressedSymbolCount :" << uitotalUncompressedSymbolCount << endl;
    outfile.flush();
    infile.clear();
    infile.seekg(0, ios::beg);
    int c;
    unsigned char curbit = 0;
    unsigned char curbyte = 0;
  
    while ((c = infile.get()) != EOF)
    {
        unsigned char uc = c;
        string encode = this->frequenceTable[uc]->encode;

        for (size_t i = 0; i < encode.size(); ++i)
        {
            curbyte |= ((encode[i] - '0') << curbit);
            if (++curbit == 8)
            {
                outfile.write(reinterpret_cast<char*>(&curbyte), sizeof(char));
               
                curbit = 0;
                curbyte = 0;
            }
        }
        uitotalUncompressedSymbolCount--;
    }
    if (uitotalUncompressedSymbolCount != 0)
    {
        throw logic_error("number of char readed from infile is less than expected!");
    }
    if (curbit > 0)
    {
        outfile.write(reinterpret_cast<char*>(&curbyte), sizeof(char));
    }
    outfile.flush();
}


void Huffman::readSymbolFrequenceTable(ifstream& in)
{
    unsigned char symbolcount = 0;
    in.read(reinterpret_cast<char*>(&symbolcount), sizeof(unsigned char));
    for (int i = 0; i < symbolcount; ++i)
    {
        unsigned char ch = 0;
        int freq = 0;
        in.read(reinterpret_cast<char*>(&ch), sizeof(unsigned char));
        in.read(reinterpret_cast<char*>(&freq), sizeof(int));
        this->frequenceTable[ch] = new Node(ch, freq);
    }
    this->totalSymbolCount = symbolcount;
}

void Huffman::Decompress(const char* inputFilename, const char* outputFilename)
{
    ifstream infile(inputFilename, std::ios_base::binary);
    if (infile.fail())
    {
        throw invalid_argument("invalid input file.");
    }
    readSymbolFrequenceTable(infile);
    unsigned int uitotalUncompressedSymbolCount = 0;
    infile.read(reinterpret_cast<char*>(&(uitotalUncompressedSymbolCount)), sizeof(unsigned int));
    totalUncompressedSymbolCount = uitotalUncompressedSymbolCount;
    this->buildTree();
    std::ofstream outfile(outputFilename, std::ios_base::binary);
    if (outfile.fail())
    {
        throw invalid_argument("invalid ouput file.");
    }
    int c;
    unsigned char curbit = 0;
    unsigned char curbyte = 0;
    Node* p = this->rootptr;
    bool iseof = infile.eof();
    while (uitotalUncompressedSymbolCount > 0 && !infile.eof())
    {
        c = infile.get();
        unsigned char byte = (unsigned char)c;
        unsigned char mask = 1;

        while (uitotalUncompressedSymbolCount > 0 && mask)
        {
            p = byte & mask ? p->right : p->left;
            mask <<= 1;

            if (p->leaf == true)
            {
                outfile.put(p->ch);
                p = this->rootptr;
                --uitotalUncompressedSymbolCount;
            }
        }
    }
    if (uitotalUncompressedSymbolCount > 0)
    {
        throw logic_error("number of uncompressed symbol readed from file is less than expected! file cold be currupted.");
    }
}
