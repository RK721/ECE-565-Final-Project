#ifndef __CVU_HH__
#define __CVU_HH__

#include <unordered_map>
#include "base/named.hh"

class CVU : public Named {
private:
    struct CVUEntry {
       uint32_t index; // index coming from lvpt
       uint32_t value; // value of the constant load
    };

    std::unordered_map<uint32_t, CVUEntry> cvuTable; // the main cvu table
    size_t maxSize; // max cvu size
    void evictIfFull();
public:
    CVU(size_t size);

    //function to check if value is in cvu table;
    bool lookup(uint32_t index, uint32_t &value);
    // insert a new constant value in cvu table;
    void insertValue(uint32_t index, uint32_t value);
    // invalidate or remove entry from table by its address
    void invalidate(uint32_t index);
};
#endif // __CVU_HH__