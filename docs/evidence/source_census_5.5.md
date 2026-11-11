# Source census 5.5 (S4, S5, S7, I43, I52, S6, module vs unit, S10)

T3.c1, phase 5.5. Read-only census of `src/` against `default.pdb` at 9501e9c60 (working tree; nothing under src/,
tools/ or config/ changed). Script (scratch, not kept in the repo): `.run/5.5/t3/census_src.py`, which imports
`tools/provenance_lint.py` (Pdb, inputs, matched_vas, scoped_files, bodies, lines_of, code_part, has_fake, NEUTRAL,
pdb_local_names, obj_procs, locals_obj, use_line, demangle_scope, symbol_starts) and edits nothing. Reproduce:
`PY .run/5.5/t3/census_src.py` (exit 0; prints the totals below). D46: file:line, our identifiers and counts only.

Scope: 219 source files under `src/` (S4, S7, I43); the 214 scoped files (files holding a matched `// FUNCTION:`
VA, `provenance_lint.scoped_files`) for S5 and I52.

## 1. S4: names declared in src/ absent from default.pdb

Rule: D78 (no invented structure: no field, type or name absent from the prototype PDB, the retail disassembly or
retail strings).

Method: each file is cleaned (comments and literals blanked, preprocessor lines blanked, `#if 0` and
NON_MATCHING-defined arms blanked, i.e. the matching build's view) and scanned by a brace-tracking statement parser.
Inside `class|struct|union` bodies each `;`-ended member is a data member, a method (inline-defined methods
included), or a nested typedef; `enum {...}` lists give enumerators; `typedef` at namespace scope gives typedefs;
the named parameters of `;`-ended method and free-function declarations give declaration parameters. Checks:
data member / method / nested typedef: the name is a member of the owner's PDB fieldlist (owner = enclosing
namespaces + classes, template args stripped; operator names compared as written, `operator X` also as
`operator struct|class X`) or `Pdb.resolves(owner::name)`; enumerator: a name in any LF_ENUM fieldlist; typedef:
an S_UDT name (global or module streams), or a PDB type / scope name; parameter: a local (S_BPREL32/REGREL32/
REGISTER, params included) of a PDB proc named `owner::method` (all overloads merged); no such proc = `noproc`,
not absent.

Statuses: `absent` = name nowhere in the PDB name sets (for params: not a local of the proc); `absent-elsewhere` =
the name exists in default.pdb but not as a member of this owner (e.g. a base-class member flattened into the
derived class); `absent-owner` = the owner has no PDB fieldlist under the qualified name declared (e.g. a nested
`_Node` declared in `std::list` where the PDB nests it elsewhere); `placeholder` = `field_<off>` (CONVENTIONS
placeholder, not counted absent); `mgr-*` = the gen_registerme template names (`tools/gen_registerme.py:27-32`:
SwampScriptManager's CreationFunc, Instance, Add, params name, func), counted apart whatever their status.

Counts (declaration sites; a class repeated in N files counts N times):

- data: ok 7015, absent 121, absent-elsewhere 141, absent-owner 67, placeholder 2009
- method: ok 13201, absent-elsewhere 18, mgr-ok 195
- typedef: ok 109, absent 173
- typedef-nested: ok 364, absent-elsewhere 1, mgr-ok 96
- param: ok 4573, absent 490, noproc 2126, mgr-ok 96, mgr-absent 95
- enumerator: ok 1459, absent 1

Totals: absent (all three statuses) 1012 sites, 308 distinct (kind, owner, name), in 186 files. noproc params 2126 (no PDB proc of that name to check). gen_registerme names: 482 sites in 99 files, of which 95 absent (all params).

Prior R5.5-003 S4 said 100 files declare the MGR class: 100 files hold `class SwampScriptManager {`; 99 carry the
gen_registerme template; `src/Libs/Swamp/Xbox/Official_Release/SwampWorld.cpp:1304` is a fuller declaration
without those names.

Distinct absent names (kind, status, `owner::name`, sites):

- data absent `BeepTDF::_pad00` 1
- data absent `SoundNode::field_pad` 1
- data absent `Swamp::_pad00` 1
- data absent `SwampScript::_pad04` 1
- data absent `std::_Tree::_pad02` 1
- data absent `std::basic_string::_pad00` 103
- data absent `std::deque::_pad00` 7
- data absent `std::vector::_pad00` 6
- data absent-elsewhere `AABox::max` 1
- data absent-elsewhere `AABox::min` 1
- data absent-elsewhere `AnimationManager::_missingResourceMessages` 1
- data absent-elsewhere `CollidableCylinder::_collidable` 1
- data absent-elsewhere `GameCam::_Location` 1
- data absent-elsewhere `GatorNode::_height` 1
- data absent-elsewhere `GatorNode::_radius` 2
- data absent-elsewhere `Quaternion::x` 2
- data absent-elsewhere `Quaternion::y` 2
- data absent-elsewhere `Quaternion::z` 2
- data absent-elsewhere `Vector2::x` 1
- data absent-elsewhere `Vector2::y` 1
- data absent-elsewhere `std::_Tree::_Alnod` 2
- data absent-elsewhere `std::_Tree::_Alptr` 1
- data absent-elsewhere `std::_Tree::_Alval` 16
- data absent-elsewhere `std::_Tree::iterator::_Ptr` 1
- data absent-elsewhere `std::basic_string::_Alval` 44
- data absent-elsewhere `std::deque::_Almap` 3
- data absent-elsewhere `std::deque::_Alval` 6
- data absent-elsewhere `std::list::_Alnod` 2
- data absent-elsewhere `std::list::_Alval` 5
- data absent-elsewhere `std::list::iterator::_Ptr` 7
- data absent-elsewhere `std::vector::_Alval` 34
- data absent-elsewhere `std::vector::iterator::_Myptr` 3
- data absent-elsewhere `std::vector::iterator::current` 1
- data absent-owner `std::_Tree::_Node::_Color` 4
- data absent-owner `std::_Tree::_Node::_Isnil` 4
- data absent-owner `std::_Tree::_Node::_Left` 8
- data absent-owner `std::_Tree::_Node::_Myval` 8
- data absent-owner `std::_Tree::_Node::_Parent` 8
- data absent-owner `std::_Tree::_Node::_Right` 8
- data absent-owner `std::list::_Node::_Myval` 9
- data absent-owner `std::list::_Node::_Next` 9
- data absent-owner `std::list::_Node::_Prev` 9
- enumerator absent `std::basic_string::npos` 1
- method absent-elsewhere `CollidableCylinder::SetCollidable` 1
- method absent-elsewhere `GatorNode::CanJorgeCollideWithMe` 1
- method absent-elsewhere `GatorNode::CheckLine` 1
- method absent-elsewhere `GatorNode::DoesJorgeRepelMe` 1
- method absent-elsewhere `ResourceHandle::operator->` 2
- method absent-elsewhere `std::_Tree::iterator::operator!=` 1
- method absent-elsewhere `std::_Tree::iterator::operator==` 1
- method absent-elsewhere `std::list::iterator::operator!=` 4
- method absent-elsewhere `std::list::iterator::operator==` 2
- method absent-elsewhere `std::vector::iterator::operator!=` 3
- method absent-elsewhere `std::vector::iterator::operator==` 1
- param absent `AABox::AddPoint::pt` 1
- param absent `AABox::AddPointPair::pts` 1
- param absent `Airplane_RocketLabMain::TimerEvent::timerId` 1
- param absent `AnimCam::Return::blend` 1
- param absent `AnimCam::SwitchTo::blend` 1
- param absent `AnimCam::SwitchTo::delay` 1
- param absent `AnimationControl::Update::secondary` 1
- param absent `AreaLoadLocatorMain::FinishedLevelIntro::script` 1
- param absent `AsyncFileHandler::AsyncFileHandler::desiredsize` 1
- param absent `AsyncFileHandler::Seek::pos` 1
- param absent `AsyncFileHandler::SeekFromEnd::pos` 1
- param absent `AsyncFileHandler::SeekRelative::pos` 1
- param absent `BSP::BSP::b` 1
- param absent `BSP::BSP::data` 1
- param absent `BeepFile::Read::buffer` 2
- param absent `BeepFile::Read::size` 2
- param absent `BeepFile::Seek::pos` 1
- param absent `BeepFile::SeekRelative::pos` 1
- param absent `BeepFile::Write::buf` 1
- param absent `BeepFile::Write::size` 1
- param absent `BeepFree::ptr` 1
- param absent `BeepLib::RandomFloat::hi` 1
- param absent `BeepLib::RandomFloat::lo` 1
- param absent `BeepLib::StripExtension::path` 1
- param absent `BeepLib::StripPath::path` 3
- param absent `BeepTDF::BeepTDF::file` 1
- param absent `BeepTDF::BeepTDF::name` 2
- param absent `BeepTDF::FindBlock::name` 2
- param absent `BeepTDF::FindBlock::recurse` 2
- param absent `BeepTDF::GetIntRecord::name` 1
- param absent `CameraManager::Find::name` 3
- param absent `ClownMissileMain::CollidedWith::critterHitBy` 1
- param absent `CollidableCylinder::CollidableCylinder::type` 1
- param absent `CollidableNode::CollidableNode::type` 1
- param absent `CritterManager::Find::name` 13
- param absent `CritterManager::FindAllMatching::matches` 2
- param absent `CritterManager::FindAllMatching::name` 2
- param absent `CutsceneDriver::AnimationTrigger::a` 2
- param absent `CutsceneDriver::AnimationTrigger::b` 2
- param absent `CutsceneDriver::AnimationTrigger::c` 2
- param absent `CutsceneDriver::SetState::state` 2
- param absent `CutsceneDriver::StateTransition::anim` 1
- param absent `CutsceneDriver::StateTransition::state` 3
- param absent `D3DDevice_CreateIndexBuffer2::length` 1
- param absent `D3DDevice_CreateVertexBuffer2::length` 4
- param absent `D3DDevice_DrawIndexedVertices::indexData` 2
- param absent `D3DDevice_DrawIndexedVertices::primitiveType` 2
- param absent `D3DDevice_DrawIndexedVertices::vertexCount` 2
- param absent `D3DDevice_SetIndices::pIndexData` 1
- param absent `D3DDevice_SetRenderState_StencilEnable::value` 2
- param absent `D3DDevice_SetRenderState_StencilFail::value` 1
- param absent `D3DDevice_SetRenderTarget::pNewZStencil` 2
- param absent `D3DResource_AddRef::pThis` 1
- param absent `D3DResource_Register::pThis` 3
- param absent `D3DResource_Release::pThis` 1
- param absent `D3DVertexBuffer_Lock2::flags` 4
- param absent `D3DVertexBuffer_Lock2::vb` 4
- param absent `D3DXGetErrorStringA::buffer` 5
- param absent `D3DXGetErrorStringA::size` 5
- param absent `D3DXLoadSurfaceFromSurface::Filter` 1
- param absent `FFlyController::GenerateCubicCoeff::cubicKeys` 1
- param absent `FFlyController::GenerateCubicCoeff::keys` 1
- param absent `FFlyEmitter::GetRenderColor::color` 1
- param absent `FFlyEmitter::GetRenderTex::tex` 1
- param absent `FFlyEmitter::SetSurface::surface` 1
- param absent `FFlyLinearCurveCtrlV3::Evaluate::v` 1
- param absent `FastReciprocalSquareRoot::f` 2
- param absent `FileHandler::GetFileData::name` 1
- param absent `FileSystem::FindFileInBundles::name` 1
- param absent `FogZone::FogZone::tdf` 1
- param absent `Frustum::CreateEmptyInverse::f` 1
- param absent `Frustum::IsVisible::box` 1
- param absent `Frustum::IsVisible::tolerance` 1
- param absent `Frustum::TrimFrustum::eye` 1
- param absent `Frustum::TrimFrustum::f` 1
- param absent `GameCam::SetShakeFactor::a` 1
- param absent `GameCam::SetShakeFactor::b` 1
- param absent `GatesRegionMain::AddFrog::name` 1
- param absent `GatorNode::GetBoneOrientation::bone` 2
- param absent `GatorNode::GetBoneOrientation::index` 1
- param absent `GatorNode::GetBoneOrientation::name` 1
- param absent `GatorNode::OverrideTexture::name` 1
- param absent `GatorNode::OverrideTexture::newName` 1
- param absent `GatorNode::StartAnimation::a` 10
- param absent `GatorNode::StartAnimation::b` 10
- param absent `GatorNode::StartAnimation::loop` 5
- param absent `GatorNode::StartAnimation::restart` 5
- param absent `HUDUI::SetSubtitle::length` 1
- param absent `HUDUI::SetSubtitle::ptr` 1
- param absent `HeatShimmer::HeatShimmer::radius` 1
- param absent `HottieBundle::CloseBundledFile::file` 1
- param absent `HottieBundle::OpenBundledFile::name` 2
- param absent `IDirectSoundBuffer::SetPosition::apply` 1
- param absent `LevelScript::DoProgressHint::hint` 4
- param absent `LevelScript::TimerEvent::id` 4
- param absent `LightManager::CreateDefinedLight::tdf` 1
- param absent `LightManager::CreateDirectionalLight::dir` 1
- param absent `LightManager::CreateGatorLight::info` 1
- param absent `LightNode::GenerateShadowBuffer::center` 1
- param absent `LightNode::GenerateShadowBuffer::range` 1
- param absent `LightNode::StartPlayback::a` 1
- param absent `LightNode::StartPlayback::anim` 1
- param absent `LightNode::StartPlayback::b` 1
- param absent `LocalizedStrings::GetGameString::id` 1
- param absent `LocatorManager::Find::name` 2
- param absent `MainstreetRegionMain::DoProgressHint::hint` 1
- param absent `MainstreetRegionMain::Message::source` 1
- param absent `MaterialManager::CreateMaterial::basename` 1
- param absent `MaterialManager::CreateMaterial::i` 1
- param absent `MaterialManager::CreateMaterial::tdf` 1
- param absent `MaterialResource::MaterialResource::tdf` 1
- param absent `Matrix::Apply::v` 1
- param absent `Matrix::ApplyNormal::v` 1
- param absent `Matrix::BatchApply::in` 1
- param absent `Matrix::BatchApply::out` 1
- param absent `Matrix::Scaling::s` 1
- param absent `MemoryFileHandler::GetFileData::name` 1
- param absent `MemoryFileHandler::Seek::pos` 1
- param absent `MemoryFileHandler::SeekFromEnd::pos` 1
- param absent `MemoryFileHandler::SeekRelative::pos` 1
- param absent `ModelResource::GetBoneIndex::b` 1
- param absent `ModelResource::GetBoneIndex::name` 4
- param absent `ModelResource::GetBoneOrientation::deform` 1
- param absent `MoleGateMain::Message::source` 1
- param absent `MoviePlayer::MoviePlayer::audio` 1
- param absent `MoviePlayer::MoviePlayer::name` 1
- param absent `MultiAnimation::Load::format` 1
- param absent `MultiAnimation::Load::name` 1
- param absent `MultiAnimation::Load::start` 1
- param absent `NeedleHat::SetVisible::visible` 1
- param absent `ParticleRenderer::ParticleRenderer::ffNode` 1
- param absent `Quaternion::Apply::in` 2
- param absent `Quaternion::Apply::out` 2
- param absent `Quaternion::Apply::r` 1
- param absent `Quaternion::Apply::v` 1
- param absent `Quaternion::Set::x` 1
- param absent `Quaternion::Set::y` 1
- param absent `Quaternion::Set::z` 1
- param absent `RELEASE::p` 5
- param absent `RegionManager::Create::block` 1
- param absent `RegionManager::Create::className` 1
- param absent `RegionManager::Create::instanceName` 1
- param absent `RegionManager::NotifyMatchingRegions::prefix` 1
- param absent `RegionTrigger::Save::str` 1
- param absent `RegionTrigger::TestLineSegmentCollision::testEndOnly` 1
- param absent `ResourceHandle::ResourceHandle::other` 1
- param absent `ResourceHandle::operator=::other` 6
- param absent `ResourceHandle::operator=::rh` 1
- param absent `ResourceManager::GetDataInBundle::bundle` 1
- param absent `ResourceManager::GetDataInBundle::name` 1
- param absent `ResourceManager::GetTDFInBundle::name` 1
- param absent `RumbleResource::GenSamples::channel` 1
- param absent `RumbleResource::GenSamples::count` 1
- param absent `RumbleResource::GenSamples::motor` 1
- param absent `RumbleResource::GenerateSamples::loop` 1
- param absent `ScareHouseMonsterMain::TimerEvent::id` 1
- param absent `SoundManager::SetCinematicVolume::v` 1
- param absent `SoundManager::SetMusicVolume::v` 1
- param absent `SoundManager::SetSoundVolume::v` 1
- param absent `Sprite::SetTexture::name` 1
- param absent `StandardFileHandler::Seek::pos` 1
- param absent `StandardFileHandler::SeekFromEnd::pos` 1
- param absent `StandardFileHandler::SeekRelative::pos` 1
- param absent `SwampGamePad::WasButtonPressed::button` 1
- param absent `SwampInput::GetPad::index` 1
- param absent `SwampNodeManager::Add::func` 3
- param absent `SwampRenderer::SetTexture::texture` 2
- param absent `SwampRenderer::SetVertexShader::format` 1
- param absent `SwampRenderer::SetVertexShader::shader` 2
- param absent `SwampScene::AddNode::flags` 1
- param absent `SwampSceneNode::AddChild::child` 3
- param absent `SwampSceneNode::SetFacing::dir` 3
- param absent `SwampSceneNode::SetLocation::location` 6
- param absent `SwampSceneNode::SetOption::a` 2
- param absent `SwampSceneNode::SetOption::b` 2
- param absent `SwampSceneNode::SetOption::on` 17
- param absent `SwampSceneNode::SetOption::recurse` 18
- param absent `SwampSceneNode::SetRenderingSetting::recurse` 5
- param absent `SwampSceneNode::SetRotation::rotation` 6
- param absent `SwampSceneNode::SetVisible::recursive` 2
- param absent `SwampSceneNode::SetVisible::visible` 2
- param absent `SwampSceneNode::iterator::iterator::node` 1
- param absent `SwampSceneNode::iterator::operator!=::other` 1
- param absent `SwampScript::AnimationTrigger::a` 1
- param absent `SwampScript::AnimationTrigger::b` 1
- param absent `SwampScript::AnimationTrigger::c` 1
- param absent `SwampScript::CollidedWith::node` 1
- param absent `SwampScript::CollidedWith::result` 1
- param absent `SwampScript::EnteringRegion::region` 1
- param absent `SwampScript::GainingControl::script` 1
- param absent `SwampScript::HitBy::node` 1
- param absent `SwampScript::HitBy::result` 1
- param absent `SwampScript::LeavingRegion::region` 1
- param absent `SwampScript::LosingControl::name` 1
- param absent `SwampScript::Message::from` 1
- param absent `SwampScript::Message::msg` 1
- param absent `SwampScript::RegisterTimerEvent::repeat` 5
- param absent `SwampScript::RegisterTimerEvent::time` 5
- param absent `SwampScript::RegisterTimerEvent::timerId` 3
- param absent `SwampScript::TimerEvent::id` 1
- param absent `SwitchMain::SetOffAnimName::name` 2
- param absent `SwitchMain::SetOnAnimName::name` 4
- param absent `TargetSwitchMain::HitBy::other` 1
- param absent `TargetSwitchMain::HitBy::result` 1
- param absent `TextureManager::CreateCubemap::size` 1
- param absent `VertexShaderManager::CreateShader::vaf` 2
- param absent `VertexShaderResource::VertexShaderResource::decl` 1
- param absent `Vince::SetTargetLocation::location` 1
- param absent `Vince::SetWhammyReady::ready` 1
- param absent `VinceCam::CopyCamera::cam` 1
- param absent `VinceCam::SetDirectionFixed::delay` 1
- param absent `VinceCam::SetPositionFixed::pos` 1
- param absent `VinceCarRide::TimerEvent::id` 1
- param absent `VinceFlat::TimerEvent::id` 1
- param absent `WaterField::RenderSetup::eMap` 1
- param absent `XLoadSectionA::pSectionName` 1
- param absent `ZipFileHandler::Seek::pos` 1
- param absent `ZipFileHandler::SeekFromEnd::pos` 1
- param absent `ZipFileHandler::SeekRelative::pos` 1
- param absent `ZoneNode::ChunkRenderSetup::mat` 1
- param absent `ZoneNode::ChunkRenderSetup::vaf` 1
- param absent `ZoneNode::ZoneNode::bogs` 1
- param absent `ZoneNode::ZoneRenderSetup::noEffects` 1
- param absent `_vsnprintf::argptr` 1
- param absent `_vsnprintf::buffer` 1
- param absent `exception::exception::v0` 1
- param absent `exception::exception::v1` 7
- param absent `fclose::stream` 1
- param absent `fopen::filename` 1
- param absent `format::fmt` 4
- param absent `fread::buf` 1
- param absent `fseek::origin` 1
- param absent `fseek::stream` 1
- param absent `ftell::stream` 1
- param absent `fwrite::buf` 1
- param absent `std::basic_string::basic_string::ptr` 62
- param absent `std::basic_string::basic_string::s` 1
- param absent `std::basic_string::compare::ptr` 1
- param absent `std::list::erase::where` 2
- param absent `std::logic_error::logic_error::v1` 2
- param absent `std::vector::push_back::val` 1
- typedef absent `BeepString` 2
- typedef absent `BeepWString` 1
- typedef absent `CloneTree` 1
- typedef absent `FloodChunkIt` 1
- typedef absent `NameTree` 1
- typedef absent `NodeMapTraits` 1
- typedef absent `ProfileTree` 1
- typedef absent `ScriptTree` 1
- typedef absent `SwampString` 154
- typedef absent `SwampWString` 8
- typedef absent `TexRemapTraits` 1
- typedef absent `TimerTree` 1
- typedef-nested absent-elsewhere `LocatorManager::Map` 1

By file (absent sites; count):

`src/Apps/Vince/Init.cpp` 3, `src/Apps/Vince/SwampD3D.cpp` 2, `src/Apps/Vince/core/Activator.cpp` 2, `src/Apps/Vince/core/AmbientBird.cpp` 2, `src/Apps/Vince/core/AnimCam.cpp` 2, `src/Apps/Vince/core/BasicDoor.cpp` 4, `src/Apps/Vince/core/Bead.cpp` 6, `src/Apps/Vince/core/Boomerang.cpp` 2, `src/Apps/Vince/core/BubbaGator.cpp` 4, `src/Apps/Vince/core/CameraManager.cpp` 10, `src/Apps/Vince/core/CarriedObject.cpp` 4, `src/Apps/Vince/core/Critter.cpp` 9, `src/Apps/Vince/core/Cutscene.cpp` 4, `src/Apps/Vince/core/DebugCam.cpp` 1, `src/Apps/Vince/core/DialogueRegion.cpp` 2, `src/Apps/Vince/core/EffectRegions.cpp` 3, `src/Apps/Vince/core/Eyecam.cpp` 5, `src/Apps/Vince/core/FanCeiling.cpp` 11, `src/Apps/Vince/core/FireRegion.cpp` 3, `src/Apps/Vince/core/FontManager.cpp` 5, `src/Apps/Vince/core/GameRegions.cpp` 2, `src/Apps/Vince/core/GoldJuneBug.cpp` 7, `src/Apps/Vince/core/HUDUI.cpp` 20, `src/Apps/Vince/core/HeavyObject.cpp` 2, `src/Apps/Vince/core/Intro.cpp` 8, `src/Apps/Vince/core/KillerDillo.cpp` 2, `src/Apps/Vince/core/LevelLoader.cpp` 5, `src/Apps/Vince/core/LevelScript.cpp` 5, `src/Apps/Vince/core/LocalizedStrings.cpp` 4, `src/Apps/Vince/core/Locator.cpp` 5, `src/Apps/Vince/core/MondoMole.cpp` 3, `src/Apps/Vince/core/Monster.cpp` 7, `src/Apps/Vince/core/MultiAnimation.cpp` 3, `src/Apps/Vince/core/Music.cpp` 10, `src/Apps/Vince/core/Particle.cpp` 2, `src/Apps/Vince/core/PitFrog.cpp` 2, `src/Apps/Vince/core/PowerIcon.cpp` 2, `src/Apps/Vince/core/SignTrolley.cpp` 11, `src/Apps/Vince/core/SwingHook.cpp` 8, `src/Apps/Vince/core/Switch.cpp` 14, `src/Apps/Vince/core/TrainingSign.cpp` 3, `src/Apps/Vince/core/Trolley.cpp` 2, `src/Apps/Vince/core/VinceCam.cpp` 3, `src/Apps/Vince/core/VinceCarryHeavy.cpp` 1, `src/Apps/Vince/core/VinceCarryLight.cpp` 4, `src/Apps/Vince/core/VinceCutScene.cpp` 2, `src/Apps/Vince/core/VinceDrinkjug.cpp` 5, `src/Apps/Vince/core/VinceFlat.cpp` 4, `src/Apps/Vince/core/VinceSwing.cpp` 3, `src/Apps/Vince/core/VinceWalkIn.cpp` 3, `src/Apps/Vince/core/VinceWalkOut.cpp` 10, `src/Apps/Vince/core/WalkSounds.cpp` 1, `src/Apps/Vince/core/WaterRegion.cpp` 4, `src/Apps/Vince/core/Whammy.cpp` 2, `src/Apps/Vince/core/ambientsound.cpp` 3, `src/Apps/Vince/core/settings.cpp` 2, `src/Apps/Vince/core/xbfont.cpp` 2, `src/Apps/Vince/core/xbresource.cpp` 4, `src/Apps/Vince/scripts/Bayou/BogWallow.cpp` 5, `src/Apps/Vince/scripts/Bayou/CrawdadJimmys.cpp` 10, `src/Apps/Vince/scripts/Bayou/FanBoat.cpp` 3, `src/Apps/Vince/scripts/Bayou/FanHurricane.cpp` 7, `src/Apps/Vince/scripts/Bayou/HannaCutscenes.cpp` 1, `src/Apps/Vince/scripts/Bayou/Hannah.cpp` 10, `src/Apps/Vince/scripts/Bayou/HurricaneHannah.cpp` 3, `src/Apps/Vince/scripts/Bayou/JimmysFanboatCutscenes.cpp` 1, `src/Apps/Vince/scripts/Bayou/Onion.cpp` 12, `src/Apps/Vince/scripts/Bayou/Outhouse.cpp` 4, `src/Apps/Vince/scripts/Bayou/SwitchBoat.cpp` 3, `src/Apps/Vince/scripts/Bayou/VinceInWind.cpp` 3, `src/Apps/Vince/scripts/Bayou/jeanlafittesship.cpp` 3, `src/Apps/Vince/scripts/Carnival/BumperCar.cpp` 3, `src/Apps/Vince/scripts/Carnival/BumperCarCutscenes.cpp` 4, `src/Apps/Vince/scripts/Carnival/BumpercarBumpoff.cpp` 3, `src/Apps/Vince/scripts/Carnival/Carnival.cpp` 5, `src/Apps/Vince/scripts/Carnival/DoorPulleyBucket.cpp` 6, `src/Apps/Vince/scripts/Carnival/EvilVince.cpp` 2, `src/Apps/Vince/scripts/Carnival/HouseOfMirrors.cpp` 4, `src/Apps/Vince/scripts/Carnival/Kosmo.cpp` 6, `src/Apps/Vince/scripts/Carnival/LandORides.cpp` 6, `src/Apps/Vince/scripts/Carnival/ScareHouse.cpp` 12, `src/Apps/Vince/scripts/Carnival/SwitchCarnival.cpp` 3, `src/Apps/Vince/scripts/Carnival/TrailerFrogs.cpp` 3, `src/Apps/Vince/scripts/Carnival/generator.cpp` 3, `src/Apps/Vince/scripts/Carnival/insidekosmobotcutscenes.cpp` 7, `src/Apps/Vince/scripts/Carnival/wheelofmystery.cpp` 5, `src/Apps/Vince/scripts/Crypt_city/Bell.cpp` 3, `src/Apps/Vince/scripts/Crypt_city/Cannon.cpp` 5, `src/Apps/Vince/scripts/Crypt_city/Lantern.cpp` 3, `src/Apps/Vince/scripts/Crypt_city/Mirror.cpp` 7, `src/Apps/Vince/scripts/Crypt_city/MoleGatePuzzle.cpp` 7, `src/Apps/Vince/scripts/Crypt_city/Net.cpp` 4, `src/Apps/Vince/scripts/Crypt_city/RatRaceRodeo.cpp` 11, `src/Apps/Vince/scripts/Crypt_city/RatRaceRodeoCutscenes.cpp` 1, `src/Apps/Vince/scripts/Crypt_city/SarcophagusHustle.cpp` 3, `src/Apps/Vince/scripts/Crypt_city/Spring.cpp` 3, `src/Apps/Vince/scripts/Crypt_city/VinceSpring.cpp` 5, `src/Apps/Vince/scripts/Crypt_city/ZombieGuidanceCounselor.cpp` 1, `src/Apps/Vince/scripts/Crypt_city/gwistlethorpe.cpp` 3, `src/Apps/Vince/scripts/Crypt_city/lightbeam.cpp` 17, `src/Apps/Vince/scripts/Crypt_city/switchcrypt.cpp` 3, `src/Apps/Vince/scripts/ManorHouse/CaptainsRoom.cpp` 9, `src/Apps/Vince/scripts/ManorHouse/TumblerRoomCutscenes.cpp` 1, `src/Apps/Vince/scripts/Sewers/BasketCase.cpp` 9, `src/Apps/Vince/scripts/Sewers/JaniceCutscenes.cpp` 9, `src/Apps/Vince/scripts/Sewers/MainTunnel.cpp` 3, `src/Apps/Vince/scripts/Sewers/MainTunnelCutscenes.cpp` 9, `src/Apps/Vince/scripts/Sewers/PropellerRoom.cpp` 6, `src/Apps/Vince/scripts/Sewers/RocketLab.cpp` 9, `src/Apps/Vince/scripts/Sewers/RocketLabCutscene.cpp` 32, `src/Apps/Vince/scripts/Sewers/SearchlightBeam.cpp` 2, `src/Apps/Vince/scripts/Sewers/ThrottleUp.cpp` 3, `src/Apps/Vince/scripts/french_quarter/Blender.cpp` 5, `src/Apps/Vince/scripts/french_quarter/Courtyard.cpp` 3, `src/Apps/Vince/scripts/french_quarter/DinoBoss.cpp` 2, `src/Apps/Vince/scripts/french_quarter/DocksMain.cpp` 3, `src/Apps/Vince/scripts/french_quarter/DoorStorageRoom.cpp` 2, `src/Apps/Vince/scripts/french_quarter/FlourSack.cpp` 3, `src/Apps/Vince/scripts/french_quarter/FrenchQuarter.cpp` 5, `src/Apps/Vince/scripts/french_quarter/GasPumps.cpp` 3, `src/Apps/Vince/scripts/french_quarter/GingerDM.cpp` 3, `src/Apps/Vince/scripts/french_quarter/KosmoPigCutscene.cpp` 4, `src/Apps/Vince/scripts/french_quarter/Mainstreet.cpp` 12, `src/Apps/Vince/scripts/french_quarter/Oven.cpp` 2, `src/Apps/Vince/scripts/french_quarter/PipeGas.cpp` 6, `src/Apps/Vince/scripts/french_quarter/Rocket.cpp` 2, `src/Apps/Vince/scripts/french_quarter/SafeHookRope.cpp` 10, `src/Apps/Vince/scripts/french_quarter/Scale.cpp` 4, `src/Apps/Vince/scripts/french_quarter/Switch.cpp` 3, `src/Apps/Vince/scripts/french_quarter/SwitchSafe.cpp` 3, `src/Apps/Vince/scripts/french_quarter/Terradactyl.cpp` 4, `src/Apps/Vince/scripts/french_quarter/TeslaCoils.cpp` 3, `src/Apps/Vince/scripts/french_quarter/VinceZapHat.cpp` 6, `src/Apps/Vince/scripts/french_quarter/WallSwitch.cpp` 2, `src/Apps/Vince/scripts/french_quarter/bakeryMain.cpp` 3, `src/Apps/Vince/scripts/french_quarter/theSquare.cpp` 6, `src/Apps/Vince/scripts/voodooShop/FrontEndUI.cpp` 9, `src/Apps/Vince/scripts/voodooShop/VoodooShop.cpp` 4, `src/Apps/Vince/scripts/voodooShop/VoodooShopVince.cpp` 3, `src/Libs/BeepLib/Xbox/Official_Release/BeepFile.cpp` 28, `src/Libs/BeepLib/Xbox/Official_Release/BeepFileSystem.cpp` 10, `src/Libs/BeepLib/Xbox/Official_Release/BeepLib.cpp` 6, `src/Libs/BeepLib/Xbox/Official_Release/BeepState.cpp` 2, `src/Libs/BeepLib/Xbox/Official_Release/BeepTDF.cpp` 8, `src/Libs/BeepLib/Xbox/Official_Release/BeepZlib.cpp` 1, `src/Libs/FireFlyLib/Xbox/Official_Release/FFlyController.cpp` 3, `src/Libs/FireFlyLib/Xbox/Official_Release/FFlyEmitterSurface.cpp` 2, `src/Libs/FireFlyLib/Xbox/Official_Release/fflyemitter.cpp` 3, `src/Libs/Swamp/Xbox/Official_Release/FogZone.cpp` 4, `src/Libs/Swamp/Xbox/Official_Release/LoadScreen.cpp` 2, `src/Libs/Swamp/Xbox/Official_Release/Localize.cpp` 4, `src/Libs/Swamp/Xbox/Official_Release/MoviePlayer.cpp` 7, `src/Libs/Swamp/Xbox/Official_Release/ParticleManager.cpp` 2, `src/Libs/Swamp/Xbox/Official_Release/ProfileBar.cpp` 1, `src/Libs/Swamp/Xbox/Official_Release/RegionTrigger.cpp` 12, `src/Libs/Swamp/Xbox/Official_Release/RumbleResource.cpp` 11, `src/Libs/Swamp/Xbox/Official_Release/SoundNode.cpp` 5, `src/Libs/Swamp/Xbox/Official_Release/Sprite.cpp` 2, `src/Libs/Swamp/Xbox/Official_Release/Swamp.cpp` 1, `src/Libs/Swamp/Xbox/Official_Release/SwampAnimationResource.cpp` 5, `src/Libs/Swamp/Xbox/Official_Release/SwampCollide.cpp` 2, `src/Libs/Swamp/Xbox/Official_Release/SwampDebugGeometry.cpp` 8, `src/Libs/Swamp/Xbox/Official_Release/SwampDiskArbitrator.cpp` 7, `src/Libs/Swamp/Xbox/Official_Release/SwampFireFlyNode.cpp` 9, `src/Libs/Swamp/Xbox/Official_Release/SwampGatorNode.cpp` 23, `src/Libs/Swamp/Xbox/Official_Release/SwampInput.cpp` 1, `src/Libs/Swamp/Xbox/Official_Release/SwampLight.cpp` 2, `src/Libs/Swamp/Xbox/Official_Release/SwampLoader.cpp` 9, `src/Libs/Swamp/Xbox/Official_Release/SwampMaterialResource.cpp` 14, `src/Libs/Swamp/Xbox/Official_Release/SwampModelResource.cpp` 12, `src/Libs/Swamp/Xbox/Official_Release/SwampRenderer.cpp` 4, `src/Libs/Swamp/Xbox/Official_Release/SwampResourceManager.cpp` 8, `src/Libs/Swamp/Xbox/Official_Release/SwampScript.cpp` 17, `src/Libs/Swamp/Xbox/Official_Release/SwampShaderResource.cpp` 2, `src/Libs/Swamp/Xbox/Official_Release/SwampShadow.cpp` 1, `src/Libs/Swamp/Xbox/Official_Release/SwampShapes.cpp` 7, `src/Libs/Swamp/Xbox/Official_Release/SwampSoundResource.cpp` 13, `src/Libs/Swamp/Xbox/Official_Release/SwampWater.cpp` 8, `src/Libs/Swamp/Xbox/Official_Release/SwampWorld.cpp` 40, `src/Libs/Swamp/Xbox/Official_Release/swampfireflyresource.cpp` 4, `src/Libs/Swamp/Xbox/Official_Release/swampsky.cpp` 2, `src/Libs/Swamp/Xbox/Official_Release/swamptextureeffects.cpp` 8, `src/Libs/SwampShared/Xbox/Official_Release/SwampBSP.cpp` 2, `src/Libs/SwampShared/Xbox/Official_Release/SwampDiary.cpp` 4, `src/Libs/SwampShared/Xbox/Official_Release/SwampHottie.cpp` 1, `src/Libs/SwampShared/Xbox/Official_Release/SwampQuat.cpp` 1

Spot-checks (5 per kind, evenly spaced over the absent rows, checked by hand against the PDB name sets with
`.run/5.5/t3/pdbq.py`):
- data: `std::basic_string::_pad00` (SwampD3D.cpp:18, BumperCar.cpp:16, Rocket.cpp:15) true absent;
  `std::basic_string::_Alval`, `std::vector::_Alval`, `std::deque::_Alval` exist only on the VC7.1 `_String_val`/
  `_Vector_val`-style bases (absent-elsewhere, true: hierarchy flattened). No false positive after fixes.
- method: `Vector4::operator Vector3` and `MultiAnimation::operator ResourceHandle<AnimationResource>` were false
  positives (PDB spells `operator struct Vector3`, `operator class ResourceHandle<class AnimationResource>`;
  fixed, now ok); `SoundNode::sizeof` was a parser false positive (`char field_pad[0xB0 - sizeof(...)]`, now data);
  `ResourceHandle::operator->` not in the ResourceHandle fieldlist (true);
  `std::list::iterator::operator!=`/`operator==` and `std::vector::iterator::operator!=` not in those fieldlists
  (true); `GatorNode::DoesJorgeRepelMe`, `GatorNode::CanJorgeCollideWithMe` not GatorNode members, named
  elsewhere (true); `CollidableCylinder::SetCollidable` absent-elsewhere (true).
- typedef: `SwampString` (0 occurrences of the name in default.pdb), `ScriptTree`, `TexRemapTraits`,
  `NodeMapTraits` true absent.
- typedef-nested: `LocatorManager::Map` (PDB has `NameMap`) true; the only hit.
- param: `CritterManager::Find(name)` (PDB locals it, lookup, s), `GatorNode::StartAnimation(restart, loop)`
  (PDB: anim, looped, reverse, script), `SwampScript::RegisterTimerEvent(repeat)` (PDB: looped),
  `Matrix::BatchApply(in)` (PDB: src, dest), `std::basic_string::basic_string(ptr)` (PDB: _Ptr): true.
- enumerator: `std::basic_string::npos` declared as an enumerator; PDB has it as a member, not an enumerator
  (kind mismatch, true as an enumerator); the only hit.
Fixed during the census (false positives found by the spot-checks): `operator->` names (provenance_lint's
`strip_templates` drops `>`; members now compared as written); template ctor/dtor names (`Ptr<X>` member names
stripped); anonymous union members (marker eaten by strip_templates); `operator=` declarations read as data;
conversion-operator spelling; `sizeof(...)` in an array bound read as a method.

Heuristic limits: no preprocessor expansion (macro-generated members invisible); classes local to function bodies
are not parsed (e.g. MoviePlayer.cpp:392 `KeyExitThread`); NON_MATCHING arms are excluded; params of inline
definitions are not counted (only `;` declarations); parameter checks merge all overloads of `owner::method`;
enumerators are checked against all PDB enum fieldlists, not their own enum; base-class lookup is not done
(hence `absent-elsewhere`); a declaration split across a template head or unusual macro may be misread.

## 2. S5: obj locals that pass the locals check only as NEUTRAL

Rule: provenance_lint `locals` check (CONVENTIONS §7; NEUTRAL = `^(?:v\d+|tmp|i|p)$`, tools/provenance_lint.py:116)
and D78 for names.

Method: as `provenance_lint.check_locals` (:794-830) with cache=True (objs from derived/provenance_locals; misses
compiled into `.run/5.5/t3/locals_scratch`, deleted at exit): for each matched function whose obj proc has locals,
each obj local whose name matches NEUTRAL and is not a PDB local of its proc (`pdb_local_names`); line =
`use_line` (first whole-word use in the body); !FAKE = `has_fake` on that line or the comment block above.

Counts: 246 neutral-only locals in 114 matched functions; with `// !FAKE:` 110, without 136. Coverage: matched functions with obj locals 2494, without 15, no obj proc 0.

By file (`line name`, `*` = has !FAKE):

- `src/Apps/Vince/core/Critter.cpp`: 921 v4*
- `src/Apps/Vince/core/FanCeiling.cpp`: 408 v3, 406 v6, 418 v5
- `src/Apps/Vince/core/GameRegions.cpp`: 198 p*
- `src/Apps/Vince/core/HUDUI.cpp`: 1150 v1*, 1153 v0, 1205 v1*, 1208 v0, 1251 v1*, 1253 v0, 1463 v11*, 1457 v9*, 1473 v13*, 1592 v2*, 1633 v1*, 1900 v0, 2099 v1*, 2102 v0, 2268 p
- `src/Apps/Vince/core/Monster.cpp`: 598 v0*
- `src/Apps/Vince/core/Music.cpp`: 470 v0*, 552 v0, 669 tmp*, 671 v1, 693 tmp*
- `src/Apps/Vince/core/Particle.cpp`: 39 p, 46 p
- `src/Apps/Vince/core/PitFrog.cpp`: 253 v0
- `src/Apps/Vince/core/Switch.cpp`: 360 p*
- `src/Apps/Vince/core/VinceCam.cpp`: 580 tmp*, 580 tmp*, 580 tmp*, 580 tmp*, 580 tmp*, 580 tmp*, 580 tmp*
- `src/Apps/Vince/core/VinceSwing.cpp`: 297 tmp*
- `src/Apps/Vince/core/VinceWalkOut.cpp`: 367 v2, 360 v1*, 361 v3
- `src/Apps/Vince/scripts/Bayou/FanBoat.cpp`: 547 tmp, 606 tmp*, 1049 tmp, 1048 v1
- `src/Apps/Vince/scripts/Bayou/Hannah.cpp`: 852 v5, 829 v2, 848 v4, 825 v1, 838 v3, 856 v6, 868 v7, 872 v8, 928 v0*, 1040 v0*, 1029 v5, 1028 v4
- `src/Apps/Vince/scripts/Bayou/HurricaneHannah.cpp`: 427 v0*, 429 v1
- `src/Apps/Vince/scripts/Bayou/Onion.cpp`: 856 v0*, 1210 v1*
- `src/Apps/Vince/scripts/Carnival/DoorPulleyBucket.cpp`: 155 tmp*
- `src/Apps/Vince/scripts/Carnival/LandORides.cpp`: 267 v2, 271 v3, 281 v5, 267 v2, 271 v3
- `src/Apps/Vince/scripts/Carnival/ScareHouse.cpp`: 783 v0, 786 tmp*, 786 tmp*, 794 v1, 808 v2, 1071 v1, 1064 v0*
- `src/Apps/Vince/scripts/Crypt_city/Cannon.cpp`: 257 v0*, 365 v0*, 367 v2, 366 v1
- `src/Apps/Vince/scripts/Crypt_city/RatRaceRodeo.cpp`: 726 v0*, 1317 v5, 1316 v2*, 1318 v1, 1320 v6, 1322 p
- `src/Apps/Vince/scripts/Crypt_city/switchcrypt.cpp`: 81 v0, 80 v1*, 396 v1, 399 v3*
- `src/Apps/Vince/scripts/ManorHouse/CaptainsRoom.cpp`: 657 v0, 659 v1, 665 v2, 684 v3
- `src/Apps/Vince/scripts/Sewers/BasketCase.cpp`: 773 v3*, 930 v1*, 953 v2, 952 v1*, 1189 v9, 1183 v6*, 1170 v5, 1185 v7, 1191 v8
- `src/Apps/Vince/scripts/Sewers/MainTunnel.cpp`: 702 tmp*, 731 tmp*
- `src/Apps/Vince/scripts/Sewers/SearchlightBeam.cpp`: 177 v1*, 179 v0
- `src/Apps/Vince/scripts/french_quarter/Blender.cpp`: 455 v0, 455 v1, 611 v0*, 605 tmp*, 629 v0*, 673 v0, 671 v4, 659 tmp*, 674 v1, 670 v3*, 664 v2*, 713 v0, 711 v4, 714 v1, 710 v3*, 704 v2*
- `src/Apps/Vince/scripts/french_quarter/DinoBoss.cpp`: 463 v1, 481 v2*, 508 v1, 513 v3
- `src/Apps/Vince/scripts/french_quarter/GasPumps.cpp`: 698 v0*, 696 v2*, 700 v1, 797 v2
- `src/Apps/Vince/scripts/french_quarter/KosmoPigCutscene.cpp`: 207 v0*
- `src/Apps/Vince/scripts/french_quarter/Mainstreet.cpp`: 609 v2*, 611 v4, 610 v3
- `src/Apps/Vince/scripts/french_quarter/SafeHookRope.cpp`: 405 v4, 404 v3, 420 v0*, 429 v2, 435 v4, 423 v1, 434 v3, 470 tmp*, 474 v1*
- `src/Apps/Vince/scripts/french_quarter/theSquare.cpp`: 879 tmp*, 901 v1, 992 p*
- `src/Libs/BeepLib/Xbox/Official_Release/BeepFileSystem.cpp`: 253 v4, 252 v3
- `src/Libs/FireFlyLib/Xbox/Official_Release/FFlyEmitterSurface.cpp`: 198 v0*, 211 v1*, 243 v0*, 262 v2*, 237 v1*, 350 v0*
- `src/Libs/FireFlyLib/Xbox/Official_Release/FFlyForce.cpp`: 159 tmp
- `src/Libs/FireFlyLib/Xbox/Official_Release/fflyemitter.cpp`: 530 v1*, 595 v5*
- `src/Libs/Swamp/Xbox/Official_Release/Decal.cpp`: 175 v0*, 214 v0*, 217 v2, 216 v1
- `src/Libs/Swamp/Xbox/Official_Release/RegionTrigger.cpp`: 417 v0*, 419 v1, 442 v0*, 565 tmp, 571 v4, 578 v3, 565 tmp, 565 tmp
- `src/Libs/Swamp/Xbox/Official_Release/SwampCamera.cpp`: 255 v0, 287 v0, 305 tmp, 305 tmp, 322 v0, 305 tmp, 305 tmp, 305 tmp, 344 v1, 350 v2, 305 tmp, 305 tmp, 356 v3, 362 v4, 305 tmp, 407 v0, 405 tmp, 405 tmp
- `src/Libs/Swamp/Xbox/Official_Release/SwampCollide.cpp`: 253 tmp*
- `src/Libs/Swamp/Xbox/Official_Release/SwampFireFlyNode.cpp`: 569 v0*, 644 v0*
- `src/Libs/Swamp/Xbox/Official_Release/SwampGatorNode.cpp`: 669 v0*, 840 v0*, 841 tmp, 859 tmp*, 865 v1, 861 v0
- `src/Libs/Swamp/Xbox/Official_Release/SwampLight.cpp`: 564 tmp*, 564 tmp*, 592 p*, 1026 v0
- `src/Libs/Swamp/Xbox/Official_Release/SwampLoader.cpp`: 826 v0*
- `src/Libs/Swamp/Xbox/Official_Release/SwampMaterialResource.cpp`: 404 tmp*, 497 tmp*, 497 tmp*
- `src/Libs/Swamp/Xbox/Official_Release/SwampModelResource.cpp`: 840 v5, 836 v2*, 839 v4, 837 v3, 851 v7, 853 v9, 891 v5, 887 v2*, 890 v4, 888 v3, 902 v7, 904 v9
- `src/Libs/Swamp/Xbox/Official_Release/SwampRenderer.cpp`: 336 v1*, 357 v1*
- `src/Libs/Swamp/Xbox/Official_Release/SwampScript.cpp`: 509 v1*
- `src/Libs/Swamp/Xbox/Official_Release/SwampWater.cpp`: 383 v2, 382 v1*, 420 v1*, 442 v1*
- `src/Libs/Swamp/Xbox/Official_Release/SwampWorld.cpp`: 1448 v0*, 1546 v0*, 1679 v0*, 1876 v2, 1991 v0*
- `src/Libs/SwampShared/Xbox/Official_Release/SwampBSP2node.cpp`: 390 v0*, 409 v7, 408 v6*, 470 v0*, 471 v1, 470 v0*, 471 v1, 470 v0*, 471 v1, 470 v0*, 471 v1
- `src/Libs/SwampShared/Xbox/Official_Release/SwampDiary.cpp`: 598 p
- `src/Libs/SwampShared/Xbox/Official_Release/SwampMatrix.cpp`: 441 v7, 439 v5, 435 v2, 434 v1*, 436 v3, 457 v1
- `src/Libs/SwampShared/Xbox/Official_Release/SwampVector.cpp`: 62 v0

Spot-checks: Critter.cpp:921 `v4` (!FAKE on the line), Hannah.cpp:838 `v3` (no !FAKE), BasketCase.cpp:1170
`v5` (no !FAKE), FFlyEmitterSurface.cpp:198 `v0` (!FAKE above), SwampLight.cpp:564 `tmp` (!FAKE above): all
declared stand-ins as listed.

Limits: the !FAKE flag is read at the first use line only; a !FAKE elsewhere in the function is not seen. A local
named `i`/`p` that the PDB also lacks counts here even when the original code may have used it.

## 3. S7: `__declspec(noreturn)`, `#pragma intrinsic`, volatile members

Rule: D78 and D116 (compiler-steering forms need `// !FAKE:`; provenance_lint d66 covers `volatile` in matched
bodies only).

Method: line scan of every src file (code part, comments excluded). noreturn: `__declspec(noreturn)`. intrinsic:
`#pragma intrinsic` lines, `before`/`after` the file's first `// FUNCTION:` line. volatile member: a `;`-ended
code line with `volatile`, no `(` and no `=` (excludes locals with initializers and casts). Arm: in a
NON_MATCHING-defined arm or live. !FAKE = has_fake.

Counts: noreturn 3 (!FAKE 3); `#pragma intrinsic` 32 (before first FUNCTION 30, after 2; !FAKE 9); volatile members 3 (!FAKE 0; in a NON_MATCHING arm 3).

noreturn:
- `src/Apps/Vince/core/HUDUI.cpp:813` !FAKE yes (live)
- `src/Apps/Vince/scripts/Crypt_city/VinceSpring.cpp:137` !FAKE yes (live)
- `src/Libs/Swamp/Xbox/Official_Release/SwampLoader.cpp:225` !FAKE yes (live)

`#pragma intrinsic`:
- `src/Apps/Vince/core/Critter.cpp:8` before !FAKE no (live)
- `src/Apps/Vince/core/HUDUI.cpp:5` before !FAKE no (live)
- `src/Apps/Vince/core/HUDUI.cpp:13` before !FAKE no (live)
- `src/Apps/Vince/core/VinceCam.cpp:372` after !FAKE yes (NON_MATCHING)
- `src/Apps/Vince/core/xbresource.cpp:4` before !FAKE no (live)
- `src/Apps/Vince/scripts/Bayou/Hannah.cpp:4` before !FAKE no (live)
- `src/Apps/Vince/scripts/Bayou/Hannah.cpp:5` before !FAKE yes (live)
- `src/Apps/Vince/scripts/Bayou/Onion.cpp:10` before !FAKE no (live)
- `src/Apps/Vince/scripts/Bayou/Onion.cpp:13` before !FAKE no (live)
- `src/Apps/Vince/scripts/Crypt_city/VinceSpring.cpp:5` before !FAKE no (live)
- `src/Apps/Vince/scripts/Crypt_city/ZombieGuidanceCounselor.cpp:6` before !FAKE yes (live)
- `src/Apps/Vince/scripts/Crypt_city/lightbeam.cpp:5` before !FAKE no (live)
- `src/Apps/Vince/scripts/Sewers/BasketCase.cpp:6` before !FAKE yes (live)
- `src/Apps/Vince/scripts/french_quarter/Blender.cpp:6` before !FAKE yes (live)
- `src/Apps/Vince/scripts/voodooShop/FrontEndUI.cpp:244` before !FAKE no (live)
- `src/Libs/BeepLib/Xbox/Official_Release/BeepAlloc.cpp:63` before !FAKE no (live)
- `src/Libs/BeepLib/Xbox/Official_Release/BeepLib.cpp:5` before !FAKE yes (live)
- `src/Libs/BeepLib/Xbox/Official_Release/BeepTDF.cpp:6` before !FAKE yes (live)
- `src/Libs/BeepLib/Xbox/Official_Release/unzip.c:9` before !FAKE no (live)
- `src/Libs/Swamp/Xbox/Official_Release/MoviePlayer.cpp:4` before !FAKE no (live)
- `src/Libs/Swamp/Xbox/Official_Release/RegionTrigger.cpp:3` before !FAKE no (live)
- `src/Libs/Swamp/Xbox/Official_Release/RegionTrigger.cpp:6` before !FAKE no (live)
- `src/Libs/Swamp/Xbox/Official_Release/SoundStreamer.cpp:6` before !FAKE no (live)
- `src/Libs/Swamp/Xbox/Official_Release/SwampCamera.cpp:5` before !FAKE no (live)
- `src/Libs/Swamp/Xbox/Official_Release/SwampCamera.cpp:7` before !FAKE no (live)
- `src/Libs/Swamp/Xbox/Official_Release/SwampDebugGeometry.cpp:6` before !FAKE no (live)
- `src/Libs/Swamp/Xbox/Official_Release/SwampInput.cpp:25` before !FAKE no (live)
- `src/Libs/Swamp/Xbox/Official_Release/SwampLight.cpp:19` before !FAKE no (live)
- `src/Libs/Swamp/Xbox/Official_Release/SwampLoader.cpp:4` before !FAKE no (live)
- `src/Libs/Swamp/Xbox/Official_Release/SwampWater.cpp:5` before !FAKE no (live)
- `src/Libs/SwampShared/Xbox/Official_Release/SwampBSP.cpp:6` before !FAKE yes (live)
- `src/Libs/SwampShared/Xbox/Official_Release/SwampBog.cpp:90` after !FAKE yes (live)

volatile members:
- `src/Libs/Swamp/Xbox/Official_Release/MoviePlayer.cpp:393` !FAKE no (NON_MATCHING)
- `src/Libs/Swamp/Xbox/Official_Release/MoviePlayer.cpp:394` !FAKE no (NON_MATCHING)
- `src/Libs/Swamp/Xbox/Official_Release/MoviePlayer.cpp:396` !FAKE no (NON_MATCHING)

R5.5-003 confirmed: noreturn x3 at the same three sites, 32 `#pragma intrinsic`, 3 volatile members
MoviePlayer.cpp:393-396. Added: the volatile members are in a function-local struct (`KeyExitThread`) inside a
NON_MATCHING arm, none carries !FAKE (the !FAKE at :404 covers the pointer-to-volatile cast at :406); the two
`after` pragmas are VinceCam.cpp:372 (NON_MATCHING arm) and SwampBog.cpp:90.

Limits: a declaration split over lines or a macro-generated noreturn is not seen; `volatile` members written
with an initializer or a function-pointer form would be missed (none found by grep: 6 `volatile` code lines in
src, 3 locals/casts in BeepAlloc.cpp and MoviePlayer.cpp:406 are not members).

## 4. I43: short `// !FAKE:` labels

Rule: docs/matching/CONVENTIONS.md:78-100 (`// !FAKE: <pass>; <instrument>; <reason>`).

Method: every line of src holding `// !FAKE:`; fields = the text after the tag split on `;`, non-empty after
strip. Line-only count, and a second count joining following own-line `//` comment lines (stopping at a tag line:
!FAKE, FUNCTION, PROTOTYPE, NAME, UNIT, DISABLED). `no before/after recorded` = case-insensitive match in the
joined text.

Counts: 1396 `// !FAKE:` lines; fewer than three fields on the line 54; with continuation lines joined 43; reasons reading `no before/after recorded` 84.

R5.5-003 (1,396 lines, 54 short, 84 no before/after) confirmed. 11 of the 54 complete their third field on a
continuation line.

Short on the line (`line(fields line/joined)`):

- `src/Apps/Vince/core/CameraManager.cpp`: 701(2/3), 739(2/3)
- `src/Apps/Vince/core/CarriedObject.cpp`: 350(2/2), 409(2/2)
- `src/Apps/Vince/core/Critter.cpp`: 921(2/2), 1172(2/3)
- `src/Apps/Vince/core/DebugCam.cpp`: 204(2/2), 236(2/2)
- `src/Apps/Vince/core/LevelLoader.cpp`: 861(2/2), 870(2/2), 1189(2/3)
- `src/Apps/Vince/core/Monster.cpp`: 482(2/2), 496(2/2), 502(1/1), 512(1/1), 522(1/1)
- `src/Apps/Vince/scripts/Bayou/CrawdadJimmys.cpp`: 471(2/2)
- `src/Apps/Vince/scripts/Bayou/FanBoat.cpp`: 604(2/3), 626(2/3), 749(2/2), 1044(2/4)
- `src/Apps/Vince/scripts/Bayou/Hannah.cpp`: 821(2/2), 832(2/2), 833(2/2), 841(2/2), 842(2/2)
- `src/Apps/Vince/scripts/Carnival/Kosmo.cpp`: 685(2/2)
- `src/Apps/Vince/scripts/Carnival/ScareHouse.cpp`: 1082(2/2)
- `src/Apps/Vince/scripts/Crypt_city/Cannon.cpp`: 212(2/2), 363(2/3)
- `src/Apps/Vince/scripts/Crypt_city/ZombieGuidanceCounselor.cpp`: 1056(2/3)
- `src/Apps/Vince/scripts/french_quarter/Blender.cpp`: 277(2/3), 1148(2/2)
- `src/Apps/Vince/scripts/french_quarter/GasPumps.cpp`: 770(2/2)
- `src/Libs/BeepLib/Xbox/Official_Release/BeepState.cpp`: 825(2/3)
- `src/Libs/FireFlyLib/Xbox/Official_Release/FFlyEmitterSurface.cpp`: 210(2/2), 236(2/2), 245(2/2), 255(2/2), 261(2/2), 349(2/2)
- `src/Libs/Swamp/Xbox/Official_Release/FogZone.cpp`: 144(2/2)
- `src/Libs/Swamp/Xbox/Official_Release/LoadScreen.cpp`: 254(2/2)
- `src/Libs/Swamp/Xbox/Official_Release/RegionTrigger.cpp`: 538(2/2), 561(2/2), 611(2/2)
- `src/Libs/Swamp/Xbox/Official_Release/SwampDebugGeometry.cpp`: 289(2/2)
- `src/Libs/Swamp/Xbox/Official_Release/SwampFireFlyNode.cpp`: 356(2/2)
- `src/Libs/Swamp/Xbox/Official_Release/SwampInput.cpp`: 438(2/2)
- `src/Libs/Swamp/Xbox/Official_Release/SwampLight.cpp`: 794(2/2)
- `src/Libs/SwampShared/Xbox/Official_Release/SwampMatrix.cpp`: 490(2/2), 502(2/2), 511(2/2), 542(2/2)

Limits: a `;` inside the reason text adds a field (a long reason with `;` passes as three fields); continuation
joining may absorb an unrelated comment block that follows the label.

## 5. I52: free functions with no PDB or symbols name

Rule: D78 and D127 (only marked `pin_emit_` scaffolds may be unmarked extra code).

Method: the S4 parser's function definitions (`(...) {` heads) at namespace scope, unqualified or qualified only
by a namespace, in the 214 scoped files, NON_MATCHING-defined and `#if 0` arms excluded; absent when neither the
namespace-qualified nor the bare name (template args stripped) is a default.pdb proc, a demangled public
(`demangle_scope`), or a symbols.txt / symbols_pin.txt row name (raw or demangled). Class: `pin_emit` (name
starts `pin_emit_`), `inline-or-template` (`inline` or `template` on the name's line or the line before), other.

Counts: 188 in 75 files: pin_emit 108, inline-or-template 78, other 2 (both `other` rows, BeepState.cpp:75 and SwampWorld.cpp:92
`std::_Dist_type`, are templates whose `template` head is two lines up: by hand 80 inline/template, 0 other).

By file (`line name`):

- `src/Apps/Vince/Init.cpp`: 227 pin_emit_vector_ctor_iterator
- `src/Apps/Vince/SwampD3D.cpp`: 25 pin_emit_vector_dtor_iterator
- `src/Apps/Vince/core/Boomerang.cpp`: 90 pin_emit_PotionOfKnowledgeMain_dtor
- `src/Apps/Vince/core/CameraManager.cpp`: 43 std::operator==, 53 std::swap, 219 std::_Iter_cat, 278 std::_Allocate, 286 std::_Destroy, 301 std::_Ptr_cat, 672 std::_Ptr_cat, 742 std::_Tree::iteratorstd::_Tree::erase, 1107 pin_emit_GameCam_assign
- `src/Apps/Vince/core/Critter.cpp`: 104 std::swap, 128 std::_Destroy, 1123 pin_emit_CritterCollision_copy, 1175 std::_Tree::iteratorstd::_Tree::erase, 1697 pin_emit_pair_DamageInfo_copy
- `src/Apps/Vince/core/DialogueRegion.cpp`: 33 std::operator==
- `src/Apps/Vince/core/FanCeiling.cpp`: 20 std::operator==, 25 std::operator!=
- `src/Apps/Vince/core/FontManager.cpp`: 94 std::swap, 118 std::_Destroy
- `src/Apps/Vince/core/HUDUI.cpp`: 774 std::_Allocate, 1693 pin_emit_BeadsUI_dtor, 1819 pin_emit_QuickMessage_copy, 1863 pin_emit_AreaStats_copy, 2145 pin_emit_SkullUI_dtor, 2153 pin_emit_HealthUI_dtor
- `src/Apps/Vince/core/LevelLoader.cpp`: 300 pin_emit_AreaStats_assign, 1146 pin_emit_LevelStats_assign
- `src/Apps/Vince/core/LevelScript.cpp`: 186 pin_emit_ProgressHint_assign
- `src/Apps/Vince/core/MondoMole.cpp`: 19 std::operator==
- `src/Apps/Vince/core/Monster.cpp`: 110 operator+
- `src/Apps/Vince/core/Music.cpp`: 53 std::operator==, 533 pin_emit_Overlay_copy, 709 pin_emit_Overlay_assign
- `src/Apps/Vince/core/PitFrog.cpp`: 38 std::operator==, 43 std::operator!=
- `src/Apps/Vince/core/PowerIcon.cpp`: 35 std::operator==
- `src/Apps/Vince/core/SignTrolley.cpp`: 94 std::swap, 118 std::_Destroy, 1179 pin_emit_MapStop_dtor, 1186 pin_emit_LocInfo_ctor, 1196 pin_emit_MapInfo_ctor, 1203 pin_emit_Sprite_delete, 1210 pin_emit_MapInfo_dtor, 1217 pin_emit_LocInfo_dtor, 1227 pin_emit_LocInfo_copy, 1237 pin_emit_MapStop_copy, 1244 pin_emit_MapStop_assign, 1266 pin_emit_SignTrolleyMain_dtor, 1307 pin_emit_MapInfo_copy, 1314 pin_emit_MapInfo_assign, 1383 pin_emit_TrolleyScreen_dtor
- `src/Apps/Vince/core/Trolley.cpp`: 340 pin_emit_TrolleyMain_dtor
- `src/Apps/Vince/core/VinceCam.cpp`: 37 std::operator==, 42 std::operator!=
- `src/Apps/Vince/core/VinceSwing.cpp`: 353 pin_emit_VinceSwing_dtor
- `src/Apps/Vince/core/WaterRegion.cpp`: 6 std::_Destroy, 66 std::operator==, 290 pin_emit_Splasher_copy, 297 pin_emit_Splasher_assign
- `src/Apps/Vince/core/Whammy.cpp`: 628 pin_emit_WhammyResultScript_dtor, 784 pin_emit_WhammyProjectileMain_dtor, 880 pin_emit_WhammyResultFrozenMain_dtor, 1081 pin_emit_VinceWhammyMain_dtor, 1148 pin_emit_WhammyPropMain_dtor
- `src/Apps/Vince/core/settings.cpp`: 58 std::operator==
- `src/Apps/Vince/scripts/Bayou/BogWallow.cpp`: 27 std::operator==, 580 pin_emit_CrawdadTrapMain_dtor
- `src/Apps/Vince/scripts/Bayou/CrawdadJimmys.cpp`: 7 std::_Destroy, 365 pin_emit_ActivatorMain_dtor, 659 pin_emit_BasicDoorMain_dtor
- `src/Apps/Vince/scripts/Bayou/FanBoat.cpp`: 876 pin_emit_FanBoatWake_dtor
- `src/Apps/Vince/scripts/Bayou/FanHurricane.cpp`: 255 pin_emit_FanHurricaneMain_dtor
- `src/Apps/Vince/scripts/Bayou/Hannah.cpp`: 991 pin_emit_HannahMain_dtor
- `src/Apps/Vince/scripts/Bayou/Onion.cpp`: 14 fabs, 18 wmemcpy, 32 std::_Destroy, 106 std::operator==, 545 pin_emit_AreaStats_delete, 622 pin_emit_CritterMain_dtor, 672 pin_emit_MonsterMain_dtor, 750 pin_emit_OnionMain_dtor, 935 pin_emit_ResourceHandle_TextureResource_delete, 955 pin_emit_Sprite_dtor, 1296 pin_emit_Slot_dtor, 1384 pin_emit_LevelLoader_dtor, 1437 pin_emit_OnionHud_dtor
- `src/Apps/Vince/scripts/Bayou/SwitchBoat.cpp`: 33 std::operator==
- `src/Apps/Vince/scripts/Bayou/VinceInWind.cpp`: 172 pin_emit_VinceShared_dtor, 213 pin_emit_VinceMain_dtor
- `src/Apps/Vince/scripts/Carnival/BumperCar.cpp`: 549 pin_emit_BumperCarHud_dtor
- `src/Apps/Vince/scripts/Carnival/BumpercarBumpoff.cpp`: 285 pin_emit_AreaLoadRegionMain_dtor
- `src/Apps/Vince/scripts/Carnival/EvilVince.cpp`: 206 std::_Ptr_cat, 302 std::operator==
- `src/Apps/Vince/scripts/Carnival/HouseOfMirrors.cpp`: 326 pin_emit_FireBugMain_dtor
- `src/Apps/Vince/scripts/Carnival/Kosmo.cpp`: 22 std::operator==, 411 pin_emit_KosmoBotHeadMain_dtor, 456 pin_emit_KosmoBotMidriffMain_dtor
- `src/Apps/Vince/scripts/Carnival/ScareHouse.cpp`: 911 pin_emit_LocatorMain_dtor
- `src/Apps/Vince/scripts/Carnival/ShootingCam.cpp`: 249 pin_emit_SwampCamera_dtor, 256 pin_emit_OverheatUI_dtor, 263 pin_emit_ShootingUI_dtor
- `src/Apps/Vince/scripts/Carnival/wheelofmystery.cpp`: 151 pin_emit_WheelOfMysteryMain_dtor, 367 pin_emit_PitFrogMain_dtor
- `src/Apps/Vince/scripts/Crypt_city/Bell.cpp`: 375 pin_emit_BellMain_dtor
- `src/Apps/Vince/scripts/Crypt_city/MoleGatePuzzle.cpp`: 564 pin_emit_MondoMolePuzzleRegionMain_dtor, 571 pin_emit_MondoMoleMain_dtor
- `src/Apps/Vince/scripts/Crypt_city/Net.cpp`: 212 pin_emit_NetMain_dtor
- `src/Apps/Vince/scripts/Crypt_city/RatRaceRodeo.cpp`: 516 pin_emit_WeaponMain_dtor, 523 pin_emit_MrsGMain_dtor, 981 pin_emit_RodeoRatMain_dtor, 991 pin_emit_VinceOnRatMain_dtor
- `src/Apps/Vince/scripts/Crypt_city/VinceSpring.cpp`: 24 std::operator==, 32 std::_Ptr_cat, 37 std::_Allocate, 42 std::_Destroy
- `src/Apps/Vince/scripts/Crypt_city/ZombieGuidanceCounselor.cpp`: 419 pin_emit_ZombieMain_dtor
- `src/Apps/Vince/scripts/ManorHouse/CaptainsRoom.cpp`: 41 std::operator==, 711 pin_emit_DollyMain_dtor
- `src/Apps/Vince/scripts/Sewers/BasketCase.cpp`: 1005 pin_emit_LaundryCarMain_dtor, 1041 pin_emit_VinceLaundryCar_dtor
- `src/Apps/Vince/scripts/Sewers/MainTunnel.cpp`: 858 pin_emit_KillerDilloMain_dtor
- `src/Apps/Vince/scripts/Sewers/RocketLab.cpp`: 29 std::operator==, 308 pin_emit_FlamePipesMain_dtor
- `src/Apps/Vince/scripts/Sewers/SearchlightBeam.cpp`: 190 pin_emit_FogZone_dtor
- `src/Apps/Vince/scripts/Sewers/ThrottleUp.cpp`: 28 std::operator==
- `src/Apps/Vince/scripts/french_quarter/Blender.cpp`: 898 pin_emit_VinceInBlender_dtor, 1184 pin_emit_BlenderMain_dtor
- `src/Apps/Vince/scripts/french_quarter/DinoBoss.cpp`: 21 std::operator==, 26 std::operator!=, 34 fabs
- `src/Apps/Vince/scripts/french_quarter/FlourSack.cpp`: 41 std::operator==, 372 pin_emit_FlourSackMain_dtor
- `src/Apps/Vince/scripts/french_quarter/GasPumps.cpp`: 31 std::_Destroy, 35 std::_Allocate, 724 pin_emit_GasPumpMain_dtor
- `src/Apps/Vince/scripts/french_quarter/GingerDM.cpp`: 560 pin_emit_GingerDeadManMain_dtor
- `src/Apps/Vince/scripts/french_quarter/Oven.cpp`: 281 pin_emit_OvenMain_dtor
- `src/Apps/Vince/scripts/french_quarter/SafeHookRope.cpp`: 388 pin_emit_SafeHookRopeMain_dtor
- `src/Apps/Vince/scripts/french_quarter/Scale.cpp`: 212 pin_emit_ScaleMain_dtor
- `src/Apps/Vince/scripts/french_quarter/VinceZapHat.cpp`: 183 pin_emit_VinceZapHat_dtor
- `src/Apps/Vince/scripts/french_quarter/WallSwitch.cpp`: 184 pin_emit_WallSwitchMain_dtor
- `src/Apps/Vince/scripts/french_quarter/theSquare.cpp`: 29 std::operator==, 563 pin_emit_SkeletonJazzGuyMain_dtor, 850 pin_emit_DialogueRegionMain_dtor
- `src/Apps/Vince/scripts/voodooShop/FrontEndUI.cpp`: 54 std::operator!=, 504 pin_emit_LevelEntry_copy
- `src/Apps/Vince/scripts/voodooShop/VoodooShop.cpp`: 32 std::_Destroy, 320 pin_emit_Transition_dtor, 327 pin_emit_SavedGameInfo_ctor, 338 pin_emit_SavedGameInfo_copy, 345 pin_emit_CaptionLine_delete, 365 pin_emit_CaptionLine_assign, 508 pin_emit_SavedGamesScreen_dtor
- `src/Libs/BeepLib/Xbox/Official_Release/BeepState.cpp`: 8 std::_Ptr_cat, 62 std::_Iter_cat, 69 std::_Val_type, 75 std::_Dist_type, 80 std::swap, 281 std::iter_swap
- `src/Libs/Swamp/Xbox/Official_Release/HeatShimmer.cpp`: 75 pin_emit_HeatShimmer_dtor
- `src/Libs/Swamp/Xbox/Official_Release/RegionTrigger.cpp`: 7 fabs, 71 std::operator==, 75 std::operator==, 79 std::operator!=
- `src/Libs/Swamp/Xbox/Official_Release/SwampFireFlyNode.cpp`: 122 std::_Destroy
- `src/Libs/Swamp/Xbox/Official_Release/SwampGatorNode.cpp`: 1216 pin_emit_ProfileBar_dtor
- `src/Libs/Swamp/Xbox/Official_Release/SwampLoader.cpp`: 14 std::_Allocate, 126 std::_Ptr_cat, 133 std::_Ptr_cat
- `src/Libs/Swamp/Xbox/Official_Release/SwampResourceManager.cpp`: 19 std::_Destroy, 125 std::swap, 796 pin_emit_ResourceBundle_copy, 901 pin_emit_ResourceBundle_delete, 1116 pin_emit_TreeNod_set_string_delete
- `src/Libs/Swamp/Xbox/Official_Release/SwampScript.cpp`: 423 pin_emit_SaveLoadVar_copy, 430 pin_emit_SaveLoadVar_assign, 449 pin_emit_QueuedMessage_copy
- `src/Libs/Swamp/Xbox/Official_Release/SwampWorld.cpp`: 86 std::_Val_type, 92 std::_Dist_type, 97 std::swap, 104 std::iter_swap
- `src/Libs/SwampShared/Xbox/Official_Release/SwampBSP.cpp`: 166 std::_Allocate, 190 std::_Ptr_cat
- `src/Libs/SwampShared/Xbox/Official_Release/SwampDiary.cpp`: 20 std::_Destroy, 52 std::swap

Spot-checks: Init.cpp:227 `pin_emit_vector_ctor_iterator`, HUDUI.cpp:1693 `pin_emit_BeadsUI_dtor` (scaffolds,
D127); VinceSpring.cpp:32 `std::_Ptr_cat`, CameraManager.cpp:43 `std::operator==`, FontManager.cpp:118
`std::_Destroy` (inline STL templates, emitted only when instantiated, named in the PDB with template args only
where instantiated): true per the method.

Limits: a PDB proc of an instantiated template is matched on the stripped name, so a used template passes;
functions defined via macros, or with a head the parser misreads, are missed; static (file-local) functions
count like any other.

## 6. S6: SEL_ANY inline copies vs retail bytes (T3.c2)

Method: build `src` -> `.run/5.5/t3/build` (218 objs, rc 0). Per obj, code symbols skipped only by the inline-copy rule (tools/matched_set.py:436-447 order: not listed (unit,symbol), not eh-funclet, not $L label, COMDAT selection SEL_ANY). Our length = symbol to next symbol in its section or section end (va_dis `obj` rule). Row: unique sized `func` line in symbols.txt, else symbols_pin.txt. Compare our bytes vs retail XBE at row VA, 4 bytes under each of our relocations masked. Verdicts: equal / differs (first diff offset) / size-differs / no table row (no func row, >1 rows, or row without size).

Totals: 4979 symbol instances across 200 objs: equal 670, differs 0, size-differs 10, no table row 4299. Unique names: equal 310, differs 0, size-differs 10, no table row 2710.
- 107 instances have a name that is in the matched set under another unit.
- equal rows: 118 via symbols.txt, 552 via symbols_pin.txt. no table row: 4,247 no func row, 52 row has no size.
- Spot-check (tools/va_dis.py obj, relocs checked by name): BeepDebug `??1logic_error@std@@UAE@XZ` equal -> `no difference`; BumpercarBumpoff `??1?$basic_string@D...@std@@QAE@XZ` equal -> `no difference`; WalkSounds `?_Tidy@?$vector@V?$ResourceHandle@VSoundResource@@@@...@IAEXXZ` size-differs (0x1F vs 0x2D) -> `first difference: line 5, ours +0x8`. 3/3 agree.
- differs: none. Unexplained: none (size-differs are the findings below).

size-differs (all):
- LevelLoader.obj `?_Buy@?$vector@VAreaStats@@V?$allocator@VAreaStats@@@std@@@std@@IAE_NI@Z` 0x000E84B1: ours 0x48 table 0x46 (symbols.txt)
- LevelLoader.obj `?_Xlen@?$vector@U?$pair@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V12@@std@@V?$allocator@U?$pair@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V12@@std@@@2@@std@@IBEXXZ` 0x00139A45: ours 0x34 table 0x35 (symbols_pin.txt)
- ProfileBar.obj `?copy@?$char_traits@D@std@@SAPADPADPBDI@Z` 0x00011000: ours 0x5 table 0x23 (symbols.txt)
- SwampGatorNode.obj `?_Xlen@?$vector@IV?$allocator@I@std@@@std@@IBEXXZ` 0x00139A45: ours 0x34 table 0x35 (symbols_pin.txt)
- SwampGatorNode.obj `?_Xlen@?$vector@UMatrix@@V?$allocator@UMatrix@@@std@@@std@@IBEXXZ` 0x00139A45: ours 0x34 table 0x35 (symbols_pin.txt)
- SwampGatorNode.obj `?_Xlen@?$vector@USwampDeformation@@V?$allocator@USwampDeformation@@@std@@@std@@IBEXXZ` 0x00139A45: ours 0x34 table 0x35 (symbols_pin.txt)
- SwampGatorNode.obj `?_Xlen@?$vector@UVector2@@V?$allocator@UVector2@@@std@@@std@@IBEXXZ` 0x00139A45: ours 0x34 table 0x35 (symbols_pin.txt)
- WalkSounds.obj `??$_Destroy_range@V?$ResourceHandle@VSoundResource@@@@V?$allocator@V?$ResourceHandle@VSoundResource@@@@@std@@@std@@YAXPAV?$ResourceHandle@VSoundResource@@@@0AAV?$allocator@V?$ResourceHandle@VSoundResource@@@@@0@@Z` 0x000C78E3: ours 0x1 table 0x1A (symbols_pin.txt)
- WalkSounds.obj `??$_Destroy_range@V?$ResourceHandle@VSoundResource@@@@V?$allocator@V?$ResourceHandle@VSoundResource@@@@@std@@@std@@YAXPAV?$ResourceHandle@VSoundResource@@@@0AAV?$allocator@V?$ResourceHandle@VSoundResource@@@@@0@U_Nonscalar_ptr_iterator_tag@0@@Z` 0x000C787B: ours 0x1 table 0x1B (symbols_pin.txt)
- WalkSounds.obj `?_Tidy@?$vector@V?$ResourceHandle@VSoundResource@@@@V?$allocator@V?$ResourceHandle@VSoundResource@@@@@std@@@std@@IAEXXZ` 0x000C7B97: ours 0x1F table 0x2D (symbols_pin.txt)

By obj (equal / differs / size-differs / no table row):
- Activator.obj: 1 / 0 / 0 / 3
- AmbientBird.obj: 2 / 0 / 0 / 6
- ambientsound.obj: 1 / 0 / 0 / 9
- AnimCam.obj: 2 / 0 / 0 / 2
- bakeryMain.obj: 1 / 0 / 0 / 0
- BasicDoor.obj: 2 / 0 / 0 / 17
- BasketCase.obj: 8 / 0 / 0 / 48
- Bead.obj: 1 / 0 / 0 / 10
- BeepDebug.obj: 10 / 0 / 0 / 41
- BeepFile.obj: 3 / 0 / 0 / 10
- BeepFileSystem.obj: 2 / 0 / 0 / 33
- BeepLib.obj: 3 / 0 / 0 / 19
- BeepRand.obj: 0 / 0 / 0 / 2
- BeepState.obj: 0 / 0 / 0 / 27
- BeepTDF.obj: 2 / 0 / 0 / 37
- Bell.obj: 2 / 0 / 0 / 9
- Blender.obj: 3 / 0 / 0 / 54
- BogWallow.obj: 2 / 0 / 0 / 15
- Boomerang.obj: 1 / 0 / 0 / 0
- BubbaGator.obj: 1 / 0 / 0 / 0
- BumperCar.obj: 3 / 0 / 0 / 11
- BumpercarBumpoff.obj: 2 / 0 / 0 / 2
- BumperCarCutscenes.obj: 1 / 0 / 0 / 13
- CameraManager.obj: 14 / 0 / 0 / 96
- Cannon.obj: 3 / 0 / 0 / 9
- CaptainsRoom.obj: 2 / 0 / 0 / 13
- Carnival.obj: 1 / 0 / 0 / 3
- CarriedObject.obj: 1 / 0 / 0 / 15
- Courtyard.obj: 1 / 0 / 0 / 2
- CrawdadJimmys.obj: 3 / 0 / 0 / 27
- Critter.obj: 25 / 0 / 0 / 188
- Cutscene.obj: 1 / 0 / 0 / 9
- Decal.obj: 2 / 0 / 0 / 4
- DialogueRegion.obj: 1 / 0 / 0 / 4
- DinoBoss.obj: 0 / 0 / 0 / 15
- DocksMain.obj: 1 / 0 / 0 / 1
- DoorPulleyBucket.obj: 1 / 0 / 0 / 2
- DoorStorageRoom.obj: 1 / 0 / 0 / 7
- EffectRegions.obj: 1 / 0 / 0 / 15
- EvilVince.obj: 0 / 0 / 0 / 13
- Eyecam.obj: 0 / 0 / 0 / 36
- FanBoat.obj: 9 / 0 / 0 / 52
- FanCeiling.obj: 1 / 0 / 0 / 17
- FanHurricane.obj: 2 / 0 / 0 / 8
- FFlyController.obj: 9 / 0 / 0 / 49
- fflyemitter.obj: 0 / 0 / 0 / 6
- FFlyEmitterSurface.obj: 6 / 0 / 0 / 20
- FFlyForce.obj: 4 / 0 / 0 / 6
- FireRegion.obj: 1 / 0 / 0 / 3
- FlourSack.obj: 2 / 0 / 0 / 17
- FogZone.obj: 3 / 0 / 0 / 4
- FontManager.obj: 1 / 0 / 0 / 43
- FrenchQuarter.obj: 1 / 0 / 0 / 3
- FrontEndUI.obj: 3 / 0 / 0 / 25
- GameCam.obj: 0 / 0 / 0 / 8
- GameRegions.obj: 1 / 0 / 0 / 8
- GasPumps.obj: 2 / 0 / 0 / 35
- generator.obj: 2 / 0 / 0 / 15
- GingerDM.obj: 2 / 0 / 0 / 19
- GoldJuneBug.obj: 1 / 0 / 0 / 8
- gwistlethorpe.obj: 1 / 0 / 0 / 1
- HannaCutscenes.obj: 2 / 0 / 0 / 2
- Hannah.obj: 3 / 0 / 0 / 34
- HeatShimmer.obj: 0 / 0 / 0 / 2
- HeavyObject.obj: 1 / 0 / 0 / 5
- HouseOfMirrors.obj: 1 / 0 / 0 / 1
- HUDUI.obj: 14 / 0 / 0 / 81
- HurricaneHannah.obj: 4 / 0 / 0 / 13
- Init.obj: 9 / 0 / 0 / 82
- insidekosmobotcutscenes.obj: 3 / 0 / 0 / 8
- Intro.obj: 1 / 0 / 0 / 10
- JaniceCutscenes.obj: 2 / 0 / 0 / 4
- jeanlafittesship.obj: 2 / 0 / 0 / 3
- JimmysFanboatCutscenes.obj: 2 / 0 / 0 / 4
- KillerDillo.obj: 1 / 0 / 0 / 2
- Kosmo.obj: 3 / 0 / 0 / 24
- KosmoPigCutscene.obj: 2 / 0 / 0 / 4
- LandORides.obj: 1 / 0 / 0 / 6
- Lantern.obj: 1 / 0 / 0 / 0
- LevelLoader.obj: 77 / 0 / 2 / 297
- LevelScript.obj: 1 / 0 / 0 / 5
- lightbeam.obj: 3 / 0 / 0 / 6
- LoadScreen.obj: 1 / 0 / 0 / 4
- LoadScreens.obj: 0 / 0 / 0 / 2
- LocalizedStrings.obj: 1 / 0 / 0 / 0
- Locator.obj: 2 / 0 / 0 / 51
- locstring.obj: 0 / 0 / 0 / 1
- Main.obj: 0 / 0 / 0 / 1
- Mainstreet.obj: 4 / 0 / 0 / 18
- MainTunnel.obj: 9 / 0 / 0 / 18
- MainTunnelCutscenes.obj: 2 / 0 / 0 / 4
- Mirror.obj: 1 / 0 / 0 / 0
- MoleGatePuzzle.obj: 1 / 0 / 0 / 34
- MondoMole.obj: 4 / 0 / 0 / 17
- Monster.obj: 2 / 0 / 0 / 31
- MoviePlayer.obj: 4 / 0 / 0 / 13
- MultiAnimation.obj: 2 / 0 / 0 / 11
- Music.obj: 2 / 0 / 0 / 22
- Net.obj: 2 / 0 / 0 / 2
- Onion.obj: 3 / 0 / 0 / 54
- Outhouse.obj: 2 / 0 / 0 / 4
- Oven.obj: 4 / 0 / 0 / 16
- Particle.obj: 1 / 0 / 0 / 0
- ParticleManager.obj: 12 / 0 / 0 / 80
- PipeGas.obj: 1 / 0 / 0 / 4
- PitFrog.obj: 2 / 0 / 0 / 11
- PowerIcon.obj: 1 / 0 / 0 / 15
- ProfileBar.obj: 6 / 0 / 1 / 45
- PropellerRoom.obj: 7 / 0 / 0 / 4
- RatRaceRodeo.obj: 2 / 0 / 0 / 28
- RatRaceRodeoCutscenes.obj: 2 / 0 / 0 / 8
- RegionTrigger.obj: 2 / 0 / 0 / 37
- Rocket.obj: 1 / 0 / 0 / 1
- RocketLab.obj: 3 / 0 / 0 / 12
- RocketLabCutscene.obj: 1 / 0 / 0 / 2
- RumbleResource.obj: 2 / 0 / 0 / 9
- SafeHookRope.obj: 2 / 0 / 0 / 18
- SarcophagusHustle.obj: 1 / 0 / 0 / 2
- Scale.obj: 2 / 0 / 0 / 8
- ScareHouse.obj: 5 / 0 / 0 / 31
- SearchlightBeam.obj: 5 / 0 / 0 / 7
- settings.obj: 2 / 0 / 0 / 6
- ShootingCam.obj: 4 / 0 / 0 / 8
- SignTrolley.obj: 18 / 0 / 0 / 94
- SoundNode.obj: 1 / 0 / 0 / 24
- SoundStreamer.obj: 1 / 0 / 0 / 3
- Spring.obj: 1 / 0 / 0 / 0
- Sprite.obj: 4 / 0 / 0 / 6
- Swamp.obj: 0 / 0 / 0 / 8
- SwampAnimationResource.obj: 2 / 0 / 0 / 14
- SwampBog.obj: 0 / 0 / 0 / 2
- SwampBSP.obj: 2 / 0 / 0 / 23
- SwampBSP2node.obj: 1 / 0 / 0 / 10
- SwampCamera.obj: 1 / 0 / 0 / 12
- SwampCollide.obj: 0 / 0 / 0 / 15
- SwampD3D.obj: 1 / 0 / 0 / 1
- SwampDebugGeometry.obj: 4 / 0 / 0 / 36
- SwampDiary.obj: 8 / 0 / 0 / 46
- SwampDiskArbitrator.obj: 0 / 0 / 0 / 19
- SwampFireFlyNode.obj: 3 / 0 / 0 / 39
- swampfireflyresource.obj: 2 / 0 / 0 / 8
- SwampGatorNode.obj: 17 / 0 / 4 / 200
- SwampGeometry.obj: 0 / 0 / 0 / 2
- SwampHottie.obj: 0 / 0 / 0 / 7
- SwampInput.obj: 0 / 0 / 0 / 7
- SwampLight.obj: 5 / 0 / 0 / 99
- SwampLightning.obj: 4 / 0 / 0 / 12
- SwampLoader.obj: 13 / 0 / 0 / 91
- SwampMaterialResource.obj: 6 / 0 / 0 / 23
- SwampMatrix.obj: 0 / 0 / 0 / 6
- SwampModelResource.obj: 7 / 0 / 0 / 105
- SwampQuat.obj: 0 / 0 / 0 / 5
- SwampRenderer.obj: 6 / 0 / 0 / 14
- SwampResourceManager.obj: 56 / 0 / 0 / 264
- SwampScript.obj: 5 / 0 / 0 / 73
- SwampShaderResource.obj: 1 / 0 / 0 / 8
- SwampShadow.obj: 0 / 0 / 0 / 2
- SwampShapes.obj: 3 / 0 / 0 / 7
- swampsky.obj: 0 / 0 / 0 / 9
- SwampSoundResource.obj: 2 / 0 / 0 / 40
- swamptextureeffects.obj: 3 / 0 / 0 / 5
- SwampVector.obj: 0 / 0 / 0 / 5
- SwampWater.obj: 3 / 0 / 0 / 27
- SwampWorld.obj: 9 / 0 / 0 / 112
- SwingHook.obj: 2 / 0 / 0 / 6
- Switch.obj: 3 / 0 / 0 / 17
- SwitchBoat.obj: 1 / 0 / 0 / 4
- SwitchCarnival.obj: 1 / 0 / 0 / 0
- switchcrypt.obj: 1 / 0 / 0 / 10
- SwitchSafe.obj: 4 / 0 / 0 / 6
- Terradactyl.obj: 2 / 0 / 0 / 4
- TeslaCoils.obj: 2 / 0 / 0 / 10
- theSquare.obj: 5 / 0 / 0 / 29
- ThrottleUp.obj: 2 / 0 / 0 / 13
- TrailerFrogs.obj: 2 / 0 / 0 / 3
- TrainingSign.obj: 1 / 0 / 0 / 0
- Trolley.obj: 2 / 0 / 0 / 5
- TumblerRoomCutscenes.obj: 2 / 0 / 0 / 2
- VinceCam.obj: 2 / 0 / 0 / 33
- VinceCarryHeavy.obj: 1 / 0 / 0 / 5
- VinceCarryLight.obj: 0 / 0 / 0 / 1
- VinceCutScene.obj: 0 / 0 / 0 / 8
- VinceFlat.obj: 3 / 0 / 0 / 8
- VinceInWind.obj: 4 / 0 / 0 / 4
- VinceSpring.obj: 3 / 0 / 0 / 26
- VinceSwing.obj: 3 / 0 / 0 / 3
- VinceWalkIn.obj: 1 / 0 / 0 / 1
- VinceWalkOut.obj: 2 / 0 / 0 / 13
- VinceZapHat.obj: 2 / 0 / 0 / 1
- VoodooShop.obj: 4 / 0 / 0 / 23
- VoodooShopVince.obj: 2 / 0 / 0 / 1
- WalkSounds.obj: 3 / 0 / 3 / 38
- WallSwitch.obj: 2 / 0 / 0 / 3
- WaterRegion.obj: 3 / 0 / 0 / 50
- Weapon.obj: 0 / 0 / 0 / 1
- Whammy.obj: 2 / 0 / 0 / 20
- wheelofmystery.obj: 3 / 0 / 0 / 18
- xbfont.obj: 0 / 0 / 0 / 2
- xbresource.obj: 1 / 0 / 0 / 0
- ZombieGuidanceCounselor.obj: 5 / 0 / 0 / 42

Per-symbol list: `.run/5.5/t3/c2_data.txt` lines `S6 ...` (scratch).
Reproduce: `PY tools/build.py --src src --out .run/5.5/t3/build` then `PY .run/5.5/t3/c2_census.py` (prints `S6 total ...`, `MOD total ...`), `PY .run/5.5/t3/c2_write.py`.

## 7. PDB module vs unit, matched set (T3.c2)

Method: all `[[function]]` rows of config/matched_set.toml. PDB modules = default.pdb modules with a proc at the prototype VA of the symbol's PDB public (imageBase + section va + off); fallback by demangled name (provenance_lint.demangle_scope). Match: row `unit` == module name and retail `va` in units.toml [text_start, text_end). Else classes: tail run (proc in a non-main proto run of the module; runs as tools/units_seed.py), COMDAT outside range, unit has no text range; module differs: template/COMDAT placed elsewhere (proc's contribution has LNK_COMDAT or name has `?$`), duplicate name (name fallback, >1 procs), no PDB proc; else unexplained.

Totals: 2507 rows; resolved via public VA 2507, via name 0. In unit and in range: 2502. template/COMDAT placed elsewhere: 5. Tail run 0, duplicate name 0, no PDB proc 0, unexplained 0 (none for T13).

Mismatches:
- template/COMDAT placed elsewhere: row unit Xbox\OfficialRelease\Hannah.obj, `?rfind@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIPBDII@Z` 0x000292B0; PDB module Xbox\OfficialRelease\RocketLab.obj
- template/COMDAT placed elsewhere: row unit Xbox\OfficialRelease\Onion.obj, `?_Eos@?$basic_string@GU?$char_traits@G@std@@V?$allocator@G@2@@std@@IAEXI@Z` 0x0001841B; PDB module Xbox\OfficialRelease\Bayou.obj
- template/COMDAT placed elsewhere: row unit Xbox\OfficialRelease\Onion.obj, `?_Tidy@?$basic_string@GU?$char_traits@G@std@@V?$allocator@G@2@@std@@IAEX_NI@Z` 0x000184CB; PDB module Xbox\OfficialRelease\Bayou.obj
- template/COMDAT placed elsewhere: row unit Xbox\Official_Release\SwampQuat.obj, `?Normalize@@YA?AUVector4@@ABU1@@Z` 0x000DC7CE; PDB module Xbox\Official_Release\SwampMatrix.obj
- template/COMDAT placed elsewhere: row unit core\XBox\OfficialRelease\BubbaGator.obj, `?capacity@?$vector@VParam@@V?$allocator@VParam@@@std@@@std@@QBEIXZ` 0x00111476; PDB module Xbox\OfficialRelease\FanBoat.obj, core\XBox\OfficialRelease\Settings.obj

Reproduce: `PY .run/5.5/t3/c2_census.py` (lines `MOD ...` in `.run/5.5/t3/c2_data.txt`).

## 8. S10: provenance_lint --diff since Generation 5 start (T3.c2)

Method: one run, `bash tools/run.sh t3-provdiff -- PY tools/provenance_lint.py --diff d72e9e0d0` (d72e9e0d0 = Generation 5 start). rc 1 (FAIL; a finding, not a tool error).

Summary line:
```
provenance_lint: FAIL 1 failure(s) (proto-pdb 0, placeholder 0, src-pdb 0, src-retail 0, hedge 0, fake-diff 1, name-retail 0, d66 0, locals 0, accepted 0, forceinline 0, noinline 0, pragma 0, include 0); 214 matched file(s), 219 source file(s); locals coverage: 2494 function(s) with obj locals, 15 without, 0 no obj proc; 13.1s
```

Fails by check: fake-diff 1; all other checks 0.
```
FAIL fake-diff src/Apps/Vince/scripts/voodooShop/FrontEndUI.cpp:350: removed `// !FAKE:` with no changed code line in its function (d72e9e0d0 line)
```

- Verdict (expert, T3): the one fake-diff fail is a reworded label, not a removal: 963b13ce9 (5.1 T6.c2, I43 fields) prefixed `linker COMDAT folding;` to the file-scope `!FAKE` at d72e9e0d0 :350 (now :357, outside any FUNCTION extent, so the per-function pairing cannot see the re-added line; C0169). Listed for T13: fake-diff pairing of file-scope labels.
