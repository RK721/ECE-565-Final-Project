#ifndef __LVPT_HH__
#define __LVPT_HH__

#include <stdint.h>

#include "base/named.hh"

namespace gem5
{

namespace minor
{

class LVPTClass : public Named
{
  public:
    LVPTClass(const std::string &aName, int numEntries);

    bool AddEntry(std::uint64_t aAddress, std::uint64_t aData);

  private:
    struct lvptEntryStruct
    {
      std::uint64_t data;
      bool valid;
    };

    lvptEntryStruct* entries;

    std::uint8_t mIndexBits;

};

}

}

#endif /* __LVPT_HH__ */
