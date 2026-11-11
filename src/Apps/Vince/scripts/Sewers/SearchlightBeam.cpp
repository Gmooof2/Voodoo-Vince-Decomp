// UNIT: src/Apps/Vince/scripts/Sewers/SearchlightBeam.cpp OBJ: Xbox\OfficialRelease\SearchlightBeam.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#include "SearchlightBeam.h"
#include "../../../../Libs/Swamp/include/Swamp.h"
#include "../../../../Libs/Swamp/include/SwampGatorNode.h"

// FUNCTION: USA 0x000AB4F9
// NAME: proto-pdb:default.pdb
SearchlightBeam::SearchlightBeam(int parentBone, const Quaternion& baseRotation)
    : FogZone(g_swamp->GetModelManager().Load("suspendedLight_base"), Vector3(0.8f, 1.0f, 0.5f), 1.25f),
      _parentBone(parentBone), _baseRotation(baseRotation)
{
    _destAlphaBlend = 1;
    _Type = UI;
}

// FUNCTION: USA 0x000AB463
// PROTOTYPE: PREVIEW 0x00496285 identical
// NAME: proto-pdb:default.pdb
void SearchlightBeam::Update()
{
    if (_parentBone >= 0) {
        Vector3 pos;
        Quaternion rot;
        static_cast<GatorNode*>(GetParent())->GetBoneOrientation(_parentBone, rot, pos);
        // !FAKE: inliner; hand form of the inline Quaternion operator* (copy, *=, by-value result); no before/after recorded
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

// FUNCTION: USA 0x000AB4C7
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the inline ??1FogZone@@UAE@XZ in this unit (its users, the
// SearchlightBeam ctor and the folded ??_G, are unmatched); this qualified call odr-uses it (matched_set [[emitter]] row)
void pin_emit_FogZone_dtor(FogZone* p)
{
    p->FogZone::~FogZone();
}
