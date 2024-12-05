#ifndef __LVPT_HH__
#define __LVPT_HH__

#include <stdint.h>

#include "base/named.hh"
#include "cpu/minor/dyn_inst.hh"

namespace gem5
{

namespace minor
{

class LVPTClass : public Named
{
  public:
    LVPTClass(const std::string &aName, int numEntries);

    bool AddEntry(std::uint64_t aAddress, std::uint64_t aData);

    bool GetPrediction(std::uint64_t aAddress, std::uint64_t &aData);

    static bool IsPredictableLoad(MinorDynInstPtr inst);

  private:
    struct lvptEntryStruct
    {
      std::uint64_t data = 0;
      bool valid = false;
    };

    lvptEntryStruct* entries;

    std::uint8_t mIndexBits;

};

}

}

#endif /* __LVPT_HH__ */
