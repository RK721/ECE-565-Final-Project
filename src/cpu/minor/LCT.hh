#ifndef __CPU_MINOR_LCT_HH__
#define __CPU_MINOR_LCT_HH__

#include "base/named.hh"

namespace gem5
{

namespace minor
{


class LCTClass : public Named
{
    public:
        LCTClass(const std::string &aName, int numEntries, int aCounterSize);
        
        enum counterState {
            NOPRED1,
            NOPRED2,
            PRED,
            CONSTANT
        };

        bool AdjustPrediction(std::uint64_t aAddress, bool predCorrect);
        bool GetPrediction(std::uint64_t aAddress, bool& aIsConstant);
        std::string stateToString(counterState state);

 
    private:
        struct lctEntryStruct
        {
            counterState state = LCTClass::NOPRED1;
            bool valid = false;
        };
    
        lctEntryStruct* entries;
        std::uint8_t mIndexBits;
        int counterSize;

};

} // namespace minor
} // namespace gem5

#endif /* __CPU_MINOR_LCT_HH__ */