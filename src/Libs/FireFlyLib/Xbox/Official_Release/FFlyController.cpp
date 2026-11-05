// UNIT: src/Libs/FireFlyLib/Xbox/Official_Release/FFlyController.cpp OBJ: Xbox\Official_Release\FFlyController.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#pragma warning(disable: 4530)
#include <vector>

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class BeepFile; // NAME: proto-pdb:default.pdb

class QuickRand { // NAME: proto-pdb:default.pdb
public:
    float GetRandomFloat();
    float GetRandomFloat(float lo, float hi) { return GetRandomFloat() * (hi - lo) + lo; }

    unsigned int _Mul;   // 0x00
    unsigned int _Carry; // 0x04
};
DECOMP_SIZE_ASSERT(QuickRand, 0x8);

extern QuickRand g_fflyRand; // NAME: proto-pdb:default.pdb

class DiskQueue { // NAME: proto-pdb:default.pdb
public:
    struct Request { // NAME: proto-pdb:default.pdb
        unsigned int priority;   // 0x00
        BeepFile* f;             // 0x04
        unsigned int offset;     // 0x08
        void* data;              // 0x0C
        unsigned int bytes;      // 0x10
        unsigned int* resultLoc; // 0x14
    };
};
DECOMP_SIZE_ASSERT(DiskQueue::Request, 0x18);

struct FFlyCtrlData { // NAME: proto-pdb:default.pdb
    struct Key { // NAME: proto-pdb:default.pdb
        float time;   // 0x00
        float value;  // 0x04
        float inTan;  // 0x08
        float outTan; // 0x0C
    };

    unsigned int _type;                                           // 0x00
    bool _loop;                                                   // 0x04
    std::vector<Key, std::allocator<Key> > _keys[3];              // 0x08
};
DECOMP_SIZE_ASSERT(FFlyCtrlData::Key, 0x10);
DECOMP_SIZE_ASSERT(FFlyCtrlData, 0x38);

struct FFlyCubicKey { // NAME: proto-pdb:default.pdb
    float time[2];  // 0x00
    float coeff[4]; // 0x08
};
DECOMP_SIZE_ASSERT(FFlyCubicKey, 0x18);

class FFlyController { // NAME: proto-pdb:default.pdb
public:
    virtual ~FFlyController();                                   // vtable+0x00
    virtual void Initialize(float& min, float& max);             // vtable+0x04
    virtual float Evaluate(double t) const = 0;                  // vtable+0x08
    virtual void Set(const FFlyCtrlData& data);                  // vtable+0x0C
    static void GenerateCubicCoeff(const std::vector<FFlyCtrlData::Key, std::allocator<FFlyCtrlData::Key> >& keys,
                                   std::vector<FFlyCubicKey, std::allocator<FFlyCubicKey> >& cubicKeys);

    float* _min; // 0x04
    float* _max; // 0x08
};
DECOMP_SIZE_ASSERT(FFlyController, 0xC);

class FFlyControllerV3 { // NAME: proto-pdb:default.pdb
public:
    virtual ~FFlyControllerV3();                                 // vtable+0x00
    virtual void Initialize(Vector3& min, Vector3& max);         // vtable+0x04
    virtual void Evaluate(double t, Vector3& v) const = 0;       // vtable+0x08
    virtual void Set(const FFlyCtrlData& data);                  // vtable+0x0C

    Vector3* _min; // 0x04
    Vector3* _max; // 0x08
};
DECOMP_SIZE_ASSERT(FFlyControllerV3, 0xC);

class FFlyMinMaxCtrl : public FFlyController { // NAME: proto-pdb:default.pdb
public:
    virtual float Evaluate(double t) const;
};
DECOMP_SIZE_ASSERT(FFlyMinMaxCtrl, 0xC);

class FFlyMinMaxCtrlV3 : public FFlyControllerV3 { // NAME: proto-pdb:default.pdb
public:
    virtual void Evaluate(double t, Vector3& result) const;
};
DECOMP_SIZE_ASSERT(FFlyMinMaxCtrlV3, 0xC);

class FFlyLinearCurveCtrl : public FFlyController { // NAME: proto-pdb:default.pdb
public:
    virtual float Evaluate(double t) const;
    virtual void Set(const FFlyCtrlData& data);

    bool _loop;                                                                // 0x0C
    std::vector<FFlyCtrlData::Key, std::allocator<FFlyCtrlData::Key> > _keys;  // 0x10
};
DECOMP_SIZE_ASSERT(FFlyLinearCurveCtrl, 0x20);

class FFlyCubicCurveCtrl : public FFlyController { // NAME: proto-pdb:default.pdb
public:
    virtual float Evaluate(double t) const;
    virtual void Set(const FFlyCtrlData& data);

    bool _loop;                                                    // 0x0C
    std::vector<FFlyCubicKey, std::allocator<FFlyCubicKey> > _keys; // 0x10
};
DECOMP_SIZE_ASSERT(FFlyCubicCurveCtrl, 0x20);

class FFlyLinearCurveCtrlV3 : public FFlyControllerV3 { // NAME: proto-pdb:default.pdb
public:
    virtual void Evaluate(double t, Vector3& v) const;
    virtual void Set(const FFlyCtrlData& data);

    bool _loop;                                                                   // 0x0C
    std::vector<FFlyCtrlData::Key, std::allocator<FFlyCtrlData::Key> > _keys[3];  // 0x10
};
DECOMP_SIZE_ASSERT(FFlyLinearCurveCtrlV3, 0x40);

class FFlyCubicCurveCtrlV3 : public FFlyControllerV3 { // NAME: proto-pdb:default.pdb
public:
    virtual void Evaluate(double t, Vector3& result) const;
    virtual void Set(const FFlyCtrlData& data);

    bool _loop;                                                       // 0x0C
    std::vector<FFlyCubicKey, std::allocator<FFlyCubicKey> > _keys[3]; // 0x10
};
DECOMP_SIZE_ASSERT(FFlyCubicCurveCtrlV3, 0x40);

#ifndef NON_MATCHING
// !FAKE: declaration-only explicit specializations (C0132, C0154); retail calls these out of line; the
// header bodies would emit them here with EH funclets and throw info that relink refuses (C0090)
namespace std {
template <> void vector<FFlyCtrlData::Key,std::allocator<FFlyCtrlData::Key> >::_Xlen()const;
template <> FFlyCtrlData::Key* _Uninitialized_copy<FFlyCtrlData::Key*,FFlyCtrlData::Key*,allocator<FFlyCtrlData::Key> >(FFlyCtrlData::Key*,FFlyCtrlData::Key*,FFlyCtrlData::Key*,allocator<FFlyCtrlData::Key>&);
}
#endif

// FUNCTION: USA 0x0006DEE3
// PROTOTYPE: PREVIEW 0x004BE1D2 identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::capacity()const;

// FUNCTION: USA 0x0006DF1C
// PROTOTYPE: PREVIEW 0x0045A185 identical
// NAME: proto-pdb:default.pdb
template FFlyCtrlData::Key * std::_Copy_opt<FFlyCtrlData::Key *,FFlyCtrlData::Key *>(FFlyCtrlData::Key *,FFlyCtrlData::Key *,FFlyCtrlData::Key *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x0006DF45
// PROTOTYPE: PREVIEW 0x0045A212 identical
// NAME: proto-pdb:default.pdb
float FFlyMinMaxCtrl::Evaluate(double t) const
{
    float mn = *_min;
    float mx = *_max;
    if (mn != mx) {
        return g_fflyRand.GetRandomFloat(mn, mx);
    }
    return mn;
}

// FUNCTION: USA 0x0006DF83
// PROTOTYPE: PREVIEW 0x0045A250 identical
// NAME: proto-pdb:default.pdb
void FFlyMinMaxCtrlV3::Evaluate(double t, Vector3& result) const
{
    // !FAKE: stand-in reference locals mn, mx (no PDB locals); retail keeps _min/_max in esi/edi across the
    // result stores, the member form reloads them (match_one 78.68 -> 100)
    const Vector3& mn = *_min;
    const Vector3& mx = *_max;
    if (mn.x == mx.x && mn.y == mx.y && mn.z == mx.z) {
        result = mn;
    } else {
        result.x = g_fflyRand.GetRandomFloat(mn.x, mx.x);
        result.y = g_fflyRand.GetRandomFloat(mn.y, mx.y);
        result.z = g_fflyRand.GetRandomFloat(mn.z, mx.z);
    }
}

// FUNCTION: USA 0x0006E59D
// PROTOTYPE: PREVIEW 0x0045A84F identical
// NAME: proto-pdb:default.pdb
void FFlyCubicCurveCtrlV3::Evaluate(double t, Vector3& result) const
{
    float x = 0.0f;
    if (_keys[0].size() > 0) {
        // !FAKE: c2 x87 code; stand-in locals tv, it, u (no PDB locals); retail holds t in st0 across the key search
        float tv = (float)t;
        if (!_loop) {
            if (_keys[0][0].time[0] > tv) {
                x = _keys[0][0].coeff[3];
            } else {
                std::vector<FFlyCubicKey, std::allocator<FFlyCubicKey> >::const_iterator it;
                for (it = _keys[0].begin(); it != _keys[0].end(); ++it) {
                    if (it->time[1] >= tv) {
                        float u = (tv - it->time[0]) / (it->time[1] - it->time[0]);
                        x = ((u * it->coeff[0] + it->coeff[1]) * u + it->coeff[2]) * u + it->coeff[3];
                        break;
                    }
                }
                if (it == _keys[0].end()) {
                    const FFlyCubicKey& k = _keys[0][_keys[0].size() - 1];
                    x = k.coeff[3] + k.coeff[2] + k.coeff[1] + k.coeff[0];
                }
            }
        }
    }
    float y = 0.0f;
    if (_keys[1].size() > 0) {
        // !FAKE: c2 x87 code; stand-in locals tv, it, u (no PDB locals); retail holds t in st0 across the key search
        float tv = (float)t;
        if (!_loop) {
            if (_keys[1][0].time[0] > tv) {
                y = _keys[1][0].coeff[3];
            } else {
                std::vector<FFlyCubicKey, std::allocator<FFlyCubicKey> >::const_iterator it;
                for (it = _keys[1].begin(); it != _keys[1].end(); ++it) {
                    if (it->time[1] >= tv) {
                        float u = (tv - it->time[0]) / (it->time[1] - it->time[0]);
                        y = ((u * it->coeff[0] + it->coeff[1]) * u + it->coeff[2]) * u + it->coeff[3];
                        break;
                    }
                }
                if (it == _keys[1].end()) {
                    const FFlyCubicKey& k = _keys[1][_keys[1].size() - 1];
                    y = k.coeff[3] + k.coeff[2] + k.coeff[1] + k.coeff[0];
                }
            }
        }
    }
    float z = 0.0f;
    if (_keys[2].size() > 0) {
        // !FAKE: c2 x87 code; stand-in locals tv, it, u (no PDB locals); retail holds t in st0 across the key search
        float tv = (float)t;
        if (!_loop) {
            if (_keys[2][0].time[0] > tv) {
                z = _keys[2][0].coeff[3];
            } else {
                std::vector<FFlyCubicKey, std::allocator<FFlyCubicKey> >::const_iterator it;
                for (it = _keys[2].begin(); it != _keys[2].end(); ++it) {
                    if (it->time[1] >= tv) {
                        float u = (tv - it->time[0]) / (it->time[1] - it->time[0]);
                        z = ((u * it->coeff[0] + it->coeff[1]) * u + it->coeff[2]) * u + it->coeff[3];
                        break;
                    }
                }
                if (it == _keys[2].end()) {
                    const FFlyCubicKey& k = _keys[2][_keys[2].size() - 1];
                    z = k.coeff[3] + k.coeff[2] + k.coeff[1] + k.coeff[0];
                }
            }
        }
    }
    const Vector3& mn = *_min;
    const Vector3& mx = *_max;
    result.x = (mx.x - mn.x) * x + mn.x;
    result.y = (mx.y - mn.y) * y + mn.y;
    result.z = (mx.z - mn.z) * z + mn.z;
}

// FUNCTION: USA 0x0006E06D
// PROTOTYPE: PREVIEW 0x004BE218 identical
// NAME: proto-pdb:default.pdb
template void std::_Construct<DiskQueue::Request,DiskQueue::Request>(DiskQueue::Request *,DiskQueue::Request const &);

// FUNCTION: USA 0x0006EBCA
// PROTOTYPE: PREVIEW 0x0045AE77 identical
// NAME: proto-pdb:default.pdb
// VC 7.1 <vector> body verbatim as an explicit specialization: the header's _Xlen cannot be redeclared noreturn
// (C2381), so `__assume(0)` restates its throw body's effect (C0237, !FAKE) and no throw-info is emitted
template <> bool std::vector<FFlyCtrlData::Key,std::allocator<FFlyCtrlData::Key> >::_Buy(size_type _Capacity)
{
    _Myfirst = 0, _Mylast = 0, _Myend = 0;
    if (_Capacity == 0)
        return (false);
    else if (max_size() < _Capacity) {
        _Xlen();
        __assume(0); // !FAKE: noreturn restated (C0237)
    } else {
        _Myfirst = this->_Alval.allocate(_Capacity);
        _Mylast = _Myfirst;
        _Myend = _Myfirst + _Capacity;
    }
    return (true);
}

// FUNCTION: USA 0x0006EFD6
// PROTOTYPE: PREVIEW 0x0045B283 identical
// NAME: proto-pdb:default.pdb
void FFlyLinearCurveCtrl::Set(const FFlyCtrlData& data)
{
    _loop = data._loop;
    _keys = data._keys[0];
}

// FUNCTION: USA 0x0006EFEF
// PROTOTYPE: PREVIEW 0x0045B29C identical
// NAME: proto-pdb:default.pdb
void FFlyCubicCurveCtrl::Set(const FFlyCtrlData& data)
{
    _loop = data._loop;
    GenerateCubicCoeff(data._keys[0], _keys);
}

// FUNCTION: USA 0x0006F00B
// PROTOTYPE: PREVIEW 0x0045B2B8 identical
// NAME: proto-pdb:default.pdb
void FFlyLinearCurveCtrlV3::Set(const FFlyCtrlData& data)
{
    _loop = data._loop;
    _keys[0] = data._keys[0];
    _keys[1] = data._keys[1];
    _keys[2] = data._keys[2];
}

// FUNCTION: USA 0x0006F042
// PROTOTYPE: PREVIEW 0x0045B2EF identical
// NAME: proto-pdb:default.pdb
void FFlyCubicCurveCtrlV3::Set(const FFlyCtrlData& data)
{
    _loop = data._loop;
    FFlyController::GenerateCubicCoeff(data._keys[0], _keys[0]);
    FFlyController::GenerateCubicCoeff(data._keys[1], _keys[1]);
    FFlyController::GenerateCubicCoeff(data._keys[2], _keys[2]);
}
