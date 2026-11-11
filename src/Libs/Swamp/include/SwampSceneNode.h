// HEADER: named for PDB class SwampSceneNode; no file recorded
// Partial: declares only what matched units use so far.
#ifndef SWAMPSCENENODE_H
#define SWAMPSCENENODE_H

#include "../../../include/decomp.h"
#include "../../SwampShared/SwampVector.h"
#include "../../SwampShared/SwampString.h"

struct AABox;  // NAME: proto-pdb:default.pdb
struct Matrix; // NAME: proto-pdb:default.pdb
class SwampScript; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum SwampSceneNodeTypes { // NAME: proto-pdb:default.pdb
        UI = 0x9
    };
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        DontDraw = 0x2
    };
    void SetOption(unsigned int option, bool value, bool recursive);
    void RemoveTreeFromScene();
    void AddChild(SwampSceneNode* node);
    SwampScript* GetScript() const;
    void SetScript(const SwampString& name);
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3& pos);             // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
protected:
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20
public:

    const Vector3& SetLocation(const Vector3&);
    const Quaternion& SetRotation(const Quaternion& q);
    SwampSceneNode* GetParent() { return _Parent; }

    int _Type;                         // 0x04
    char field_8[0x24 - 0x8];          // 0x08
    SwampSceneNode* _Parent;           // 0x24
    char field_28[0xA4 - 0x28];        // 0x28
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

#endif
