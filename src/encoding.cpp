// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include <queue>
#include <vector>
#include <strlib.h>
// TODO: include any other headers you need

map<int, int> buildFrequencyTable(istream& input) {
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
    //Prioritetskö som sorterar efter frekvens, lägst hamnar först
    priority_queue<HuffmanNode*,vector<HuffmanNode*>,lessThanByFrequency> priorityQueue;
    //Skapa en ny HuffmanNode för varje element i frekvenslistan
    for(auto it = freqTable.begin(); it != freqTable.end(); ++it){
        HuffmanNode* x = new HuffmanNode(it->first,it->second, nullptr, nullptr);
        priorityQueue.push(x);
    }

    while(priorityQueue.size() != 1){
        //Ta ut de två lägsta elementen i priorityQueue
        HuffmanNode* left = priorityQueue.top();
        priorityQueue.pop();
        HuffmanNode* right = priorityQueue.top();
        priorityQueue.pop();
        //Summera deras frekvenser och skapa en ny HuffmanNode med de båda som vänster/höger subträd
        int sum_of_left_and_right = left->count + right->count;
        HuffmanNode* node = new HuffmanNode(NOT_A_CHAR, sum_of_left_and_right, left, right);

        priorityQueue.push(node);
    }
    return priorityQueue.top();
}
void encodingHelper(HuffmanNode* encodingTree, string code, map<int, string>& encodingMap){
    //Noden är ett löv, lägg till den i encoding map
    if(encodingTree->isLeaf()){
        encodingMap.insert(make_pair(encodingTree->character,code));
    }else{
        //Annars lägg till 0 till kodningen och traversera vänster
        code += "0";
        encodingHelper(encodingTree->zero,code,encodingMap);
        //Ta bort förra nodens tecken, lägg till 1 och traversera höger
        code = code.substr(0, code.size()-1);
        code += "1";
        encodingHelper(encodingTree->one,code,encodingMap);
    }
}

map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {

    map<int, string> encodingMap;
    string code = "";
    encodingHelper(encodingTree, code, encodingMap);
    return encodingMap;

}

void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output) {
    while(input){
        int tempByte = input.get();
        string tempCode;
        if(tempByte == -1){
            tempCode = encodingMap.at(PSEUDO_EOF);
        }else{
            tempCode = encodingMap.at(tempByte);
        }

        while(tempCode.size() > 0){
            int tempBit = stringToInteger(string(1,tempCode.front()));
            output.writeBit(tempBit);
            tempCode.erase(tempCode.begin());
        }
    }
}
void decodeDataHelper(ibitstream& input, HuffmanNode* encodingTree, ostream& output){
    if(encodingTree->isLeaf()){
        output.put(encodingTree->character);
    }else{
        int tempBit = input.readBit();
        if(tempBit == 0){
            decodeDataHelper(input, encodingTree->zero, output);
        }else if(tempBit == 1){
            decodeDataHelper(input, encodingTree->one, output);
        }
    }
}
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    while(input){
        decodeDataHelper(input, encodingTree, output);
    }
}
void compress(istream& input, obitstream& output) {
    // TODO: implement this function
    map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* priorityTree = buildEncodingTree(freqTable);
    map<int, string> encodingMap = buildEncodingMap(priorityTree);

    output.put('{');
    for(map<int, int>::iterator it = freqTable.begin(); it != freqTable.end(); ++it){
        int first = it->first;
        int second = it->second;
        string firstString = integerToString(first);
        while(firstString.size() > 0){
            char tempFirst = firstString.front();
            output.put(tempFirst);
            firstString.erase(firstString.begin());
        }
        output.put(':');
        string secondString = integerToString(second);
        while(secondString.size() > 0){
            char tempSecond = secondString.front();
            output.put(tempSecond);
            secondString.erase(secondString.begin());
        }
        //Iterationen innan slutet för utskrift av kommatecken
        if(it != --freqTable.end()){
            output.put(',');
            output.put(' ');
        }
    }
    output.put('}');
    input.clear();
    input.seekg(0, ios::beg);
    encodeData(input, encodingMap, output);
}

void decompress(ibitstream& input, ostream& output) {
    // TODO: implement this function
    while(true){
        int tempByte = 0;
        char tempChar = '';
        for(int i = 7; i > 0; --i){
            tempByte += (2**i)*input.readBit();
        }
        tempChar = tempByte;
        if(tempChar == '}'){
            break;
        }
    }

}

void freeTree(HuffmanNode* node) {
    // TODO: implement this function
}
