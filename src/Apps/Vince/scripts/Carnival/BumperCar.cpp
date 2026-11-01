// UNIT: src/Apps/Vince/scripts/Carnival/BumperCar.cpp OBJ: Xbox\OfficialRelease\BumperCar.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampScript();                                        // vtable+0x00
    virtual bool TestActivate() const;                             // vtable+0x04

    char field_4[0x4C - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    int _state;                        // 0x4C
    char field_50[0xBC - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class BumperCarMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_PARKED = 0,
        STATE_BOARDING = 1,
        STATE_IDLE = 2,
        STATE_DRIVING = 3,
        STATE_PARKING = 4,
        STATE_CRASHFORWARD = 5,
        STATE_CRASHREVERSE = 6
    };
    virtual bool TestActivate() const;

    char field_BC[0x19C - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(BumperCarMain, 0x19C);

// FUNCTION: USA 0x00046B3B
// PROTOTYPE: PREVIEW 0x004343CE identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0 + bytes match; gate relink FAIL
void BumperCarHud::Update()
{
    float pct;
    if (_maxSpeed == 0.0f) {
        pct = 0.0f;
    } else {
        pct = _curSpeed / _maxSpeed;
        if (pct > 1.0f)
            pct = 1.0f;
    }
    // !FAKE: retail double literals (__real@400ba5614d2562ef, __real@3fc41b2f7ba3bcbe); the original expression is
    // unknown
    _needle._rotation = pct * 3.455751993850178 - 0.1570796350201586;
}
#endif
// FUNCTION: USA 0x00046BAF
// PROTOTYPE: PREVIEW 0x00434442 identical
// NAME: proto-pdb:default.pdb
bool BumperCarMain::TestActivate() const
{
    return _state == STATE_PARKED ? true : false;
}

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

class Sprite { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x18];                // 0x00
    ResourceHandle<TextureResource> _texture; // 0x18
    char field_1C[0x48 - 0x1C];        // 0x1C
    float _rotation;                   // 0x48
};
DECOMP_SIZE_ASSERT(Sprite, 0x4C);

class UINode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0xD4 - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(UINode, 0xD4);

// FUNCTION: USA 0x000483BF
// PROTOTYPE: PREVIEW 0x00435B92 identical
// NAME: proto-pdb:default.pdb
class BumperCarHud : public UINode { // NAME: proto-pdb:default.pdb
public:
    virtual void Update();

    Sprite _bgMeter;                  // 0xD4
    Sprite _needle;                    // 0x120
    float _curSpeed;                   // 0x16C
    float _maxSpeed;                   // 0x170
};
DECOMP_SIZE_ASSERT(BumperCarHud, 0x174);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1BumperCarHud@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_BumperCarHud_dtor(BumperCarHud* p)
{
    p->BumperCarHud::~BumperCarHud();
}

