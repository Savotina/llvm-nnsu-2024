#include "X86.h"
#include "X86InstrInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"

using namespace llvm;

class X86SavotinaInstrCounterPass : public MachineFunctionPass {
public:
  static inline char ID = 0;

  X86SavotinaInstrCounterPass() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &machFunc) override {
    const TargetInstrInfo *tgtInstrInfo = nullptr;
    DebugLoc dbgLoc;

    if (!machFunc.empty() && machFunc.front().begin() != machFunc.front().end()) {
      tgtInstrInfo = machFunc.getSubtarget().getInstrInfo();
      dbgLoc = machFunc.front().begin()->getDebugLoc();

      if (tgtInstrInfo) {
        unsigned regCount = machFunc.getRegInfo().createVirtualRegister(&X86::GR64RegClass);

        for (auto BBIter = machFunc.begin(); BBIter != machFunc.end(); ++BBIter) {
          MachineBasicBlock &machBasicBlock = *BBIter;

          if (!machBasicBlock.empty()) {
            unsigned count = 0;
            for (auto InstrIter = machBasicBlock.begin(), InstrEnd = machBasicBlock.end();
                 InstrIter != InstrEnd; ++InstrIter) {
              MachineInstr &machInstr = *InstrIter;
              bool isDebug = machInstr.isDebugInstr();
              if (!isDebug) {
                count++;
              }
            }
            if (count) {
              BuildMI(machBasicBlock, machBasicBlock.getFirstTerminator(),
                      dbgLoc, tgtInstrInfo->get(X86::ADD64ri32), regCount)
                  .addReg(regCount)
                  .addImm(count);
            }
          }
        }

        BuildMI(machFunc.back(), machFunc.back().getFirstTerminator(), 
                dbgLoc, tgtInstrInfo->get(X86::MOV64mi32))
            .addReg(regCount)
            .addImm(1)
            .addExternalSymbol("ic");
      }
    }

    return true;
  }
};

INITIALIZE_PASS(X86SavotinaInstrCounterPass, "x86-savotina-instr-counter",
                "This pass let to count the X86 machine instructions' number", false, false)