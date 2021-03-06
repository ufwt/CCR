//===-- AMDGPUAsmBackend.cpp - AMDGPU Assembler Backend -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
/// \file
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/AMDGPUMCTargetDesc.h"
#include "MCTargetDesc/AMDGPUFixupKinds.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

namespace {

class AMDGPUMCObjectWriter : public MCObjectWriter {
public:
  AMDGPUMCObjectWriter(raw_pwrite_stream &OS) : MCObjectWriter(OS, true) {}
  void executePostLayoutBinding(MCAssembler &Asm,
                                const MCAsmLayout &Layout) override {
    //XXX: Implement if necessary.
  }
  void recordRelocation(MCAssembler &Asm, const MCAsmLayout &Layout,
                        const MCFragment *Fragment, const MCFixup &Fixup,
                        MCValue Target, bool &IsPCRel,
                        uint64_t &FixedValue) override {
    assert(!"Not implemented");
  }

  void writeObject(MCAssembler &Asm, const MCAsmLayout &Layout) override;

};

class AMDGPUAsmBackend : public MCAsmBackend {
public:
  AMDGPUAsmBackend(const Target &T)
    : MCAsmBackend() {}

  unsigned getNumFixupKinds() const override { return AMDGPU::NumTargetFixupKinds; };
  void applyFixup(const MCFixup &Fixup, char *Data, unsigned DataSize,
                  uint64_t Value, bool IsPCRel) const override;
  bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                            const MCRelaxableFragment *DF,
                            const MCAsmLayout &Layout) const override {
    return false;
  }
  void relaxInstruction(const MCInst &Inst, const MCSubtargetInfo &STI,
                        MCInst &Res) const override {
    assert(!"Not implemented");
  }
  bool mayNeedRelaxation(const MCInst &Inst) const override { return false; }
  bool writeNopData(uint64_t Count, MCObjectWriter *OW) const override;

  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override;
  
  unsigned getFixupKindLog2Size(unsigned Kind) const override { return 0; } // Koo [N/A Here]
};

} //End anonymous namespace

void AMDGPUMCObjectWriter::writeObject(MCAssembler &Asm,
                                       const MCAsmLayout &Layout) {
  for (MCAssembler::iterator I = Asm.begin(), E = Asm.end(); I != E; ++I) {
    Asm.writeSectionData(&*I, Layout);
  }
}

static unsigned getFixupKindNumBytes(unsigned Kind) {
  switch (Kind) {
  case FK_SecRel_1:
  case FK_Data_1:
    return 1;
  case FK_SecRel_2:
  case FK_Data_2:
    return 2;
  case FK_SecRel_4:
  case FK_Data_4:
  case FK_PCRel_4:
    return 4;
  case FK_SecRel_8:
  case FK_Data_8:
    return 8;
  default:
    llvm_unreachable("Unknown fixup kind!");
  }
}

void AMDGPUAsmBackend::applyFixup(const MCFixup &Fixup, char *Data,
                                  unsigned DataSize, uint64_t Value,
                                  bool IsPCRel) const {

  switch ((unsigned)Fixup.getKind()) {
    case AMDGPU::fixup_si_sopp_br: {
      int64_t BrImm = ((int64_t)Value - 4) / 4;
      if (!isInt<16>(BrImm))
        report_fatal_error("branch size exceeds simm16");

      uint16_t *Dst = (uint16_t*)(Data + Fixup.getOffset());
      *Dst = BrImm;
      break;
    }

    default: {
      // FIXME: Copied from AArch64
      unsigned NumBytes = getFixupKindNumBytes(Fixup.getKind());
      if (!Value)
        return; // Doesn't change encoding.
      MCFixupKindInfo Info = getFixupKindInfo(Fixup.getKind());

      // Shift the value into position.
      Value <<= Info.TargetOffset;

      unsigned Offset = Fixup.getOffset();
      assert(Offset + NumBytes <= DataSize && "Invalid fixup offset!");

      // For each byte of the fragment that the fixup touches, mask in the
      // bits from the fixup value.
      for (unsigned i = 0; i != NumBytes; ++i)
        Data[Offset + i] |= uint8_t((Value >> (i * 8)) & 0xff);
    }
  }
}

const MCFixupKindInfo &AMDGPUAsmBackend::getFixupKindInfo(
                                                       MCFixupKind Kind) const {
  const static MCFixupKindInfo Infos[AMDGPU::NumTargetFixupKinds] = {
    // name                   offset bits  flags
    { "fixup_si_sopp_br",     0,     16,   MCFixupKindInfo::FKF_IsPCRel },
  };

  if (Kind < FirstTargetFixupKind)
    return MCAsmBackend::getFixupKindInfo(Kind);

  return Infos[Kind - FirstTargetFixupKind];
}

bool AMDGPUAsmBackend::writeNopData(uint64_t Count, MCObjectWriter *OW) const {
  OW->WriteZeros(Count);

  return true;
}

//===----------------------------------------------------------------------===//
// ELFAMDGPUAsmBackend class
//===----------------------------------------------------------------------===//

namespace {

class ELFAMDGPUAsmBackend : public AMDGPUAsmBackend {
  bool Is64Bit;
  bool HasRelocationAddend;

public:
  ELFAMDGPUAsmBackend(const Target &T, const Triple &TT) :
      AMDGPUAsmBackend(T), Is64Bit(TT.getArch() == Triple::amdgcn),
      HasRelocationAddend(TT.getOS() == Triple::AMDHSA) { }

  MCObjectWriter *createObjectWriter(raw_pwrite_stream &OS) const override {
    return createAMDGPUELFObjectWriter(Is64Bit, HasRelocationAddend, OS);
  }
};

} // end anonymous namespace

MCAsmBackend *llvm::createAMDGPUAsmBackend(const Target &T,
                                           const MCRegisterInfo &MRI,
                                           const Triple &TT, StringRef CPU) {
  // Use 64-bit ELF for amdgcn
  return new ELFAMDGPUAsmBackend(T, TT);
}
