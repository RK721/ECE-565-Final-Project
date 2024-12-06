#ifndef CVU_HH
#define CVU_HH

#include <cstdint>
#include <string>
#include <unordered_map>
#include "base/trace.hh"

namespace gem5
{

namespace minor
{

class CVUClass : public Named
{
  public:
    CVUClass(const std::string &aName);
    ~CVUClass();

    // Add an entry to the CVU
    bool AddEntry(std::uint64_t aAddress, std::uint64_t aData);

    // Check if an entry exists for the given address
    bool CheckEntry(std::uint64_t aAddress, std::uint64_t &outData) const;

    // Remove an entry for the given address
    void RemoveEntry(std::uint64_t aAddress);

  private:
    std::unordered_map<std::uint64_t, std::uint64_t> entries; // Address to data mapping
};

} // namespace minor

} // namespace gem5

#endif // CVU_HH
