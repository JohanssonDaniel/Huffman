// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include <queue>
#include <vector>
#include <strlib.h>
#include <cmath>                            /**pow**/

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
        //Ta ut första byten
        int tempByte = input.get();
        string tempCode;
        //Hämta ut bitsekvensen som hör till bokstaven
        if(tempByte == -1){
            tempCode = encodingMap.at(PSEUDO_EOF);
        }else{
            tempCode = encodingMap.at(tempByte);
        }

        while(tempCode.size() > 0){
            //Ta ut första biten från sekvensen
            int tempBit = stringToInteger(string(1,tempCode.front()));
            //Skriv ut den till output
            output.writeBit(tempBit);
            //Ta bort den från sekvensen
            tempCode.erase(tempCode.begin());
        }
    }
}
void decodeDataHelper(ibitstream& input, HuffmanNode* encodingTree, ostream& output){
    //Noden är ett löv, skriv ut värdet
    if(encodingTree->isLeaf()){
        int character = encodingTree->character;
        if(character != PSEUDO_EOF){    //Om vi inte nått slutet
            output.put(encodingTree->character);
        }else{
            input.get();                //Ta bort sista tecknet
        }
    }else{
        //Läs in första biten
        int tempBit = input.readBit();
        //Traversera vänster/höger träd beroende på biten
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

void addBiggerThanByte(map<int, int> freqTable, obitstream& output){
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
}
//Komprimerar headern genom att se om det finns tal i frekvenstabellen som är större än en byte (255)
//Om så är fallet läggs den in i ouputen på samma sätt med en 1 först i filen för att markera det
//Annars sätts 0 först i filen samt storleken på frekvenstabellen, alla karaktärer och frekvenser
//Skrivs till ouput med .put()
//Exempel för example.txt 08322973983991 EOF lägg inte in eftersom den är större än en byte
void compress(istream& input, obitstream& output) {

    map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* priorityTree = buildEncodingTree(freqTable);
    map<int, string> encodingMap = buildEncodingMap(priorityTree);
    bool containsBiggerThanByte = false;
    vector<char> vectorForFreqTable;

    for(map<int,int>::iterator it = freqTable.begin(); it != freqTable.end(); ++it){
        if(it->first == PSEUDO_EOF){        //Lägg inte till PSEUDO_EOF än
            break;
        }
        else if(!(it->first > PSEUDO_EOF) && !(it->second > PSEUDO_EOF)){   //Om frekvensen och karaktären tar mindre än en byte
            vectorForFreqTable.push_back(it->first);
            vectorForFreqTable.push_back(it->second);
        }else{
            containsBiggerThanByte = true;
            break;
        }
    }
    if(containsBiggerThanByte){
        output.put('1');     //Tal större än en byte finns
        addBiggerThanByte(freqTable,output);

    }else{
        output.put('0');    //Inget tal större än en byte
        int freqTableSize = vectorForFreqTable.size();
        output.put(freqTableSize);  //tabellens storlek kommer användas för inmatningen
        for(int i = 0; i < freqTableSize; ++i){
            int tempNum = vectorForFreqTable.at(i);
            output.put(tempNum);
        }
    }
    input.clear();
    input.seekg(0, ios::beg);
    encodeData(input, encodingMap, output);
    freeTree(priorityTree);
}

char buildCharFromByte(ibitstream& input) {
    int tempByte = 0;
    char tempChar;
    for(double i = 0; i <= 7; ++i){
        tempByte += pow(2.0, i)*input.readBit();
    }
    tempChar = tempByte;
    return tempChar;
}

void decompressHelper(ibitstream& input, map<int, int>& freqTable, string value, string freq, bool switched){
    //Ta ut första byten och gör den till en char
    char tempChar = buildCharFromByte(input);
    //Om slut av par lägg in i freqTable och första rekursera
    if(tempChar == ','){
        int intValue = stringToInteger(value);
        int intFreq = stringToInteger(freq);
        freqTable.insert(make_pair(intValue,intFreq));
        decompressHelper(input, freqTable, value, freq, switched);
    }else if(tempChar == ':'){  //Om semikolon, byt till att bygga frekvensen
        switched = true;
        decompressHelper(input, freqTable, value, freq, switched);
    }else if (tempChar == ' '){ //Reseta värden
        value = "";
        freq = "";
        switched = false;
        decompressHelper(input, freqTable, value, freq, switched);
    }else if(tempChar == '{'){  //Om början av tabell, gör inget
        decompressHelper(input, freqTable, value, freq, switched);
    }else if(tempChar == '}'){  //Om slutet av tabell, lägg in sista värdet och avsluta loop
        int intValue = stringToInteger(value);
        int intFreq = stringToInteger(freq);
        freqTable.insert(make_pair(intValue,intFreq));
        return;
    }
    else{
        if(switched){           //Om efter semikolon, öka frekvensen
            freq += tempChar;
            decompressHelper(input, freqTable, value, freq, switched);
        }else{                  //Annars öka värdet
            value += tempChar;
            decompressHelper(input, freqTable, value, freq, switched);
        }
    }
}
//Om det är 1 i början av filen packas filen upp på samma sätt som förut
//Annars läses storleken på frekvenstbellen in först och sen byte för byte värden som sedan sätts in i frekvenstabellen
void decompress(ibitstream& input, ostream& output) {
    map<int, int> freqTable;
    int testForSize = input.get();
    if(testForSize == '1'){
        bool switchSide = false;
        string value = "";
        string freq = "";
        decompressHelper(input,freqTable,value,freq,switchSide);
    }else{
        int inputSize = input.get();
        for(int i = 0; i < inputSize; ++i){
            char tempByte = input.get();
            int tempValue = tempByte;
            int tempFreq = input.get();
            --inputSize;    //Eftersom både frekvensen samt karaktären tagits ut minska inputSize
            freqTable.insert(make_pair(tempValue,tempFreq));
        }
        //Eftersom "endoffile" karaktären inte lades till om vi inte hade tal större än en byte läggs den till här
        freqTable.insert(make_pair(PSEUDO_EOF,1));
    }
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    decodeData(input,encodingTree,output);
    freeTree(encodingTree);
}

void freeTree(HuffmanNode* node) {
    if(node != nullptr){
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}
