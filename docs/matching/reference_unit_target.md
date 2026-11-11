# Reference unit, target form (5.5 T17.1; preview, not compiled, not in force)

What SearchlightBeam would look like once the tool and rule changes below land (Gen 6). The source in `src/` is the
form in force (`docs/matching/reference_unit.md`). Nothing here is built or checked by the gate; the code blocks are
the proposal, from the T17 deep-thinker's reading (`.run/5.5/t17_thinker/answer.md`), corrected where its tool
pointers were wrong. Terms: `reference_unit.md` "Glossary".

## SearchlightBeam.cpp (target)
```cpp
// NAMES: proto-pdb:default.pdb
#include "include/decomp.h"
#include "Apps/Vince/scripts/Sewers/SearchlightBeam.h"
#include "Libs/Swamp/include/Swamp.h"
#include "Libs/Swamp/include/SwampGatorNode.h"

// FUNCTION: USA 0x000AB4F9
SearchlightBeam::SearchlightBeam(int parentBone, const Quaternion& baseRotation)
    : FogZone(g_swamp->GetModelManager().Load("suspendedLight_base"), Vector3(0.8f, 1.0f, 0.5f), 1.25f),
      _parentBone(parentBone), _baseRotation(baseRotation)
{
    _destAlphaBlend = 1;
    _Type = UI;
}

// FUNCTION: USA 0x000AB463
// PROTOTYPE: PREVIEW 0x00496285 identical
void SearchlightBeam::Update()
{
    if (_parentBone >= 0) {
        Vector3 pos;
        Quaternion rot;
        static_cast<GatorNode*>(GetParent())->GetBoneOrientation(_parentBone, rot, pos);

        // !FAKE: inliner; long-hand Quaternion product; no operator* in the PDB, five other forms tried (best 98.61)
        // Stands in for: SetRotation(rot * _baseRotation);   v0 and v1 are our names.
        Quaternion v1;
        {
            Quaternion v0 = rot;
            v0 *= _baseRotation;
            v1 = v0;
        }
        SetRotation(v1);
        SetLocation(pos);
    }
}
```

## SearchlightBeam.h (target)
```cpp
// HEADER: named for PDB class SearchlightBeam; no file recorded
// NAMES: proto-pdb:default.pdb
// Partial: declares only what matched units use so far.
#ifndef SEARCHLIGHTBEAM_H
#define SEARCHLIGHTBEAM_H

#include "include/decomp.h"
#include "Libs/SwampShared/SwampVector.h"
#include "Libs/Swamp/include/FogZone.h"

// A FogZone that follows one bone of its parent node (used as a GatorNode): Update copies the bone's position
// and sets the rotation to the bone's rotation times _baseRotation. With a negative _parentBone, Update does nothing.
// SearchlightBeam: the PDB has two class records (0x148 with _lightSound, 0x144 without); retail's
// ctor ??0SearchlightBeam@@QAE@HABUQuaternion@@@Z matches the later 0x144 record, used here.
class SearchlightBeam : public FogZone {
public:
    SearchlightBeam(int parentBone, const Quaternion& baseRotation);
    virtual void Update();                                         // vtable+0x1C

    int _parentBone;                   // 0x130
    Quaternion _baseRotation;          // 0x134
};
DECOMP_SIZE_ASSERT(SearchlightBeam, 0x144);

#endif
```

## FogZone.h (target)
```cpp
// HEADER: proto-pdb:default.pdb file libs\swamp\include\fogzone.h
// NAMES: proto-pdb:default.pdb
// Partial: declares only what matched units use so far.
#ifndef FOGZONE_H
#define FOGZONE_H

#include "include/decomp.h"
#include "Libs/SwampShared/SwampVector.h"
#include "Libs/SwampShared/SwampResource.h"
#include "Libs/Swamp/include/SwampSceneNode.h"

class FogZone : public SwampSceneNode {
public:
    FogZone(const ResourceHandle<ModelResource>& volume, const Vector3& color, float visibilityDistance);
    // FUNCTION: USA 0x000AB4C7
    virtual ~FogZone() {}                                          // vtable+0x00
    virtual void Render(const Matrix&) const;                      // vtable+0x20

    ResourceHandle<ModelResource> _model;          // 0xA4
    Vector3 _points[8];                // 0xA8
    char field_108[0x118 - 0x108];     // 0x108  PDB: Vector4 _color; Vector4 not declared yet
    unsigned long _destAlphaBlend;     // 0x118
    float _visibilityDistance;         // 0x11C
    float _noiseSpeed;                 // 0x120
    float _noiseScale;                 // 0x124
    ResourceHandle<PixelShaderResource> _pshader;  // 0x128
    ResourceHandle<VertexShaderResource> _vshader; // 0x12C
};
DECOMP_SIZE_ASSERT(FogZone, 0x130);

#endif
```

## Each difference from `src/`, and what it needs (Gen 6)
Already in `src/` since T17.1 (no tool needed): the class summary instead of 3 per-function doc comments, the
`Partial:` line in all 8 headers, `field_108`'s `_color` note, `_Type = UI` (PDB `SwampSceneNode::SwampSceneNodeTypes`
0x9), FogZone's ctor parameter names `volume`, `color`, `visibilityDistance` (PDB locals of the ctor).

Still different:
1. `// UNIT:` line dropped. Needs: `tools/unit_header.py` (`--check`/`--write`, CONVENTIONS §1 :7-12) retired or
   changed, `tools/readability_census.py:47` (its ANNOTATION pattern counts the line) and `gen_creationfunc.py:184`
   (writes it) changed, CONVENTIONS §1 rewritten. The line copies `config/retail/units.toml`; the gate does not read
   it (CONVENTIONS §1). The T17 thinker said no tool reads it; that is wrong (unit_header.py, readability_census.py).
2. One `// NAMES: proto-pdb:default.pdb` line per file instead of `// NAME:` on every name; a `NAME` tag stays only
   where a name's source differs. Needs: a developer decision amending D42 (the per-name tag) in the light of D108;
   `annotations.py:30,49-51` (FUNCTION must be followed by a NAME line) and provenance_lint's proto-pdb check (prove
   each name by lookup under the file line); readability_census counts.
3. Includes from `src/` instead of `../../../../`. Needs: `build.py` passes one `/I src`; a gate run proving no byte
   moves (a header inline that uses `__FILE__` would change its string with the spelling; prove none is affected).
4. `// PROTOTYPE: PREVIEW` kept in the target, against the thinker's `// FUNCTION: PREVIEW`. CONVENTIONS §1 (:26-31)
   chose the word `PROTOTYPE` on purpose, so no reccmp-style parser takes the prototype address for a match target.
   Changing it needs that rule changed, plus `annotations.py:29,47-48`, `gen_registerme.py:123`,
   `gen_creationfunc.py:171`. Recommendation: keep the word; the glossary explains it.
5. `!FAKE` reworded (the plain line it stands in for, and the evidence). Needs: provenance_lint's fake-diff (LISTED)
   to allow a text-only edit of a `!FAKE` line when the unit's bytes are unchanged; CONVENTIONS §4 adds the
   `Stands in for:` line to the spelling.
6. The `pin_emit_FogZone_dtor` scaffold gone; the dtor's `FUNCTION:` line on its inline body in FogZone.h. Needs:
   `matched_set.py remove --emitter` (LISTED; the `[[emitter]]` row goes stale otherwise); annotations.py and
   provenance_lint counting a `FUNCTION:` line in a header the unit includes (annotations.py already scans `.h`,
   `EXTS` :28; whether that line then counts for the unit's obj is for the change to prove). Without the scaffold
   `~FogZone` is still emitted at 100.0 (T17 trial; the matched ctor odr-uses it).
7. `NAME` tags on class lines dropped (covered by item 2's file line).
