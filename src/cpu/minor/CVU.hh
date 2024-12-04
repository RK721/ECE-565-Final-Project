#ifndef CVU_HH
#define CVU_HH

#include <cstdint>
#include <string>
#include "base/trace.hh"

namespace gem5
{

namespace minor
{

// Entry structure for CVU
struct cvuEntryStruct {
    std::uint64_t address;  // Memory address
    std::uint64_t data;     // Cached constant value
    bool valid;             // Validity flag
};

class CVUClass : public Named
{
  public:
    CVUClass(const std::string &aName, int numEntries);

    // Add an entry to the CVU
    bool AddEntry(std::uint64_t aAddress, std::uint64_t aData);

    // Check if an entry exists for the given address
    bool CheckEntry(std::uint64_t aAddress, std::uint64_t &outData);

    // Invalidate an entry for the given address
    void InvalidateEntry(std::uint64_t aAddress);

  private:
    cvuEntryStruct *entries;          // Array of CVU entries
    std::uint8_t mIndexBits;          // Number of index bits
};

} // namespace minor

} // namespace gem5

#endif // CVU_HH
