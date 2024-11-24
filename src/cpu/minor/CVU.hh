#ifndef __CVU_HH__
#define __CVU_HH__

#include <unordered_map>
#include "base/named.hh"

class CVU : public Named {
private:
    struct CVUEntry {
       
       uint32_t value;
    };

    std::unordered_map<Addr, CVUEntry> cvuTable; // the main cvu table
    size_t maxSize; // max cvu size
public:
    CVU(size_t size);

    //function to check if value is in cvu table;

    // insert a new constant value in cvu table;

    // invalidate or remove entry from table by its address

};
#endif // __CVU_HH__