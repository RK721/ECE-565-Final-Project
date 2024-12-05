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
        LCTClass(const std::string &aName, int numEntries, int counterSize);
        
        enum counterState {
            NOPRED1,
            NOPRED2,
            PRED,
            CONSTANT
        };

        bool adjustPrediction(std::uint64_t aAddress, bool predCorrect);
        bool GetPrediction(std::uint64_t aAddress, bool& aIsConstant);
        std::string stateToString(counterState state);

 
    private:
        struct lctEntryStruct
        {
            int counterSize;
            counterState state;
            bool valid;
        };
    

        lctEntryStruct* entries;
        std::uint8_t mIndexBits;

};

} // namespace minor
} // namespace gem5

#endif /* __CPU_MINOR_LCT_HH__ */