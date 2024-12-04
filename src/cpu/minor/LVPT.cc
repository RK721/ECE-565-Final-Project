#include <cmath>

#include "LVPT.hh"
#include "base/trace.hh"
#include "debug/MinorMem.hh"
#include "debug/LvpDebug.hh"

namespace gem5
{

namespace minor
{

LVPTClass::LVPTClass(const std::string &aName, int numEntries) :
    Named(aName),
    mIndexBits(static_cast<std::uint8_t>(log2(numEntries)))
{
    this->entries = new lvptEntryStruct[numEntries]();
}

bool LVPTClass::AddEntry(std::uint64_t aAddress, std::uint64_t aData)  //Rick, replace these with Addr and whatever datatype is used for data
{
    std::uint64_t lIndexMask = pow(2, mIndexBits) - 1;
    std::uint64_t lIndex = (aAddress >> 2) & lIndexMask;

    bool lOverwroteData = false;

    if (this->entries[lIndex].data != aData)
    {
        lOverwroteData = this->entries[lIndex].valid;
        this->entries[lIndex].data = aData;
        this->entries[lIndex].valid = true;
    }

    DPRINTF(LvpDebug, "LVPT Updated Index: 0x%03lX for address: 0x%016lX with value: 0x%016lX\n",
                    lIndex, aAddress, this->entries[lIndex].data);

    return lOverwroteData;
}

bool LVPTClass::GetPrediction(std::uint64_t aAddress, std::uint64_t &aData)
{
    std::uint64_t lIndexMask = pow(2, mIndexBits) - 1;
    std::uint64_t lIndex = (aAddress >> 2) & lIndexMask;

    aData = this->entries[lIndex].data;

    return this->entries[lIndex].valid;
}

bool LVPTClass::IsPredictableLoad(MinorDynInstPtr inst)
{
    return inst->staticInst->isLoad() && !inst->staticInst->isMicroop();
}

/*ResetEverythingLogicThatllHaveToGoInExecute
{
    //inst is load inst

    ThreadContext *thread = cpu.getContext(inst->id.threadId);

    thread.state = FetchHalted;
    cpu.wakeupOnEvent(Pipeline::ExecuteStageId);  //Maybe Do this?????
    setDrainState(inst->id.threadId. DrainAllInsts);

    while (DrainState::Drained == cpu.drain())  //This could lead to an infinite loop...
    {
    }

    const std::unique_ptr<PCStateBase> newPC(inst->pc->clone() + 4 somehow);

    set(thread.pc, newPC);
    thread.fetchAddr = thread.pc->instAddr();

    //setDrainState(inst->id.threadId. NotDraining);
    cpu.drainResume();
    thread.state = FetchRunning;
}*/

}

}
