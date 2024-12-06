#include "base/trace.hh"
#include "cpu/minor/CVU.hh"
#include "debug/LvpDebug.hh"
namespace gem5
{

namespace minor
{

CVUClass::CVUClass(const std::string &aName)
    : Named(aName)
{
}

CVUClass::~CVUClass()
{
}

bool CVUClass::AddEntry(std::uint64_t aAddress, std::uint64_t aData)
{
    entries[aAddress] = aData;

    DPRINTF(LvpDebug, "CVU AddEntry: Address=0x%016lX, Data=0x%016lX\n",
            aAddress, aData);

    return true;
}

bool CVUClass::CheckEntry(std::uint64_t aAddress, std::uint64_t predictedValue)
{
    auto it = entries.find(aAddress);

    if (it != entries.end()) {
        std::uint64_t actualValue = it->second;

        DPRINTF(LvpDebug, "CVU CheckEntry: Address=0x%016lX, PredictedValue=0x%016lX, ActualValue=0x%016lX\n",
                aAddress, predictedValue, actualValue);

        // Return true if the prediction matches the actual value
        return predictedValue == actualValue;
    }

    DPRINTF(LvpDebug, "CVU CheckEntry miss: Address=0x%016lX\n", aAddress);

    // Return false if no entry exists
    return false;
}

void CVUClass::RemoveEntry(std::uint64_t aAddress)
{
    size_t erased = entries.erase(aAddress);

    if (erased > 0) {
        DPRINTF(LvpDebug, "CVU InvalidateEntry: Address=0x%016lX\n", aAddress);
    }
}

} // namespace minor

} // namespace gem5
