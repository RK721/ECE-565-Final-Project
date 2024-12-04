#include <cmath>
#include "CVU.hh"
#include "base/trace.hh"
#include "debug/MinorMem.hh"

namespace gem5
{

namespace minor
{

CVUClass::CVUClass(const std::string &aName, int numEntries) :
    Named(aName),
    mIndexBits(static_cast<std::uint8_t>(log2(numEntries)))
{
    this->entries = new cvuEntryStruct[numEntries]();
}

bool CVUClass::AddEntry(std::uint64_t aAddress, std::uint64_t aData)
{
    std::uint64_t lIndexMask = pow(2, mIndexBits) - 1;
    std::uint64_t lIndex = (aAddress >> 2) & lIndexMask;

    bool lOverwroteData = false;

    if (this->entries[lIndex].data != aData)
    {
        lOverwroteData = this->entries[lIndex].valid;
        this->entries[lIndex].data = aData;
        this->entries[lIndex].address = aAddress;
        this->entries[lIndex].valid = true;
    }

    DPRINTF(MinorMem, "CVU Updated Index: 0x%03lX for address: 0x%016lX with value: 0x%016lX\n",
                    lIndex, aAddress, this->entries[lIndex].data);

    return lOverwroteData;
}

bool CVUClass::CheckEntry(std::uint64_t aAddress, std::uint64_t &outData)
{
    std::uint64_t lIndexMask = pow(2, mIndexBits) - 1;
    std::uint64_t lIndex = (aAddress >> 2) & lIndexMask;

    if (this->entries[lIndex].valid && this->entries[lIndex].address == aAddress)
    {
        outData = this->entries[lIndex].data;
        DPRINTF(MinorMem, "CVU Hit Index: 0x%03lX for address: 0x%016lX with value: 0x%016lX\n",
                        lIndex, aAddress, outData);
        return true;
    }

    DPRINTF(MinorMem, "CVU Miss Index: 0x%03lX for address: 0x%016lX\n", lIndex, aAddress);
    return false;
}

void CVUClass::InvalidateEntry(std::uint64_t aAddress)
{
    std::uint64_t lIndexMask = pow(2, mIndexBits) - 1;
    std::uint64_t lIndex = (aAddress >> 2) & lIndexMask;

    if (this->entries[lIndex].valid && this->entries[lIndex].address == aAddress)
    {
        this->entries[lIndex].valid = false;
        DPRINTF(MinorMem, "CVU Invalidate Index: 0x%03lX for address: 0x%016lX\n", lIndex, aAddress);
    }
}

} // namespace minor

} // namespace gem5
