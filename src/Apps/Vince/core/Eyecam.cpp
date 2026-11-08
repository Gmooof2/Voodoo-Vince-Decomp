// UNIT: src/Apps/Vince/core/Eyecam.cpp OBJ: core\XBox\OfficialRelease\Eyecam.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"
// EyeCam: the PDB has two records; the first (size 0x13C, _ui 0x128) matches retail's offsets.

struct Matrix; // NAME: proto-pdb:default.pdb
struct Vector3; // NAME: proto-pdb:default.pdb
struct Quaternion; // NAME: proto-pdb:default.pdb
struct AABox; // NAME: proto-pdb:default.pdb
class SwampSceneNode; // NAME: proto-pdb:default.pdb
class Critter; // NAME: proto-pdb:default.pdb

namespace std { // NAME: proto-pdb:default.pdb
template <class _Ty> class allocator {
};
template <class _Ty, class _Ax = allocator<_Ty> > class vector {
public:
    class const_iterator {
    public:
        const_iterator(_Ty* _Ptr) { _Myptr = _Ptr; }
        const _Ty& operator*() const { return (*_Myptr); }
        const_iterator& operator++() { ++_Myptr; return (*this); }
        const_iterator& operator+=(int _Off) { _Myptr += _Off; return (*this); }
        const_iterator& operator-=(int _Off) { return (*this += -_Off); }
        bool operator==(const const_iterator& _Right) const { return (_Myptr == _Right._Myptr); }
        bool operator!=(const const_iterator& _Right) const { return (!(*this == _Right)); }

        _Ty* _Myptr;                   // 0x00
    };
    class iterator : public const_iterator {
    public:
        iterator(_Ty* _Ptr) : const_iterator(_Ptr) {}
        _Ty& operator*() const { return ((_Ty&)**(const_iterator*)this); }
        iterator& operator++() { ++(*(const_iterator*)this); return (*this); }
        iterator& operator-=(int _Off) { (*(const_iterator*)this) -= _Off; return (*this); }
        iterator operator-(int _Off) const { iterator _Tmp = *this; return (_Tmp -= _Off); }
    };
    vector() : _Myfirst(0), _Mylast(0), _Myend(0) {}
    ~vector() { _Tidy(); }
    void push_back(const _Ty& _Val);
    iterator begin() { return (iterator(_Myfirst)); }
    iterator end() { return (iterator(_Mylast)); }
    _Ty& back() { return (*(end() - 1)); }
protected:
    void _Tidy();
private:
    _Ax _Alval;                        // 0x00
    _Ty* _Myfirst;                     // 0x04
    _Ty* _Mylast;                      // 0x08
    _Ty* _Myend;                       // 0x0C
};
template <class _Ty, class _Container = vector<_Ty> > class stack {
public:
    _Ty& top() { return (c.back()); }
protected:
    _Container c;                      // 0x00
};
}

class SwampSceneNodeStack { // NAME: proto-pdb:default.pdb
public:
    void ReturnToPool() { _inUse = false; }

    std::stack<SwampSceneNode*, std::vector<SwampSceneNode*, std::allocator<SwampSceneNode*> > > _stack; // 0x00
    bool _inUse;                       // 0x10
};
DECOMP_SIZE_ASSERT(SwampSceneNodeStack, 0x14);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        DontDraw = 0x2
    };
    class iterator { // NAME: proto-pdb:default.pdb
    public:
        iterator();
        iterator(SwampSceneNode* node, int type);
        ~iterator() { _nodeStack->ReturnToPool(); }
        int operator!=(const iterator& other);
        SwampSceneNode*& operator*() { return _nodeStack->_stack.top(); }
        iterator& operator++();

        int _type;                     // 0x00
        SwampSceneNodeStack* _nodeStack; // 0x04
    };
    iterator begin(int type) const { return iterator(const_cast<SwampSceneNode*>(this), type); }
    iterator end() const { return iterator(); }

    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
protected:
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20
public:

    void RemoveTreeFromScene();
    void SetOption(unsigned int option, bool value, bool recursive);
    bool GetOption(unsigned int option) const { return (_RenderingOptions & option) != 0; }
    void SetVisible(bool visible, bool recursive);
    bool IsVisible() const { return !GetOption(DontDraw); }

    char field_4[0x88 - 0x4];          // 0x04
    unsigned int _RenderingOptions;    // 0x88
    char field_8C[0xA4 - 0x8C];        // 0x8C
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class UINode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    UINode();
    virtual void Update();
    virtual void Render(const Matrix& m) const;

    char field_A4[0xD4 - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(UINode, 0xD4);

class EyeUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    EyeUI();
    virtual ~EyeUI();
    virtual void Update();
    virtual void Render(const Matrix& m) const;

    std::vector<const Critter*, std::allocator<const Critter*> > _items; // 0xD4
    std::vector<const SwampSceneNode*, std::allocator<const SwampSceneNode*> > _invisibleHUDs; // 0xE4
};
DECOMP_SIZE_ASSERT(EyeUI, 0xF4);

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0x10C - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(SwampCamera, 0x10C);

class GameCam : public SwampCamera { // NAME: proto-pdb:default.pdb
public:
    bool IsCurrentCamera() const { return _isCurrentCamera; }

    char field_10C[0x118 - 0x10C];     // 0x10C
    bool _isCurrentCamera;             // 0x118
    char field_119[0x11C - 0x119];     // 0x119
};
DECOMP_SIZE_ASSERT(GameCam, 0x11C);

class AnimCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    char field_11C[0x128 - 0x11C];     // 0x11C
};
DECOMP_SIZE_ASSERT(AnimCam, 0x128);

class EyeCam : public AnimCam { // NAME: proto-pdb:default.pdb
public:
    virtual void Render(const Matrix& m) const;

private:
    void ReleaseUI() const;

    mutable EyeUI* _ui;                // 0x128
    char field_12C[0x13C - 0x12C];     // 0x12C
};
DECOMP_SIZE_ASSERT(EyeCam, 0x13C);

// FUNCTION: USA 0x001369AE
// PROTOTYPE: PREVIEW 0x00520B74 identical
// NAME: proto-pdb:default.pdb
void EyeCam::ReleaseUI() const
{
    if (_ui) {
        _ui->RemoveTreeFromScene();
        _ui->Invalidate();
        _ui = 0;
    }
}

// FUNCTION: USA 0x00136A4A
// PROTOTYPE: PREVIEW 0x00520C33 identical
// NAME: proto-pdb:default.pdb
void EyeCam::Render(const Matrix& m) const
{
    if (!IsCurrentCamera() && _ui) {
        ReleaseUI();
    }
}

class RootNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class SwampScene { // NAME: proto-pdb:default.pdb
public:
    RootNode* GetRootNode() const { return _root; }

    RootNode* _root;                   // 0x00
    char field_4[0x14 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampScene, 0x14);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0xC];                 // 0x00
    SwampScene* _scene;                // 0x0C
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x00136CEE
// PROTOTYPE: PREVIEW 0x00520F1A identical
// NAME: proto-pdb:default.pdb
EyeUI::~EyeUI()
{
    for (SwampSceneNode::iterator hit = g_swamp->_scene->GetRootNode()->begin(9);
         hit != g_swamp->_scene->GetRootNode()->end(); ++hit) {
        if (*hit != this) {
            // !FAKE: inliner; hand form of a find loop with a dummy flag local; std::find stays out of line under /O1;
            // retail branches from the hit straight past SetOption (match_one 76.84 call -> 87.86 -> 98.21 -> 100.0
            // with the PDB begin/end)
            std::vector<const SwampSceneNode*, std::allocator<const SwampSceneNode*> >::iterator v0 = _invisibleHUDs.begin();
            bool v1 = false;
            for (; v0 != _invisibleHUDs.end(); ++v0) {
                if (*v0 == *hit) {
                    v1 = true;
                    break;
                }
            }
            if (!v1)
                (*hit)->SetOption(SwampSceneNode::DontDraw, false, false);
        }
    }
}

// FUNCTION: USA 0x00137308
// PROTOTYPE: PREVIEW 0x00521534 identical
// NAME: proto-pdb:default.pdb
EyeUI::EyeUI()
{
    for (SwampSceneNode::iterator hit = g_swamp->_scene->GetRootNode()->begin(9);
         hit != g_swamp->_scene->GetRootNode()->end(); ++hit) {
        if ((*hit)->IsVisible()) {
            // !FAKE: inliner; hand form of the inlined SwampSceneNode::SetVisible(false, false); its in-class body emits a
            // COMDAT named like CrawdadJimmys' matched row 0x00019DA1 (relink LNK2005), as in Hannah.cpp
            (*hit)->SetOption(SwampSceneNode::DontDraw, true, false);
        } else {
            _invisibleHUDs.push_back(*hit);
        }
    }
}
