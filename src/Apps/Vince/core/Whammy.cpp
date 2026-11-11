// UNIT: src/Apps/Vince/core/Whammy.cpp OBJ: core\XBox\OfficialRelease\Whammy.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"
#include "../../../Libs/SwampShared/SwampString.h"
#include "../../../Libs/SwampShared/SwampVector.h"
#include "../../../Libs/SwampShared/SwampResource.h"
#include "../../../Libs/SwampShared/Matrix.h"
#include "../../../Libs/Swamp/include/Swamp.h"
#include "../../../Libs/Swamp/include/SwampInput.h"
#include "../../../Libs/Swamp/include/SwampSceneNode.h"
#include "../../../Libs/Swamp/include/SwampGatorNode.h"
#include "../../../Libs/Swamp/include/SwampScript.h"
#include "../../../Libs/Swamp/include/SwampScriptManager.h"
#include "../../../Libs/Swamp/include/SoundNode.h"
#include "Critter.h"
#include "CritterMain.h"
#include "CritterManager.h"
#include "Vince.h"
#include "Globals.h"
#include "GameCam.h"
#include "CameraManager.h"
#include "Whammy.h"

extern "C" int __cdecl _strnicmp(const char*, const char*, unsigned int);

#ifdef NON_MATCHING // D78, D117: default.pdb records no free Vector3 operator (5.2 T20)
inline Vector3 operator+(const Vector3& a, const Vector3& b) { return Vector3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline Vector3 operator-(const Vector3& a, const Vector3& b) { return Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
inline Vector3 operator*(float f, const Vector3& v) { return Vector3(v.x * f, v.y * f, v.z * f); }
#endif

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(float min, float max);
}

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

// A SwampScript this unit only registers (RegisterMe adds creationFunc to SwampScriptManager); its own bytes from
// 0x4C are not reached.
class WaitForWhammyProjectileMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    WaitForWhammyProjectileMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_4C[0x5C - 0x4C];        // 0x4C (not reached)
};
DECOMP_SIZE_ASSERT(WaitForWhammyProjectileMain, 0x5C);

// Moves its node along a cubic curve (_coeff, from _p1, _p2, _r1, _r2) from Vince to the critter named _target;
// each FrameTick adds the elapsed time to _age and places and turns the node at _age / _ageMax. Past the end it gives
// the target the script _scriptName, stops _soundNode and invalidates itself.
class WhammyProjectileMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SET_TARGET = 0x14,
        MESSAGE_SET_RESPONSE_SCRIPT = 0x15
    };
    WhammyProjectileMain(SwampSceneNode* node);
    virtual void FrameTick();
    virtual void Message(SwampScript* source, int message, void* data);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    void InitCurve();
    void UpdateCoefficients();

    static int _soundCount;

    char field_4C[0x50 - 0x4C];        // 0x4C (_effect; not reached)
    Vector3 _p1;                       // 0x50
    Vector3 _p2;                       // 0x5C
    Vector3 _r1;                       // 0x68
    Vector3 _r2;                       // 0x74
    Vector3 _coeff[4];                 // 0x80
    float _age;                        // 0xB0
    float _ageMax;                     // 0xB4
    SwampString _target;               // 0xB8
    SwampString _scriptName;           // 0xD4
    SoundNode* _soundNode;             // 0xF0
};
DECOMP_SIZE_ASSERT(WhammyProjectileMain, 0xF4);

// FUNCTION: USA 0x00133D9D
// PROTOTYPE: PREVIEW 0x0051DFC0 identical
// NAME: proto-pdb:default.pdb
class WhammyResultScript;

// FUNCTION: USA 0x001349D9
// PROTOTYPE: PREVIEW 0x0051E7EA identical
// NAME: proto-pdb:default.pdb
// ??1WhammyResultFrozenMain: compiler-generated (implicit, no vptr store); emitted by pin_emit_WhammyResultFrozenMain_dtor
class WhammyResultFrozenMain;

// ??_GWhammyProjectileMain 0x00133F3D, ??_GWhammyResultFrozenMain 0x001349BD, ??_GVinceWhammyMain 0x00134E35,
// ??_GWhammyPropMain 0x00135F1E unmatched, no draft (5.3 T6.c15): companion wall: a virtual ??_G is emitted only with
// its class vtable; the retail ??1 of each is implicit (no vptr store) and the ctors (0x00133E96, 0x0013469C,
// 0x00134D1B, 0x00135939) are not passing rows. Lift: match the class ctor.
// ??_GVinceFlat 0x001348F8 unmatched, no draft: unsized pin (size `-`, precheck ADD refuses) and fold-name wall
// (VinceFlat/BasketStopper* live in other units; the fold also holds this unit's WhammyResult{Flatten,Poke,Jackhammer,
// Toxic,Twister}Main, whose ctors are not rows). Lift: size the pin, pin a this-unit name, match that class's ctor.
// 5.4 T6.c4: pin sized (0x1C), ??0VinceFlat 0x0012177A now matched in VinceFlat.cpp; ??_GVinceFlat still not claimable:
// the cut gives 0x001348F8 to Whammy.obj, match_one objdiff 0.0 (fold-name wall as above)

// FUNCTION: USA 0x001338F3
// PROTOTYPE: PREVIEW 0x0051DB07 identical
// NAME: proto-pdb:default.pdb
void WhammyResultDisintegrateMain::TimerEvent(unsigned int timerId)
{
    switch (timerId) {
    case TIMER_FINISH:
        AnimationFinished();
        break;
    }
}

// FUNCTION: USA 0x00133902
// PROTOTYPE: PREVIEW 0x0051DB16 identical
// NAME: proto-pdb:default.pdb
void WhammyResultTwisterMain::AnimationFinished()
{
    KillMonsterNow();
    InvalidateScript();
}

// !FAKE: inliner; scaffold call; odr-uses the implicit ??1WhammyResultScript@@UAE@XZ before the ctor's vtable emits
// ??_GWhammyResultScript, so ??_G inlines it as retail does (retail's derived dtors in this unit are unmatched;
// without it ??_G calls ??1 out of line) (matched_set [[emitter]] row)
void pin_emit_WhammyResultScript_dtor(WhammyResultScript* p)
{
    p->WhammyResultScript::~WhammyResultScript();
}

// FUNCTION: USA 0x00133912
// PROTOTYPE: PREVIEW 0x0051DB35 identical
// NAME: proto-pdb:default.pdb
WhammyResultScript::WhammyResultScript(SwampSceneNode* node) : SwampScript(node)
{
}

// FUNCTION: USA 0x0013392E
// PROTOTYPE: PREVIEW 0x0051DB51 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 67.72, bytes mismatch; wall: retail's totarget length is inline-asm sqrt
// (rsqrtss + fcompi/fcmovbe/ffree), D116 forbids __asm (C0219)
void WhammyProjectileMain::InitCurve()
{
    Critter* critter = pManager->Find(_target);
    if (critter) {
        Vince* vince = pManager->GetVince();
        _p1 = vince->GetWorldPosition();
        _p1.y += vince->GetHeight() * 0.5f;
        _p2 = critter->GetWorldPosition();
        _p2.y += critter->GetHeight() * 0.5f;
        Vector3 totarget = _p2 - _p1;
        float length = totarget.Length();
        _r1 = 4.0f * totarget;
        _r1.RotateJ(BeepLib::RandomFloat(-1.03672564f, 1.03672564f));
        _r2 = _r1;
        _r1.y += length;
        _r2.y -= length;
        _ageMax = length / gSettings._whammyProjectileSpeed;
    }
}
#endif

// FUNCTION: USA 0x00133A87
// PROTOTYPE: PREVIEW 0x0051DCAA identical
// NAME: proto-pdb:default.pdb
void WhammyProjectileMain::UpdateCoefficients()
{
    Critter* critter = pManager->Find(_target);
    _p2 = critter->GetWorldPosition();
    _p2.y += critter->GetHeight() * 0.5f;
    _coeff[0].x = _p1.x;
    _coeff[0].y = _p1.y;
    _coeff[0].z = _p1.z;
    _coeff[1].x = _r1.x;
    _coeff[1].y = _r1.y;
    _coeff[1].z = _r1.z;
    _coeff[2].x = 3.0f * _p2.x - 3.0f * _p1.x - 2.0f * _r1.x - _r2.x;
    _coeff[2].y = 3.0f * _p2.y - 3.0f * _p1.y - 2.0f * _r1.y - _r2.y;
    _coeff[2].z = 3.0f * _p2.z - 3.0f * _p1.z - 2.0f * _r1.z - _r2.z;
    _coeff[3].x = 2.0f * _p1.x - 2.0f * _p2.x + _r1.x + _r2.x;
    _coeff[3].y = 2.0f * _p1.y - 2.0f * _p2.y + _r1.y + _r2.y;
    _coeff[3].z = 2.0f * _p1.z - 2.0f * _p2.z + _r1.z + _r2.z;
}

// FUNCTION: USA 0x00133D00
// PROTOTYPE: PREVIEW 0x0051DF23 identical
// NAME: proto-pdb:default.pdb
void VinceWhammyMain::SetState(int state)
{
    _state = state;
    if (state == STATE_WHAMMY_ANIM)
        GetCritter()->StartAnimation(_animPowerUp, true, false);
}

// FUNCTION: USA 0x00133D20
// PROTOTYPE: PREVIEW 0x0051DF43 identical
// NAME: proto-pdb:default.pdb
void WhammyResultScript::KillMonsterNow()
{
    // !FAKE: c2 register allocation; stand-in local (no PDB local); retail keeps _node in edi across both virtual calls
    Critter* critter = (Critter*)_node;
    if (_modelRestore.IsValid())
        critter->SetModel(_modelRestore);
    _parent->Message(this, 0x14, 0);
    critter->SetEdgeGlow(false);
    critter->MakeDead(true);
}

// FUNCTION: USA 0x00133D57
// PROTOTYPE: PREVIEW 0x0051DF7A identical
// NAME: proto-pdb:default.pdb
void WhammyResultFrozenMain::TimerEvent(unsigned int timerId)
{
    if (timerId == TIMER_FINISH) {
        // !FAKE: c2 register allocation; stand-in local (no PDB local); trial 5.2 T14: without it the zero moves ebx->edx (no push ebx) and _node is reloaded into ecx before StartAnimation
        Critter* critter = (Critter*)_node;
        critter->SetEdgeGlow(false);
        if (_animRecover.IsValid())
            critter->StartAnimation(_animRecover, false, false);
        else
            AnimationFinished();
    }
}

// FUNCTION: USA 0x00133D85
// PROTOTYPE: PREVIEW 0x0051DFA8 identical
// NAME: proto-pdb:default.pdb
void WhammyResultFrozenMain::AnimationFinished()
{
    // !FAKE: c1xx evaluation order; stand-in local (no PDB local); trial 5.2 T14: without it _parent loads after the InvalidateScript call and esi holds this instead of _parent
    SwampScript* parent = _parent;
    InvalidateScript();
    parent->Message(0, 10, 0);
}

// FUNCTION: USA 0x00133DC5
// PROTOTYPE: PREVIEW 0x0051DFE8 identical
// NAME: proto-pdb:default.pdb
SwampScript* WaitForWhammyProjectileMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("core\\Whammy.cpp", 256) WaitForWhammyProjectileMain(node);
}

// FUNCTION: USA 0x001362D6
// NAME: proto-pdb:default.pdb
WaitForWhammyProjectileMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WaitForWhammyProjectileMain", creationFunc);
}

// FUNCTION: USA 0x00133F16
// PROTOTYPE: PREVIEW 0x0051E139 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyProjectileMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("core\\Whammy.cpp", 31) WhammyProjectileMain(node);
}

// FUNCTION: USA 0x0013629A
// NAME: proto-pdb:default.pdb
WhammyProjectileMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WhammyProjectileMain", creationFunc);
}

// FUNCTION: USA 0x00133F59
// PROTOTYPE: PREVIEW 0x0051E17C identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~WhammyProjectileMain in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_WhammyProjectileMain_dtor(WhammyProjectileMain* p)
{
    p->WhammyProjectileMain::~WhammyProjectileMain();
}

// FUNCTION: USA 0x00133F82
// PROTOTYPE: PREVIEW 0x0051E1A5 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 92.11, bytes mismatch (forward's x87 schedule, early &pos push); retail inlines nested free
// Vector3 operator+/operator* (no PDB record, D78) that our /O1 keeps out of line; hand forms below
void WhammyProjectileMain::FrameTick()
{
    _age += g_swamp->GetInput().GetElapsedTime();
    float pct = _age / _ageMax;
    Critter* critter = pManager->Find(_target);
    if (!critter) {
        InvalidateScript();
    } else if (pct > 1.0f) {
        critter->GetScript()->InvalidateScript();
        critter->SetScript(_scriptName);
        if (_soundNode) {
            _soundNode->Stop();
            _soundNode->Invalidate();
            _soundNode = 0;
            _soundCount--;
        }
        InvalidateScript();
    } else {
        UpdateCoefficients();
        Matrix m;
        Vector3 pos;
        float xr, yr;
        Vector3 dCoeff[3];
        dCoeff[2] = Vector3(_coeff[3].x * 3.0f, _coeff[3].y * 3.0f, _coeff[3].z * 3.0f);
        dCoeff[1] = Vector3(_coeff[2].x * 2.0f, _coeff[2].y * 2.0f, _coeff[2].z * 2.0f);
        dCoeff[0] = _coeff[1];
        Vector3 forward;
        {
            // !FAKE: inliner; hand form of retail's inlined free Vector3 operators; stand-ins v0-v2 (92.11 with, 66.4 without)
            Vector3 v0(dCoeff[2].x * pct, dCoeff[2].y * pct, dCoeff[2].z * pct);
            Vector3 v1(dCoeff[1].x + v0.x, dCoeff[1].y + v0.y, dCoeff[1].z + v0.z);
            Vector3 v2(v1.x * pct, v1.y * pct, v1.z * pct);
            forward.x = v2.x + dCoeff[0].x;
            forward.y = v2.y + dCoeff[0].y;
            forward.z = v2.z + dCoeff[0].z;
        }
        float zr;
        m.Orient(forward, Vector3(0.0f, 1.0f, 0.0f));
        m.Euler(xr, yr, zr);
        {
            // !FAKE: inliner; hand form of retail's inlined Quaternion(float, float, float); stand-in q; no before/after recorded
            Quaternion q;
            q.Set(xr, yr, zr);
            _node->SetRotation(q);
        }
        {
            // !FAKE: inliner; hand form of retail's inlined free Vector3 operators; stand-ins v3-v9 (88.93 -> 92.11 with refs)
            Vector3 v3(_coeff[3].x * pct, _coeff[3].y * pct, _coeff[3].z * pct);
            Vector3 v4(v3.x + _coeff[2].x, v3.y + _coeff[2].y, v3.z + _coeff[2].z);
            Vector3 v5(v4.x * pct, v4.y * pct, v4.z * pct);
            const Vector3& v6 = _coeff[1];
            Vector3 v7(v5.x + v6.x, v5.y + v6.y, v5.z + v6.z);
            Vector3 v8(v7.x * pct, v7.y * pct, v7.z * pct);
            const Vector3& v9 = _coeff[0];
            pos.x = v8.x + v9.x;
            pos.y = v8.y + v9.y;
            pos.z = v8.z + v9.z;
        }
        _node->SetWorldPosition(pos);
    }
}
#endif

// FUNCTION: USA 0x00134207
// NAME: proto-pdb:default.pdb
void WhammyPropMain::AnimationFinished()
{
    if (_cameraShake < 0.0f) {
        // !FAKE: provenance; stand-in local (the PDB lists none); trial 5.2 T14: not removable, cam is a Find call result used twice (test and call), inlining it repeats the call
        GameCam* cam = g_CameraManager->Find(_cameraName);
        if (cam)
            cam->SetShakeFactor(0.0f, -1.0f);
    }
    _node->SetOption(SwampSceneNode::DontDraw, true, false);
    _finished = true;
    // !FAKE: c2 register allocation; stand-in local (the PDB lists none); trial 5.2 T14: without it edi no longer holds vince, _vinceCache is reloaded from pManager at each later use
    Vince* vince = pManager->GetVince();
    vince->GetScript()->InvalidateScript();
    if (_restoreVinceModel)
        vince->SetModel(_modelVince);
    vince->GetScript()->Message(0, VinceMain::MESSAGE_WHAMMY_CLEANUP, 0);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~WhammyResultFrozenMain in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_WhammyResultFrozenMain_dtor(WhammyResultFrozenMain* p)
{
    p->WhammyResultFrozenMain::~WhammyResultFrozenMain();
}

// FUNCTION: USA 0x001349FC
// PROTOTYPE: PREVIEW 0x0051EC00 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultFlattenMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 98) WhammyResultFlattenMain(node);
}

// FUNCTION: USA 0x00136006
// NAME: proto-pdb:default.pdb
WhammyResultFlattenMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WhammyResultFlattenMain", creationFunc);
}

// FUNCTION: USA 0x00134A20
// PROTOTYPE: PREVIEW 0x0051EC24 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultFrozenMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 119) WhammyResultFrozenMain(node);
}

// FUNCTION: USA 0x00136042
// NAME: proto-pdb:default.pdb
WhammyResultFrozenMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WhammyResultFrozenMain", creationFunc);
}

// FUNCTION: USA 0x00134A44
// PROTOTYPE: PREVIEW 0x0051EC48 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultCutHorizontalMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 142) WhammyResultCutHorizontalMain(node);
}

// FUNCTION: USA 0x0013607E
// NAME: proto-pdb:default.pdb
WhammyResultCutHorizontalMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WhammyResultCutHorizontalMain", creationFunc);
}

// FUNCTION: USA 0x00134A6B
// PROTOTYPE: PREVIEW 0x0051EC6F identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultCutVerticalMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 163) WhammyResultCutVerticalMain(node);
}

// FUNCTION: USA 0x001360BA
// NAME: proto-pdb:default.pdb
WhammyResultCutVerticalMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WhammyResultCutVerticalMain", creationFunc);
}

// FUNCTION: USA 0x00134A92
// PROTOTYPE: PREVIEW 0x0051EC96 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultCutMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 184) WhammyResultCutMain(node);
}

// FUNCTION: USA 0x001360F6
// NAME: proto-pdb:default.pdb
WhammyResultCutMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WhammyResultCutMain", creationFunc);
}

// FUNCTION: USA 0x00134AB9
// PROTOTYPE: PREVIEW 0x0051ECBD identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultDisintegrateMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 205) WhammyResultDisintegrateMain(node);
}

// FUNCTION: USA 0x00136132
// NAME: proto-pdb:default.pdb
WhammyResultDisintegrateMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WhammyResultDisintegrateMain", creationFunc);
}

// FUNCTION: USA 0x00134AE0
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultTwisterMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 226) WhammyResultTwisterMain(node);
}

// FUNCTION: USA 0x0013616E
// NAME: proto-pdb:default.pdb
WhammyResultTwisterMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WhammyResultTwisterMain", creationFunc);
}

// FUNCTION: USA 0x00134B07
// PROTOTYPE: PREVIEW 0x0051ED0B identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultPokeMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 246) WhammyResultPokeMain(node);
}

// FUNCTION: USA 0x001361AA
// NAME: proto-pdb:default.pdb
WhammyResultPokeMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WhammyResultPokeMain", creationFunc);
}

// FUNCTION: USA 0x00134B2E
// PROTOTYPE: PREVIEW 0x0051ED32 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultJackhammerMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 266) WhammyResultJackhammerMain(node);
}

// FUNCTION: USA 0x001361E6
// NAME: proto-pdb:default.pdb
WhammyResultJackhammerMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WhammyResultJackhammerMain", creationFunc);
}

// FUNCTION: USA 0x00134B55
// PROTOTYPE: PREVIEW 0x0051ED59 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultToxicMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 286) WhammyResultToxicMain(node);
}

// FUNCTION: USA 0x00136222
// NAME: proto-pdb:default.pdb
WhammyResultToxicMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WhammyResultToxicMain", creationFunc);
}

// FUNCTION: USA 0x00134B7C
// PROTOTYPE: PREVIEW 0x0051ED80 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultBurstMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 306) WhammyResultBurstMain(node);
}

// FUNCTION: USA 0x0013625E
// NAME: proto-pdb:default.pdb
WhammyResultBurstMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WhammyResultBurstMain", creationFunc);
}

// FUNCTION: USA 0x00134CAB
// PROTOTYPE: PREVIEW 0x0051EEAF identical
// NAME: proto-pdb:default.pdb
void WhammyProjectileMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_SET_TARGET:
        _target = SwampString((const char*)data);
        InitCurve();
        break;
    case MESSAGE_SET_RESPONSE_SCRIPT:
        _scriptName = SwampString((const char*)data);
        break;
    }
}

// FUNCTION: USA 0x00134E51
// PROTOTYPE: PREVIEW 0x0051F055 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~VinceWhammyMain in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_VinceWhammyMain_dtor(VinceWhammyMain* p)
{
    p->VinceWhammyMain::~VinceWhammyMain();
}

// FUNCTION: USA 0x00134F0A
// PROTOTYPE: PREVIEW 0x0051F0FA identical
// NAME: proto-pdb:default.pdb
void VinceWhammyMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_SET_WHAMMY_NAME:
        _whammyName = (const char*)data;
        CreateProp();
        break;
    case MESSAGE_KILL_WHAMMY:
        _prop->Invalidate();
        InvalidateScript();
        break;
    }
}

// FUNCTION: USA 0x00135502
// PROTOTYPE: PREVIEW 0x0051F6F2 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceWhammyMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 16) VinceWhammyMain(node);
}

// FUNCTION: USA 0x00135F8E
// NAME: proto-pdb:default.pdb
VinceWhammyMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("VinceWhammyMain", creationFunc);
}

// FUNCTION: USA 0x00135529
// PROTOTYPE: PREVIEW 0x0051F719 identical
// NAME: proto-pdb:default.pdb
void VinceWhammyMain::CreateProp()
{
    if (_strnicmp(_whammyName.c_str(), "whammy", 6) == 0) {
        SwampString propName(_whammyName, 6, (unsigned int)-1);
        SwampString modelName = propName + "_base";
        SwampString animName = propName + "_attack001";
        ResourceHandle<ModelResource> model = g_swamp->GetModelManager().Load(modelName);
        ResourceHandle<AnimationResource> anim = g_swamp->GetAnimationManager().Load(animName);
        if (model.IsValid() && anim.IsValid()) {
            _prop = pManager->Create(propName, "WhammyProp", propName.c_str(), model,
                Vector3(0.0f, 0.0f, 0.0f), Quaternion(0.0f, 0.0f, 0.0f, 1.0f), 0);
            _prop->RemoveTreeFromScene();
            _node->AddChild(_prop);
            _prop->_animTriggerFlags = 0x100;
            _prop->StartAnimation(anim, false, false);
            return;
        }
    }
    InvalidateScript();
}

// FUNCTION: USA 0x00135F3A
// PROTOTYPE: PREVIEW 0x0052012A identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~WhammyPropMain in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_WhammyPropMain_dtor(WhammyPropMain* p)
{
    p->WhammyPropMain::~WhammyPropMain();
}

// FUNCTION: USA 0x00135F67
// PROTOTYPE: PREVIEW 0x00520157 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyPropMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 54) WhammyPropMain(node);
}

// FUNCTION: USA 0x00135FCA
// NAME: proto-pdb:default.pdb
WhammyPropMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WhammyPropMain", creationFunc);
}
