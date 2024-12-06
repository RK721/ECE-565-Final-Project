#include "cpu/minor/CVU.hh"
#include <cmath>

namespace gem5
{

namespace minor
{

CVUClass::CVUClass(const std::string &aName, int numEntries)
    : Named(aName),
      entries(numEntries),          // Initialize entries array with size
      indexMask(numEntries - 1)     // Mask for direct-mapped indexing
{
    // Ensure all entries are invalid at the start
    for (auto &entry : entries) {
        entry.valid = false;
    }
}

CVUClass::~CVUClass()
{
    // No dynamic memory to clean up, vector handles itself
}

bool CVUClass::AddEntry(std::uint64_t aAddress, std::uint64_t aData)
{
    std::uint64_t index = aAddress & indexMask;

    if (entries[index].valid && entries[index].address != aAddress) {
        DPRINTF(MinorMem, "CVU conflict: Overwriting entry at index %lu\n", index);
    }

    entries[index].address = aAddress;
    entries[index].data = aData;
    entries[index].valid = true;

    DPRINTF(MinorMem, "CVU AddEntry: Address=0x%016lX, Data=0x%016lX, Index=%lu\n",
            aAddress, aData, index);

    return true;
}

bool CVUClass::CheckEntry(std::uint64_t aAddress, std::uint64_t &outData) const
{
    std::uint64_t index = aAddress & indexMask;

    if (entries[index].valid && entries[index].address == aAddress) {
        outData = entries[index].data;
        DPRINTF(MinorMem, "CVU CheckEntry hit: Address=0x%016lX, Data=0x%016lX, Index=%lu\n",
                aAddress, outData, index);
        return true;
    }

    DPRINTF(MinorMem, "CVU CheckEntry miss: Address=0x%016lX, Index=%lu\n", aAddress, index);
    return false;
}

void CVUClass::InvalidateEntry(std::uint64_t aAddress)
{
    std::uint64_t index = aAddress & indexMask;

    if (entries[index].valid && entries[index].address == aAddress) {
        entries[index].valid = false;
        DPRINTF(MinorMem, "CVU InvalidateEntry: Address=0x%016lX, Index=%lu\n",
                aAddress, index);
    }
}

} // namespace minor

} // namespace gem5
