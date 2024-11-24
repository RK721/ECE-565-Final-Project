#include "cpu/minor/CVU.hh"

CVU::CVU(size_t size) : maxSize(size) {}

//look for an entry
bool CVU::lookup(uint32_t index, uint32_t &value) {
    auto it = cvuTable.find(index);
    if(it != cvuTable.end()) {
        return true;
    }
    return false;
}

//insert an entry
void CVU::insert(uint32_t index, uint32_t value) {
    //check if the table size is at maxSize -> if it is need to evict something
    evictIfFull();
    cvuTable[index] = {index, value}; // push in
}

//erase an entry
void CVU::invalidate(uint32_t index) {
    auto it = cvuTable.find(index);
    if (it != cvuTable.end()) {
        cvuTable.erase(it);
    }
}

//evict from table
void CVU::evictIfFull() {
    if (cvuTable.size() >= maxSize) {
        cvuTable.erase(cvuTable.begin()); //remove first entry for simplicity 
    }
}