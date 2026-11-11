// UNIT: src/Libs/Swamp/Xbox/Official_Release/Swamp.cpp OBJ: Xbox\Official_Release\Swamp.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

#pragma warning(disable: 4530) // no /EH in the unit flags; as WalkSounds.cpp
#include <vector>
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

namespace BeepLib { // NAME: proto-pdb:default.pdb
double GetTime();
}

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    void Update();
};

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    void BeginScene();
    void EndScene();
};

class SwampDebugGeometry { // NAME: proto-pdb:default.pdb
public:
    void Update();
};

class DiskArbitrator { // NAME: proto-pdb:default.pdb
public:
    void Update();
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    void Update();
};

class TextureManager { // NAME: proto-pdb:default.pdb
public:
    void Update();
};

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    static SwampScriptManager& Instance();
    void Cleanup();
};

class SwampNodeManager { // NAME: proto-pdb:default.pdb
public:
    static SwampNodeManager& Instance();
    void Cleanup();
};

class BSP { // NAME: proto-pdb:default.pdb
public:
    ~BSP();
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    void SetGlobalBSP(BSP* bsp);
    void BeginScene();
    void MarkSceneRenderingStartTime();
    void EndScene();
private:
    Swamp(const SwampString& renderswitches);
    friend void Swamp_Init(const SwampString& renderswitches);
public:

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x4];                 // 0x0C
    SwampDebugGeometry* _debugGeometry; // 0x10
    DiskArbitrator* _diskArbitrator;   // 0x14
    char field_18[0x1C];               // 0x18
    SoundManager* _soundManager;       // 0x34
    TextureManager* _textureManager;   // 0x38
    char field_3C[0x8];                // 0x3C
    BSP* _bsp;                         // 0x44
    double _timeFrameBegan;            // 0x48
    double _timeSceneRenderingBegan;   // 0x50
    float _lastFrameProcessingTime;    // 0x58
    float _lastFrameRenderingTime;     // 0x5C
    unsigned int _frameID;             // 0x60
    char field_64[0x4];                // 0x64
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

enum PARAM_TYPES { // NAME: proto-pdb:default.pdb
    PARAM_FLOAT = 0x0,
    PARAM_STRING = 0x1,
    PARAM_VECTOR2 = 0x2,
    PARAM_VECTOR3 = 0x3,
    PARAM_VECTOR4 = 0x4,
    PARAM_VERTEX = 0x5,
    PARAM_QUATERNION = 0x6,
    PARAM_INT = 0x7,
    PARAM_DOTANGLE = 0x8,
};

class Param { // NAME: proto-pdb:default.pdb
public:
    const char* _Name;                 // 0x00
    PARAM_TYPES _Type;                 // 0x04
    float _Min;                        // 0x08
    float _Max;                        // 0x0C
    const char* _s;                    // 0x10
    void* val;                         // 0x14
    bool _Modify;                      // 0x18
};
DECOMP_SIZE_ASSERT(Param, 0x1C);

// FUNCTION: USA 0x000AD32B
// PROTOTYPE: PREVIEW 0x004981D1 identical
// NAME: proto-pdb:default.pdb
void Swamp::MarkSceneRenderingStartTime()
{
    _timeSceneRenderingBegan = BeepLib::GetTime();
}

// FUNCTION: USA 0x000AD338
// PROTOTYPE: PREVIEW 0x004981DE identical
// NAME: proto-pdb:default.pdb
void Swamp::SetGlobalBSP(BSP* bsp)
{
    delete _bsp;
    _bsp = bsp;
}

// FUNCTION: USA 0x000AD5B1
// NAME: proto-pdb:default.pdb
void Swamp::BeginScene()
{
    _timeFrameBegan = BeepLib::GetTime();
    _frameID++;
    _input->Update();
    _debugGeometry->Update();
    _renderer->BeginScene();
}

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

// FUNCTION: USA 0x000AD5D8
// PROTOTYPE: PREVIEW 0x0049847E identical
// NAME: proto-pdb:default.pdb
void Swamp_Init(const SwampString& renderswitches)
{
    new("Swamp.cpp", 31) Swamp(renderswitches);
}

// FUNCTION: USA 0x000AD5F9
// PROTOTYPE: PREVIEW 0x004123ED identical
// NAME: proto-pdb:default.pdb
template void std::vector<Param,std::allocator<Param> >::_Tidy();

// FUNCTION: USA 0x000AD679
// PROTOTYPE: PREVIEW 0x00498500 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 86.76, bytes mismatch (retail frame `and esp,-16; sub esp,0x1c` from an unseen SSE construct)
void Swamp::EndScene()
{
    const double frameend = _timeSceneRenderingBegan;
    const double framecomplete = BeepLib::GetTime();
    _renderer->EndScene();
    _lastFrameProcessingTime = frameend - _timeFrameBegan;
    _lastFrameRenderingTime = framecomplete - _timeFrameBegan;
    SwampScriptManager::Instance().Cleanup();
    SwampNodeManager::Instance().Cleanup();
    _soundManager->Update();
    _diskArbitrator->Update();
    _textureManager->Update();
}
#endif
