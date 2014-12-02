// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include <queue>
#include <vector>
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
        }else if(freqTable.count(PSEUDO_EOF) == 1){
            //Öka dess "förekomst" med 1
            freqTable.at(tempByte) += 1;
        }
        else{
            //Annars skapa en ny nyckel med ett som startvärde
            if(tempByte == -1){
                freqTable.insert(make_pair(PSEUDO_EOF,1));
            }else{
                freqTable.insert(make_pair(tempByte,1));
            }
        }
    }
    return freqTable;
}

class lessThanByFrequency{
    public:
    bool operator()(HuffmanNode* lhs, HuffmanNode* rhs){
        return lhs->count > rhs->count;
    }
};
HuffmanNode* buildEncodingTree(const map<int, int> &freqTable) {
    // TODO: Skriva egen jämförelseoprator
    priority_queue<HuffmanNode*,vector<HuffmanNode*>,lessThanByFrequency> priorityQueue;

    for(auto it = freqTable.begin(); it != freqTable.end(); ++it){
        HuffmanNode* x = new HuffmanNode(it->first,it->second, nullptr, nullptr);
        priorityQueue.push(x);
    }
    while(priorityQueue.size() != 1){
        HuffmanNode* left = priorityQueue.top();
        priorityQueue.pop();
        HuffmanNode* right = priorityQueue.top();
        priorityQueue.pop();

        int sum_of_left_and_right = left->count + right->count;
        HuffmanNode* node = new HuffmanNode(NOT_A_CHAR, sum_of_left_and_right, left, right);

        priorityQueue.push(node);
    }
    return priorityQueue.top();
}
void encodingHelper(HuffmanNode* encodingTree, string code, map<int, string> encodingMap){
    if(encodingTree->one == nullptr && encodingTree->zero == nullptr){
        encodingMap.insert(make_pair(encodingTree->character,code));
    }else{
        code += encodingTree->count;
        enco1dingHelper(encodingTree->zero,code,encodingMap);
        encodingHelper(encodingTree->one,code,encodingMap);
    }
}
map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    // TODO: implement this function
    map<int, string> encodingMap;
    string code = "";
    encodingHelper(encodingTree, code, encodingMap);
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
