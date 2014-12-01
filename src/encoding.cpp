// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include <queue>
// TODO: include any other headers you need

map<int, int> buildFrequencyTable(istream& input) {
    // TODO: implement this function
    map<int, int> freqTable;
    //Medan input har data
    while(input){
        //Spara undan första byten
        int tempByte = input.get();
        //Om tecknet redan finns i freqTable
        if (freqTable.count(tempByte) == 1){
            //Öka dess "förekomst" med 1
            freqTable.at(tempByte) += 1;
        }
        else{
            //Annars skapa en ny nyckel med ett som startvärde
            freqTable.insert(make_pair(tempByte,1));
        }
    }
    return freqTable;
}
HuffmanNode* buildEncodingTree(const map<int, int> &freqTable) {
    // TODO: implement this function
    priority_queue<HuffmanNode> priorityQueue;

    for(auto it = freqTable.begin(); it != freqTable.end(); ++it){
        HuffmanNode x = HuffmanNode(it->first,it->second);
        priorityQueue.push(x);
    }
    return nullptr;
}

map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    // TODO: implement this function
    map<int, string> encodingMap;
    return encodingMap;
}

void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output) {
    // TODO: implement this function
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    // TODO: implement this function
}

void compress(istream& input, obitstream& output) {
    // TODO: implement this function
}

void decompress(ibitstream& input, ostream& output) {
    // TODO: implement this function
}

void freeTree(HuffmanNode* node) {
    // TODO: implement this function
}
