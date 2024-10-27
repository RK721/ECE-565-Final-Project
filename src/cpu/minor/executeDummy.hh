/**
 * @file
 *
 *  All the fun of executing instructions from ExecuteDummy and sending branch/new
 *  instruction stream info. to Fetch1.
 */

#ifndef __CPU_MINOR_EXECUTE_DUMMY_HH__
#define __CPU_MINOR_EXECUTE_DUMMY_HH__

#include <vector>

#include "base/named.hh"
#include "cpu/minor/buffers.hh"
#include "cpu/minor/cpu.hh"
#include "cpu/minor/dyn_inst.hh"
#include "cpu/minor/pipe_data.hh"

namespace gem5
{

GEM5_DEPRECATED_NAMESPACE(Minor, minor);
namespace minor
{

class ExecuteDummy : public Named
{
  protected:
    /** Pointer back to the containing CPU */
    MinorCPU &cpu;

    /** Input port carrying macro instructions from Fetch2 */
    Latch<ForwardInstData>::Output inp;
    /** Output port carrying micro-op decomposed instructions to Execute */
    Latch<ForwardInstData>::Input out;

    /** Interface to reserve space in the next stage */
    std::vector<InputBuffer<ForwardInstData>> &nextStageReserve;

    /** Width of output of this stage/input of next in instructions */
    unsigned int outputWidth;

    /** If true, more than one input word can be processed each cycle if
     *  there is room in the output to contain its processed data */
    bool processMoreThanOneInput;

  public:
    /* Public for Pipeline to be able to pass it to Fetch2 */
    std::vector<InputBuffer<ForwardInstData>> inputBuffer;

  protected:
    /** Data members after this line are cycle-to-cycle state */

    struct ExecuteDummyThreadInfo
    {
        ExecuteDummyThreadInfo() {}

        ExecuteDummyThreadInfo(const ExecuteDummyThreadInfo& other) :
            inputIndex(other.inputIndex),
            inMacroop(other.inMacroop),
            execSeqNum(other.execSeqNum),
            blocked(other.blocked)
        {
            set(microopPC, other.microopPC);
        }


        /** Index into the inputBuffer's head marking the start of unhandled
         *  instructions */
        unsigned int inputIndex = 0;

        /** True when we're in the process of decomposing a micro-op and
         *  microopPC will be valid.  This is only the case when there isn't
         *  sufficient space in Executes input buffer to take the whole of a
         *  decomposed instruction and some of that instructions micro-ops must
         *  be generated in a later cycle */
        bool inMacroop = false;
        std::unique_ptr<PCStateBase> microopPC;

        /** Source of execSeqNums to number instructions. */
        InstSeqNum execSeqNum = InstId::firstExecSeqNum;

        /** Blocked indication for report */
        bool blocked = false;
    };

    std::vector<ExecuteDummyThreadInfo> executeDummyInfo;
    ThreadID threadPriority;

  protected:
    /** Get a piece of data to work on, or 0 if there is no data. */
    const ForwardInstData *getInput(ThreadID tid);

    /** Pop an element off the input buffer, if there are any */
    void popInput(ThreadID tid);

    /** Use the current threading policy to determine the next thread to
     *  decode from. */
    ThreadID getScheduledThread();
  public:
    ExecuteDummy(const std::string &name,
        MinorCPU &cpu_,
        const BaseMinorCPUParams &params,
        Latch<ForwardInstData>::Output inp_,
        Latch<ForwardInstData>::Input out_,
        std::vector<InputBuffer<ForwardInstData>> &next_stage_input_buffer);

  public:
    /** Pass on input/buffer data to the output if you can */
    void evaluate();

    void minorTrace() const;

    /** Is this stage drained?  For Decoed, draining is initiated by
     *  Execute halting Fetch1 causing Fetch2 to naturally drain
     *  into ExecuteDummy and on to Execute which is responsible for
     *  actually killing instructions */
    bool isDrained();
};

} // namespace minor
} // namespace gem5

#endif /* __CPU_MINOR_EXECUTE_DUMMY_HH__ */
