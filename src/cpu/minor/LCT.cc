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
LCTClass::LCTClass(const std::string &aName, int numEntries, int counterSize) :
    Named(aName), 
    mIndexBits(static_cast<std::uint8_t>(log2(numEntries))) // countsize will be 1 or 2 for now
{
    this->entries = new lctEntryStruct[numEntries];
    int i;
    for (i = 0; i < numEntries; i++) {
        this->entries[i].valid = false;
        this->entries[i].state = LCTClass::NOPRED1;
        this->entries[i].counterSize = counterSize;
        //this->entries[i].counter.counterSize = counterSize;
        //this->entries[i].counter.counterState = NOPRED1;
    }
}

// not sure what to do with return types

bool LCTClass::GetPrediction(std::uint64_t aAddress, bool& aIsConstant) {
    std::uint64_t lIndexMask = pow(2, mIndexBits) - 1;
    std::uint64_t lIndex = (aAddress >> 2) & lIndexMask;

    bool lPredict = false;
    aIsConstant = false;

    LCTClass::lctEntryStruct entry = this->entries[lIndex];
    if (entry.valid) {
        switch(entry.state)
        {
            default:
            case NOPRED1:
            case NOPRED2:
            {
                lPredict = false;
            }
            break;

            case PRED:
            {
                lPredict = true;
            }
            break;

            case CONSTANT:
            {
                lPredict = true;
                aIsConstant = true;
            }
            break;
        }
    }
    
    return lPredict;
}

bool LCTClass::adjustPrediction(std::uint64_t aAddress, bool predCorrect) {
    std::uint64_t lIndexMask = pow(2, mIndexBits) - 1;
    std::uint64_t lIndex = (aAddress >> 2) & lIndexMask;

    LCTClass::counterState nextPred = counterState::NOPRED1;
    LCTClass::lctEntryStruct entry = this->entries[lIndex];
    entry.valid = true; // this address has been seen

    bool constantPred = false;
    
    if (entry.counterSize == 1) {
        if (predCorrect) { // correct prediction
            switch(entry.state) {
                case NOPRED1:   nextPred = CONSTANT;
                                constantPred = true;
                    break;
                case CONSTANT:  nextPred = CONSTANT;
                    break;
                default:        nextPred = entry.state;
                    break;
            }

        } else { // incorrect prediction
            switch(entry.state) {
                case NOPRED1:   nextPred = NOPRED1; 
                    break;
                case CONSTANT:  nextPred = NOPRED1;
                    break;
                default:        nextPred = entry.state;
                    break;
            }
        }
    } else if (entry.counterSize == 2) {
        if (predCorrect) {
            switch(entry.state) {
                case NOPRED1:   nextPred = NOPRED2; 
                    break;
                case NOPRED2:   nextPred = PRED;
                    break;
                case PRED:      nextPred = CONSTANT;
                                constantPred = true;
                    break;
                case CONSTANT:  nextPred = CONSTANT;
                    break;
                default:        nextPred = entry.state;
                    break;
            }

        } else {
            switch(entry.state) {
                case NOPRED1:   nextPred = NOPRED1; 
                    break;
                case NOPRED2:   nextPred = NOPRED1;
                    break;
                case PRED:      nextPred = NOPRED2;
                    break;
                case CONSTANT:  nextPred = PRED;
                    break;
                default:        nextPred = entry.state;
                    break;
            }

        }
    }


    DPRINTF(LvpDebug, "LCT Updated Prediction for Index: 0x%03lX for address: 0x%016lX: %s converted to %s correct:%u\n",
                    lIndex, aAddress, stateToString(entry.state).c_str(), stateToString(nextPred).c_str(), predCorrect);
    
    if (constantPred) {
        DPRINTF(LvpDebug, "LCT Upgraded to Constant for Index: 0x%03lX for address: 0x%016lX\n",
                    lIndex, aAddress);
    }

    entry.state = nextPred;

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
