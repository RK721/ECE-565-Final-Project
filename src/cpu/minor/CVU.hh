#ifndef CVU_HH
#define CVU_HH

#include <cstdint>
#include <string>
#include <vector>
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
    ~CVUClass();

    // Add an entry to the CVU
    bool AddEntry(std::uint64_t aAddress, std::uint64_t aData);

    // Check if an entry exists for the given address
    bool CheckEntry(std::uint64_t aAddress, std::uint64_t &outData) const;

    // Invalidate an entry for the given address
    void InvalidateEntry(std::uint64_t aAddress);

  private:
    std::vector<cvuEntryStruct> entries; // Array of CVU entries
    std::uint64_t indexMask;             // Mask for indexing
};

} // namespace minor

} // namespace gem5

#endif // CVU_HH
