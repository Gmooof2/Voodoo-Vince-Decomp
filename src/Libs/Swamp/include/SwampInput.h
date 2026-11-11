// HEADER: named for PDB class SwampInput; no file recorded
// Partial: declares only what matched units use so far.
// The PDB's file list names libs\swamp\include\swampinput.h; no proc is attributed to it.
#ifndef SWAMPINPUT_H
#define SWAMPINPUT_H

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
};

#endif
