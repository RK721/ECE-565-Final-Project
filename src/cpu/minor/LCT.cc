#include <cmath>

#include "cpu/minor/LCT.hh"

#include "base/trace.hh"
#include "debug/LvpDebug.hh"


namespace gem5
{

GEM5_DEPRECATED_NAMESPACE(Minor, minor);
namespace minor
{
// needs interfacing with CPU for addr and the CVU
LCTClass::LCTClass(const std::string &aName, int numEntries, int aCounterSize) :
    Named(aName), 
    mIndexBits(static_cast<std::uint8_t>(log2(numEntries))), // countsize will be 1 or 2 for now
    counterSize(aCounterSize)
{
    this->entries = new lctEntryStruct[numEntries];
}

bool LCTClass::GetPrediction(std::uint64_t aAddress, bool& aIsConstant) {
    std::uint64_t lIndexMask = pow(2, mIndexBits) - 1;
    std::uint64_t lIndex = (aAddress >> 2) & lIndexMask;

    aIsConstant = (this->entries[lIndex].valid) && (CONSTANT == this->entries[lIndex].state);
    
    return (this->entries[lIndex].valid) && ((CONSTANT == this->entries[lIndex].state) || (PRED == this->entries[lIndex].state));
}

bool LCTClass::AdjustPrediction(std::uint64_t aAddress, bool predCorrect, bool& aDowngradedFromConstant) {
    std::uint64_t lIndexMask = pow(2, mIndexBits) - 1;
    std::uint64_t lIndex = (aAddress >> 2) & lIndexMask;

    LCTClass::counterState nextPred = counterState::NOPRED1;
    aDowngradedFromConstant = false;

    this->entries[lIndex].valid = true; // this address has been seen

    bool constantPred = false;
    
    if (this->counterSize == 1) {
        if (predCorrect) { // correct prediction
            switch(this->entries[lIndex].state) {

                case NOPRED1:   
                {
                    nextPred = CONSTANT;
                    constantPred = true;
                }
                break;

                case CONSTANT:  nextPred = CONSTANT;
                    break;
                default:        nextPred = this->entries[lIndex].state;
                    break;
            }

        } else { // incorrect prediction
            switch(this->entries[lIndex].state) {
                case NOPRED1:   nextPred = NOPRED1; 
                    break;
                case CONSTANT:
                {
                    nextPred = NOPRED1;
                    aDowngradedFromConstant = true;
                }
                break;

                default:        nextPred = this->entries[lIndex].state;
                    break;
            }
        }
    } else if (this->counterSize == 2) {
        if (predCorrect) {
            switch(this->entries[lIndex].state) {
                case NOPRED1:   nextPred = NOPRED2; 
                    break;
                case NOPRED2:   nextPred = PRED;
                    break;
                case PRED:
                {
                    nextPred = CONSTANT;
                    constantPred = true;
                }      
                break;
                
                case CONSTANT:  nextPred = CONSTANT;
                    break;
                default:        nextPred = this->entries[lIndex].state;
                    break;
            }

        } else {
            switch(this->entries[lIndex].state) {
                case NOPRED1:   nextPred = NOPRED1; 
                    break;
                case NOPRED2:   nextPred = NOPRED1;
                    break;
                case PRED:      nextPred = NOPRED2;
                    break;

                case CONSTANT:
                {
                    nextPred = PRED;
                    aDowngradedFromConstant = true;
                }  
                break;

                default:        nextPred = this->entries[lIndex].state;
                    break;
            }

        }
    }

    DPRINTF(LvpDebug, "LCT Updated Prediction for Index: 0x%03lX for address: 0x%016lX: %s converted to %s correct:%u\n",
                    lIndex, aAddress, stateToString(this->entries[lIndex].state).c_str(), stateToString(nextPred).c_str(), predCorrect);
    
    if (constantPred) {
        DPRINTF(LvpDebug, "LCT Upgraded to Constant for Index: 0x%03lX for address: 0x%016lX\n",
                    lIndex, aAddress);
    }

    this->entries[lIndex].state = nextPred;

    return constantPred;
}

std::string LCTClass::stateToString(counterState state) {
    std::string name;

    switch(state) {
        case NOPRED1:   name = "NOPRED1";
            break;
        case NOPRED2:   name = "NOPRED2";
            break;
        case PRED:      name = "PRED";
            break;
        case CONSTANT:  name = "CONSTANT";
            break;
        default:        name = "ERROR";
            break;
    }
    return name;
}




} // namespace minor
} // namespace gem5
