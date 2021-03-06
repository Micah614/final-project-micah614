#include <iostream>
#include "../code/DataPacket.h"
#include "../code/MaxHeapHashMap.h"

#include <iostream>
#include <array>
#include <vector>
#include <set>
#include <queue>
#include <memory>
#include <string>
#include <map>
#include <stack>
#include <random>
#include <algorithm>

using namespace std;


int main(){



// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ STAGE ONE: CREATE DATAPACKET OBJECTS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    std::array<int,50> packetPriority;  // ARRAY CONTAINING PACKET PRIORITIES
    std::array<string,50> packetData;  // ARRAY CONTAINING PACKET DATA
    // packetDomainID ARRAY DOMAIN_ID FREQUENCIES: [8, 12, 9, 7, 14] (FOR DOMAIN 1,2,3... ETC.)
    std::array<int,50> packetDomainID = {1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};

// LOAD EACH ARRAY WITH DATA
for(int i=0; i<packetPriority.size(); ++i) {  // NUMBERED 0-50
    packetPriority[i] = i+1;
}
for(int i=0; i<packetData.size(); ++i) {  // NUMBERED 0-50
    packetData[i] = to_string(i+1);  // CONTAINS A STRING VALUE OF THE PRIORITY
}





// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ STAGE TWO:LOAD SHARED POINTERS INTO STACKS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// CREATE AN IN-ORDER DATAPACKET STACK CALLED "inOrderStack"
std::stack<shared_ptr<DataPacket>> inOrderStack;  // STACK TO HOLD IN-ORDER FEED
for(int a=packetData.size()-1; a>-1; a--) {  // PUSH TO STACK IN REVERSE ORDER
    DataPacket curNode;
    shared_ptr<DataPacket> node = curNode.InitDataPacket(packetData[a], packetPriority[a], packetDomainID[a]);  // INITDATAPACKET
    inOrderStack.push(node);
}
// VERIFY THE IN-ORDER DATA FEED
std::stack<shared_ptr<DataPacket>> tempStack = inOrderStack;  // MAKE A COPY OF THE STACK TO VERIFY ITS' CONTENTS
cout << endl << "NOTE: In-order and shuffled object feeds contain the same DataPacket objects, arranged in a different order. " << endl << endl << "Verifying in-order feed: ";
while(!tempStack.empty()) {
    shared_ptr<DataPacket> curPacket;
    curPacket = tempStack.top();
    tempStack.pop();
    cout << (*curPacket).GetPacketData() << " ";  // GETPACKETDATA
}
cout << endl << "  In-order stack size: " << inOrderStack.size() << endl;



// CREATE A SHUFFLED DATA FEED 
std::vector<shared_ptr<DataPacket>> shuffleVec;
std::stack<shared_ptr<DataPacket>> tempStack2 = inOrderStack;  // COPY inOrderStack TO tempStack2
// CREATE A VECTOR WITH THE DATAPACKET OBJECTS
while(!tempStack2.empty()) {
    shared_ptr<DataPacket> curPacket;
    curPacket = tempStack2.top();
    tempStack2.pop();
    shuffleVec.push_back(curPacket);
}
std::random_shuffle(shuffleVec.begin(), shuffleVec.end());  // SHUFFLE THE VECTOR
// PUSH THE SHUFFLED OBJECTS INTO A STACK
std::stack<shared_ptr<DataPacket>> shuffledStack;  // DECLARE A STACK
for(int c=0; c<shuffleVec.size(); ++c) {
    shared_ptr<DataPacket> curPacket;
    curPacket = shuffleVec.at(c);
    shuffledStack.push(curPacket);
}
shuffleVec.clear();
// VERIFY THAT THE FEED IS SHUFFLED
cout << endl << "Verifying shuffled feed: ";
std::stack<shared_ptr<DataPacket>> tempStack3 = shuffledStack;  // COPY SHUFFLEDSTACK TO VERIFY ITS' CONTENTS
while(!tempStack3.empty()) {
    cout << (*tempStack3.top()).GetPacketData() << " ";
    tempStack3.pop();
}
cout << endl << "  Shuffled stack size: " << shuffledStack.size() << endl << endl;  // ENUMERATE THE NUMBER OF OBJECTS





// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ STAGE THREE: CREATE "IN-ORDER" AND "SHUFFLED" HASHMAP OBJECTS AND INSERT THE DATAPACKETS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// INITIATE AND LOAD IN-ORDER HASHMAP:
cout << "~~~~~~" << endl << "Creating In-Order MaxHeapHashMap object..." << endl;
HashMap inOrderHashMap;  // DECLARE IN-ORDER HASHMAP OBJECT
inOrderHashMap.initHashMap();  // INITIATE IN-ORDER HASHMAP OBJECT
if(inOrderHashMap.packetCount == 0) {  // VERIFY THAT HASHMAP EXISTS
    cout << "HashMap object created successfully... " << endl;
}
if((*inOrderHashMap.table[1]).domain == "uxui_interface") {  // VERIFY HASHMAP "table" MEMBER EXISTS AND HAS THE DESIRED ATTRIBUTES
    cout << "HashMap table and PQ sub-structures successfully initiated." << endl;
}
cout << "Routing DataPacket objects..." << endl;
while(!inOrderStack.empty()) {  // INSERT THE DATA PACKETS FROM inOrderStack INTO inOrderHashMap USING assignPqDomainAndInsert MEMBER FUNCTION
    shared_ptr<DataPacket> curPacket;
    curPacket = inOrderStack.top();
    inOrderStack.pop();
    inOrderHashMap.assignPqDomainAndInsert(curPacket);  // INSERT THE PACKET INTO inOrderHashMap
}
cout << endl;
cout << inOrderHashMap.packetCount << " sorted DataPackets were successfully routed and inserted in the inOrderHashMap MaxHeapHashMap data structure." << endl;
for(int j=1; j<6; ++j) {  // DESCRIBE THE DISTRIBUTION ACCROSS DOMAINS
    cout << "  " << (*inOrderHashMap.table[j]).nodeCount << " objects in the " << (*inOrderHashMap.table[j]).domain << " domain." << endl;
}
cout << endl;



// INITIATE AND LOAD SHUFFLED HASHMAP OBJECT:
cout << "~~~~~~" << endl << "Creating Shuffled MaxHeapHashMap object..." << endl;
HashMap shuffledHashMap;  // SHUFFLED HASHMAP OBJECT
shuffledHashMap.initHashMap();  // INITIATE SHUFFLED HASHMAP OBJECT
if(shuffledHashMap.packetCount == 0) {  // VERIFY THAT HASHMAP EXISTS
    cout << "HashMap object created successfully... " << endl;
}
if((*shuffledHashMap.table[1]).domain == "uxui_interface") {  // VERIFY HASHMAP "table" MEMBER EXISTS AND HAS THE DESIRED ATTRIBUTES
    cout << "HashMap table and PQ sub-structures successfully initiated." << endl;
}
cout << "Routing DataPacket objects..." << endl;
while(!shuffledStack.empty()) {  // INSERT DATA PACKETS FROM shuffledStack INTO shuffledHashMap USING assignPqDomainAndInsert MEMBER FUNCTION
    shared_ptr<DataPacket> curPacket;
    curPacket = shuffledStack.top();
    shuffledStack.pop();
    shuffledHashMap.assignPqDomainAndInsert(curPacket);  // INSERT THE PACKET INTO shuffledHashMap
}
cout << endl;
cout << shuffledHashMap.packetCount << " shuffled DataPacket objects were successfully routed and stored in the shuffledHashMap MaxHeapHashMap data structure." << endl;
for(int k=1; k<6; ++k) {  // DESCRIBE THE DISTRIBUTION ACCROSS DOMAINS
    cout << "  " << (*shuffledHashMap.table[k]).nodeCount << " objects in the " << (*shuffledHashMap.table[k]).domain << " domain." << endl;
}
cout << endl;





// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ STAGE FOUR: EMPTY THE MAXHEAPS AND POPULATE THE ARRAYS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

cout << "~~~~~~" << endl;
// READ THE pqMaxHeapArray FOR shuffledHashMap
cout << "The contents of the UXUI_INTERFACE domain pqMaxHeapArray of shuffledHashMap is: " << endl;
    map<int, shared_ptr<MaxHeapPQ>>::iterator it;  // ITERATE THROUGH "table"
    for(it=shuffledHashMap.table.begin(); it!=shuffledHashMap.table.end(); it++) {
        if(it->first == 1) {
            for (int i=0; i < 8; ++i) {
                shared_ptr<DataPacket> thing = (*it->second).pqMaxHeapArray[i];
                cout << (*thing).GetPacketData() <<" ";
            }
            //assignedPQ = it->second;  // COLLECT POINTER TO ASSIGNED DOMAIN PQ
        }
    }
cout << endl << endl;

// READ THE pqMaxHeapArray FOR inOrderHashMap
cout << "The contents of the UXUI_INTERFACE domain pqMaxHeapArray of inOrderHashMap is: " << endl;
    map<int, shared_ptr<MaxHeapPQ>>::iterator it1;  // ITERATE THROUGH "table"
    for(it1=inOrderHashMap.table.begin(); it1!=inOrderHashMap.table.end(); it1++) {
        if(it1->first == 1) {
            for (int i=0; i < 8; ++i) {
                shared_ptr<DataPacket> thing = (*it1->second).pqMaxHeapArray[i];
                cout << (*thing).GetPacketData() << " ";
            }
            //assignedPQ = it->second;  // COLLECT POINTER TO ASSIGNED DOMAIN PQ
        }
    }
cout << endl;



// // CALL indexPqAndRetrievePacket() ON "inOrderHashMap" AND "shuffledHashMap"; PLACE RETRIEVED POINTERS INTO THE ASSIGNED VECTORS
// cout << "~~~~~~" << endl;
// shared_ptr<DataPacket> checkShuffledPacket;
// cout << "Verifying index and removal operations using the UXUI_INTERFACE domain of shuffledHashMap:" << endl;
// bool done1 = false;
// while(done1 != true) {
//     checkShuffledPacket = shuffledHashMap.indexPqAndRetrievePacket(1);
//     if(checkShuffledPacket != NULL) {
//     cout << (*checkShuffledPacket).GetPacketData() << "  ";
//     }
//     else {
//         done1 = true;
//     }
// }
// cout << endl << endl;


// shared_ptr<DataPacket> checkInOrderPacket;
// cout << "Verifying index and removal operations using the UXUI_INTERFACE domain of inOrderHashMap:" << endl;
// bool done = false;
// while(done != true) {
//     checkInOrderPacket = inOrderHashMap.indexPqAndRetrievePacket(1);
//     if(checkInOrderPacket != NULL) {
//     cout << (*checkInOrderPacket).GetPacketData() << "  ";
//     }
//     else {
//         done = true;
//     }
// }
// cout << endl << endl;





// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ STAGE FIVE: ASSERT OR DENY THE EQVIVALENCE OF EACH ARRAY @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

vector<shared_ptr<DataPacket>> InOrder_Result;  // RESULT VECTOR FOR IN-ORDER DATA
vector<shared_ptr<DataPacket>> Shuffled_Result;  // RESULT VECTOR FOR SHUFFLED DATA

// RETRIEVE THE shuffledHashMap DATAPACKETS FROM THE HASHMAP STRUCTURE
Shuffled_Result = shuffledHashMap.FillDestinationVector();
// PRINT OUT Shuffled_Result VECTOR SIZE AND CONTENTS:
cout << endl << "Size of the Shuffled_Result vector is: ";
cout << Shuffled_Result.size() << endl;

for(int i=0; i < Shuffled_Result.size(); i++){
   cout << (*Shuffled_Result[i]).GetPacketData() << " ";
}
cout << endl;


// RETRIEVE THE inOrderHashMap DATAPACKETS FROM THE HASHMAP STRUCTURE:
InOrder_Result = inOrderHashMap.FillDestinationVector();
// PRINT OUT InOrder_Result VECTOR SIZE AND CONTENTS:
cout << endl << "Size of the InOrder_Result vector is: ";
cout << InOrder_Result.size() << endl;

for(int i=0; i < InOrder_Result.size(); i++){
   cout << (*InOrder_Result[i]).GetPacketData() << " ";
}
cout << endl << endl;


//DELARE EQUIVALENCE/NON-EQUIVALENCE OF THE TWO VECTORS:
cout << "~~~~~~" << endl << "RESULT: ";
if(Shuffled_Result == InOrder_Result) {
    cout << "THE VECTORS ARE EQUIVALENT AND MAXHEAPHASHMAP DATA STRUCTURE EXHIBITS SORTING CHARACTERISTICS!" << endl << endl;
}
else {
    cout << "MAX HEAP PROPERTIES ALONE ARE INSUFFICIENT TO ACHIEVE SORTING, FURTHER DEVELOPMENT IS NECESSARY..." << endl << endl;
}


    return 0;
}