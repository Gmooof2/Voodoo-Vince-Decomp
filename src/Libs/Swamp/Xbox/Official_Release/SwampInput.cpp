// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampInput.cpp OBJ: Xbox\Official_Release\SwampInput.obj OPT: /O1 /Oy /Oi
#include <stdio.h>
#include <stdlib.h>
#include "../../../../include/decomp.h"

typedef struct _LAUNCH_DATA { // NAME: proto-pdb:default.pdb
    unsigned char Data[0xC00]; // 0x000
} LAUNCH_DATA;
DECOMP_SIZE_ASSERT(LAUNCH_DATA, 0xC00);

typedef struct _LD_LAUNCH_DASHBOARD { // NAME: proto-pdb:default.pdb
    unsigned long dwReason;            // 0x000
    unsigned long dwContext;           // 0x004
    unsigned long dwParameter1;        // 0x008
    unsigned long dwParameter2;        // 0x00C
    unsigned char Reserved[0xBF0];     // 0x010
} LD_LAUNCH_DASHBOARD;
DECOMP_SIZE_ASSERT(LD_LAUNCH_DASHBOARD, 0xC00);

extern "C" unsigned long __stdcall XLaunchNewImageA(const char*, LAUNCH_DATA*); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall XInputClose(void* hDevice); // src proto-pdb:default.pdb XInputClose
void operator delete[](void* p);
void* operator new[](unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb
extern "C" void* __cdecl memset(void*, int, unsigned int); // NAME: proto-pdb:default.pdb
#pragma intrinsic(memset)

typedef struct _XPP_DEVICE_TYPE { // NAME: proto-pdb:default.pdb
    unsigned long Reserved[3];         // 0x00
} XPP_DEVICE_TYPE;
DECOMP_SIZE_ASSERT(XPP_DEVICE_TYPE, 0xC);
extern "C" XPP_DEVICE_TYPE XDEVICE_TYPE_GAMEPAD_TABLE; // NAME: proto-pdb:default.pdb
extern "C" void* __stdcall XInputOpen(XPP_DEVICE_TYPE*, unsigned long, unsigned long, void*); // NAME: proto-pdb:default.pdb
extern "C" XPP_DEVICE_TYPE XDEVICE_TYPE_MEMORY_UNIT_TABLE; // NAME: proto-pdb:default.pdb

typedef struct _XDEVICE_PREALLOC_TYPE { // NAME: proto-pdb:default.pdb
    XPP_DEVICE_TYPE* DeviceType;       // 0x00
    unsigned long dwPreallocCount;     // 0x04
} XDEVICE_PREALLOC_TYPE;
DECOMP_SIZE_ASSERT(XDEVICE_PREALLOC_TYPE, 0x8);
extern "C" void __stdcall XInitDevices(unsigned long, XDEVICE_PREALLOC_TYPE*); // NAME: proto-pdb:default.pdb
extern "C" unsigned long __stdcall XGetDevices(XPP_DEVICE_TYPE*); // NAME: proto-pdb:default.pdb
extern "C" int __stdcall XGetDeviceChanges(XPP_DEVICE_TYPE*, unsigned long*, unsigned long*); // NAME: proto-pdb:default.pdb
void* operator new(unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb

typedef struct _XINPUT_GAMEPAD { // NAME: proto-pdb:default.pdb
    unsigned short wButtons;           // 0x00
    unsigned char bAnalogButtons[8];   // 0x02
    short sThumbLX;                    // 0x0A
    short sThumbLY;                    // 0x0C
    short sThumbRX;                    // 0x0E
    short sThumbRY;                    // 0x10
} XINPUT_GAMEPAD;
DECOMP_SIZE_ASSERT(XINPUT_GAMEPAD, 0x12);
#pragma pack(push, 1)
typedef struct _XINPUT_STATE { // NAME: proto-pdb:default.pdb
    unsigned long dwPacketNumber;      // 0x00
    _XINPUT_GAMEPAD Gamepad;           // 0x04
} XINPUT_STATE;
#pragma pack(pop)
DECOMP_SIZE_ASSERT(XINPUT_STATE, 0x16);
extern "C" unsigned long __stdcall XInputGetState(void* hDevice, _XINPUT_STATE* pState); // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb
double GetTime();
}

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x24];                // 0x00
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class BeepTDF; // NAME: proto-pdb:default.pdb

class RumbleResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int SamplesRequired() const;
    unsigned int SampleRate() const { return _rate; }
    void GenerateSamples(unsigned short* samples, bool& loop) const;

    BeepTDF* _rumble;                  // 0x24
    float _duration;                   // 0x28
    unsigned int _rate;                // 0x2C
};
DECOMP_SIZE_ASSERT(RumbleResource, 0x30);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    const T& operator()() const { return *_Resource; }
    bool IsValid() const { return _Resource != 0; }

    T* _Resource;                      // 0x00
};

typedef struct _XINPUT_RUMBLE { // NAME: proto-pdb:default.pdb
    unsigned short wLeftMotorSpeed;    // 0x00
    unsigned short wRightMotorSpeed;   // 0x02
} XINPUT_RUMBLE;

#pragma pack(push, 1)
typedef struct _XINPUT_FEEDBACK_HEADER { // NAME: proto-pdb:default.pdb
    unsigned long dwStatus;            // 0x00
    void* hEvent;                      // 0x04
    unsigned char Reserved[58];        // 0x08
} XINPUT_FEEDBACK_HEADER;

typedef struct _XINPUT_FEEDBACK { // NAME: proto-pdb:default.pdb
    _XINPUT_FEEDBACK_HEADER Header;    // 0x00
    _XINPUT_RUMBLE Rumble;             // 0x42
} XINPUT_FEEDBACK;
#pragma pack(pop)
DECOMP_SIZE_ASSERT(XINPUT_FEEDBACK, 0x46);
extern "C" unsigned long __stdcall XInputSetState(void* hDevice, _XINPUT_FEEDBACK* pFeedback); // NAME: proto-pdb:default.pdb

class SwampGamePad { // NAME: proto-pdb:default.pdb
public:
    SwampGamePad(int slot);
    void PlayRumble(const ResourceHandle<RumbleResource>& rumble);
    ~SwampGamePad();
    void SetState(const _XINPUT_GAMEPAD& newState);
    bool IsAnyButton() const;
    const _XINPUT_GAMEPAD& GetState() const { return _current; }

protected:
    friend class SwampInput;
    void Update();

public:
    void* _handle;                     // 0x00
    unsigned long _lastPacketNum;      // 0x04
    bool _ignoreNextUpdate;            // 0x08
    bool _rumbleDirty;                 // 0x09
    bool _rumblePlaying;               // 0x0A
    bool _rumbleLoop;                  // 0x0B
    bool _rumblePaused;                // 0x0C
    unsigned short _buttonsHeld;       // 0x0E
    float _autoRepeatTimer;            // 0x10
    unsigned int _rumbleCurrent;       // 0x14
    unsigned int _rumbleMax;           // 0x18
    unsigned int _rumbleHi;            // 0x1C
    float _rumbleRate;                 // 0x20
    float _rumbleLast;                 // 0x24
    unsigned short* _rumblePattern;    // 0x28
    _XINPUT_GAMEPAD _current;          // 0x2C
    _XINPUT_GAMEPAD _previous;         // 0x3E
    _XINPUT_FEEDBACK _feedback;        // 0x50
};
DECOMP_SIZE_ASSERT(SwampGamePad, 0x98);

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    enum { // NAME: proto-pdb:default.pdb
        BUFFERED_RECORDS = 0x3C
    };

    struct InputRecord { // NAME: proto-pdb:default.pdb
        double sessionTime;            // 0x00
        float elapsed;                 // 0x08
        _XINPUT_GAMEPAD padState;      // 0x0C
    };

    void ResumeGame();
    void ResumeInputStream();
    void StopRecording();
    void SetGameTime(float);
    void Update();
    const SwampGamePad* GetPad(int player) const;
    bool IsPadPresent(int player) const;
    bool FindActivePad(int player);
    void PlayRumble(int player, const ResourceHandle<RumbleResource>& rumble);
    float GetGameTime() const { return _gameTimePublic; }

private:
    SwampInput();
    void AddRecord();
    const InputRecord& GetRecord();

public:

    float _elapsed;                    // 0x00
    float _actualElapsed;              // 0x04
    float _gameTimePublic;             // 0x08
    float _sessionTimePublic;          // 0x0C
    double _gameTimeInternal;          // 0x10
    double _sessionTimeInternal;       // 0x18
    double _createTime;                // 0x20
    float _simSpeed;                   // 0x28
    float _frameMaximum;               // 0x2C
    float _rumbleScale;                // 0x30
    bool _pauseGame;                   // 0x34
    bool _devChanges;                  // 0x35
    bool _recording;                   // 0x36
    bool _playback;                    // 0x37
    bool _ignoreInput;                 // 0x38
    bool _pauseInput;                  // 0x39
    bool _pipeJpadToDpad;              // 0x3A
    bool _autoRepeatEnabled;           // 0x3B
    float _autoRepeatRate;             // 0x3C
    float _autoRepeatStart;            // 0x40
    int _activePad[4];                 // 0x44
    unsigned long _deviceMap;          // 0x54
    SwampGamePad* _pads[4];            // 0x58
    SwampGamePad _defaultPad;          // 0x68
    InputRecord* _records;             // 0x100
    int _currentRecord;                // 0x104
    int _maxRecords;                   // 0x108
    FILE* _recordFile;                 // 0x10C
    int _totalRecords;                 // 0x110
    char field_114[0xC];               // 0x114
};
DECOMP_SIZE_ASSERT(SwampInput, 0x120);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000AD6E6
// PROTOTYPE: PREVIEW 0x0049856D identical
// NAME: proto-pdb:default.pdb
SwampGamePad::SwampGamePad(int slot)
{
    _handle = 0;
    _lastPacketNum = (unsigned long)-1;
    _ignoreNextUpdate = false;
    _rumbleDirty = false;
    _rumblePlaying = false;
    _rumbleLoop = false;
    _rumblePaused = false;
    _buttonsHeld = 0;
    _autoRepeatTimer = 0.0f;
    _rumblePattern = 0;
    memset(&_current, 0, sizeof(_current));
    memset(&_previous, 0, sizeof(_previous));
    memset(&_feedback, 0, sizeof(_feedback));
    if (slot >= 0)
        _handle = XInputOpen(&XDEVICE_TYPE_GAMEPAD_TABLE, slot, 0, 0);
}

// FUNCTION: USA 0x000AD74E
// PROTOTYPE: PREVIEW 0x004985D5 identical
// NAME: proto-pdb:default.pdb
SwampGamePad::~SwampGamePad()
{
    if (_handle) {
        XInputClose(_handle);
        _handle = 0;
    }
    delete[] _rumblePattern;
    _rumblePattern = 0;
}

// FUNCTION: USA 0x000AD76F
// PROTOTYPE: PREVIEW 0x004985F6 identical
// NAME: proto-pdb:default.pdb
bool SwampGamePad::IsAnyButton() const
{
    if (_current.wButtons != _previous.wButtons)
        return true;
    for (int i = 0; i < 8; i++) {
        if (abs(_current.bAnalogButtons[i] - _previous.bAnalogButtons[i]) > 0x20)
            return true;
    }
    return false;
}

// FUNCTION: USA 0x000AD7AB
// PROTOTYPE: PREVIEW 0x00498632 identical
// NAME: proto-pdb:default.pdb
SwampInput::SwampInput()
    : _elapsed(0.0f),
      _actualElapsed(0.0f),
      _gameTimePublic(0.0f),
      _sessionTimePublic(0.0f),
      _gameTimeInternal(0.0),
      _sessionTimeInternal(0.0),
      _createTime(BeepLib::GetTime()),
      _simSpeed(1.0f),
      _frameMaximum(0.1f),
      _rumbleScale(1.0f),
      _pauseGame(false),
      _devChanges(false),
      _recording(false),
      _playback(false),
      _ignoreInput(false),
      _pauseInput(false),
      _pipeJpadToDpad(false),
      _autoRepeatEnabled(true),
      _autoRepeatRate(0.1f),
      _autoRepeatStart(1.0f),
      _deviceMap(0),
      _defaultPad(-1),
      _records(0),
      _recordFile(0)
{
    XDEVICE_PREALLOC_TYPE deviceTypes[2] = {
        { &XDEVICE_TYPE_GAMEPAD_TABLE, 4 },
        { &XDEVICE_TYPE_MEMORY_UNIT_TABLE, 1 }
    };
    XInitDevices(2, deviceTypes);
    _deviceMap = XGetDevices(&XDEVICE_TYPE_GAMEPAD_TABLE);
    for (int i = 0; i < 4; i++) {
        _activePad[i] = 0;
        if (_deviceMap & (1 << i))
            _pads[i] = new("SwampInput.cpp", 0x17A) SwampGamePad(i);
        else
            _pads[i] = 0;
    }
}

// FUNCTION: USA 0x000AD8B3
// PROTOTYPE: PREVIEW 0x0049873A identical
// NAME: proto-pdb:default.pdb
void SwampInput::ResumeGame()
{
    if (_pauseGame) {
        _pauseGame = false;
        if (!_playback) {
            _sessionTimeInternal = BeepLib::GetTime() - _createTime;
            _sessionTimePublic = _sessionTimeInternal;
        }
    }
}

// FUNCTION: USA 0x000AD8D5
// PROTOTYPE: PREVIEW 0x0049875C identical
// NAME: proto-pdb:default.pdb
void SwampInput::StopRecording()
{
    if (_recording) {
        fwrite(_records, 1, _currentRecord * sizeof(InputRecord), _recordFile);
        fclose(_recordFile);
        _recording = false;
    }
}

// FUNCTION: USA 0x000AD90F
// PROTOTYPE: PREVIEW 0x00498796 identical
// NAME: proto-pdb:default.pdb
bool SwampInput::FindActivePad(int player)
{
    if (_activePad[player] > 0)
        return true;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (_activePad[j] == i + 1)
                goto next;
        }
        if (_pads[i] && _pads[i]->IsAnyButton()) {
            _activePad[player] = i + 1;
            return true;
        }
    next:;
    }
    return false;
}

// FUNCTION: USA 0x000AD974
// PROTOTYPE: PREVIEW 0x004987FB identical
// NAME: proto-pdb:default.pdb
const SwampGamePad* SwampInput::GetPad(int player) const
{
    if (_activePad[player] && _pads[_activePad[player] - 1])
        return _pads[_activePad[player] - 1];
    return &_defaultPad;
}

// FUNCTION: USA 0x000AD98E
// PROTOTYPE: PREVIEW 0x00498815 identical
// NAME: proto-pdb:default.pdb
bool SwampInput::IsPadPresent(int player) const
{
    if (_activePad[player] > 0)
        return _pads[_activePad[player] - 1] != 0;
    return false;
}

// FUNCTION: USA 0x000AD9AC
// PROTOTYPE: PREVIEW 0x00498833 identical
// NAME: proto-pdb:default.pdb
void SwampInput::ResumeInputStream()
{
    _pauseInput = false;
    if (!_playback) {
        _sessionTimeInternal = BeepLib::GetTime() - _createTime;
        _sessionTimePublic = _sessionTimeInternal;
    }
}

// FUNCTION: USA 0x000AD9C9
// PROTOTYPE: PREVIEW 0x00498850 identical
// NAME: proto-pdb:default.pdb
void SwampInput::AddRecord()
{
    if (_recording) {
        if (_currentRecord == BUFFERED_RECORDS) {
            fwrite(_records, 1, BUFFERED_RECORDS * sizeof(InputRecord), _recordFile);
            _currentRecord = 0;
        }
        // !FAKE: c2 scheduling; pointer local; retail calls GetPad before the time stores (match_one 66.7 -> 100.0)
        const SwampGamePad* p = GetPad(0);
        _records[_currentRecord].sessionTime = _sessionTimeInternal;
        _records[_currentRecord].elapsed = _elapsed;
        _records[_currentRecord].padState = p->GetState();
        _currentRecord++;
        _totalRecords++;
    }
}

// FUNCTION: USA 0x000ADA62
// PROTOTYPE: PREVIEW 0x004988E9 identical
// NAME: proto-pdb:default.pdb
const SwampInput::InputRecord& SwampInput::GetRecord()
{
    if (_currentRecord == _maxRecords) {
        if (!feof(_recordFile)) {
            _maxRecords = fread(_records, 1, BUFFERED_RECORDS * sizeof(InputRecord), _recordFile) / sizeof(InputRecord);
            _currentRecord = 0;
        } else {
            LD_LAUNCH_DASHBOARD ld = {0};
            ld.dwReason = 0;
            XLaunchNewImageA(0, (LAUNCH_DATA*)&ld);
        }
    }
    _currentRecord++;
    _totalRecords++;
    return _records[_currentRecord - 1];
}

// FUNCTION: USA 0x000ADAF7
// PROTOTYPE: PREVIEW 0x0049897E identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; `inline` drops cl's same-unit register summary (C0251): Update keeps no edx across the call
inline void SwampGamePad::SetState(const _XINPUT_GAMEPAD& newState)
{
    _previous = _current;
    _current = newState;
    _ignoreNextUpdate = true;
}

// FUNCTION: USA 0x000ADB1C
// PROTOTYPE: PREVIEW 0x004989A3 identical
// NAME: proto-pdb:default.pdb
void SwampGamePad::PlayRumble(const ResourceHandle<RumbleResource>& rumble)
{
    if (!rumble.IsValid()) {
        _rumblePlaying = false;
        _rumbleDirty = true;
        return;
    }
    _rumbleHi = rumble().SamplesRequired();
    if (!_rumbleHi) {
        _rumblePlaying = false;
        _rumbleDirty = true;
        return;
    }
    if (!_rumblePattern || _rumbleMax < _rumbleHi) {
        delete[] _rumblePattern;
        _rumbleMax = (_rumbleHi + 15) & ~15;
        _rumblePattern = new("SwampInput.cpp", 142) unsigned short[_rumbleMax * 2];
    }
    rumble().GenerateSamples(_rumblePattern, _rumbleLoop);
    _rumbleRate = rumble().SampleRate();
    _rumbleCurrent = 0;
    _rumbleLast = g_swamp->GetInput().GetGameTime();
    _rumblePlaying = true;
}

// FUNCTION: USA 0x000ADBBA
// PROTOTYPE: PREVIEW 0x00498A41 identical
// NAME: proto-pdb:default.pdb
void SwampGamePad::Update()
{
    if (!_handle)
        return;
    if (_ignoreNextUpdate) {
        _lastPacketNum--;
        _ignoreNextUpdate = false;
        return;
    }
    if (g_swamp->GetInput()._pauseGame) {
        if (!_rumblePaused) {
            _rumblePaused = true;
            _rumbleDirty = true;
        }
    } else if (_rumblePaused) {
        _rumblePaused = false;
        _rumbleDirty = true;
    }
    if (_rumblePlaying) {
        float now = g_swamp->GetInput().GetGameTime();
        if (_rumblePaused) {
            _rumbleLast = now;
        } else {
            unsigned int samples = (unsigned int)((now - _rumbleLast) * _rumbleRate);
            if (samples) {
                _rumbleCurrent += samples;
                _rumbleLast = now;
                _rumbleDirty = true;
                if (_rumbleLoop)
                    _rumbleCurrent %= _rumbleHi;
                else if (_rumbleCurrent >= _rumbleHi)
                    _rumblePlaying = false;
            }
        }
    }
    _XINPUT_STATE state;
    if (XInputGetState(_handle, &state) == 0) {
        _previous = _current;
        if (state.dwPacketNumber != _lastPacketNum) {
            _current = state.Gamepad;
            _lastPacketNum = state.dwPacketNumber;
        }
        if (_rumbleDirty && _feedback.Header.dwStatus != 0x3E5) {
            if (_rumblePlaying && !_rumblePaused) {
                unsigned short* sample = &_rumblePattern[_rumbleCurrent * 2];
                _feedback.Rumble.wLeftMotorSpeed = sample[0];
                _feedback.Rumble.wRightMotorSpeed = sample[1];
            } else {
                _feedback.Rumble.wLeftMotorSpeed = 0;
                _feedback.Rumble.wRightMotorSpeed = 0;
            }
            XInputSetState(_handle, &_feedback);
            _rumbleDirty = false;
        }
        if (g_swamp->GetInput()._pipeJpadToDpad) {
            float x = _current.sThumbLX * (1.0f / 32767.0f);
            float y = _current.sThumbLY * (1.0f / 32767.0f);
            if (x > 0.5f)
                _current.wButtons |= 8;
            else if (x < -0.5f)
                _current.wButtons |= 4;
            if (y > 0.5f)
                _current.wButtons |= 1;
            else if (y < -0.5f)
                _current.wButtons |= 2;
        }
        SwampInput& input = g_swamp->GetInput();
        if (input._autoRepeatEnabled) {
            if ((_buttonsHeld ^ _current.wButtons) & 0xF) {
                _buttonsHeld = _current.wButtons;
                _autoRepeatTimer = g_swamp->GetInput()._autoRepeatStart;
            } else {
                _autoRepeatTimer -= input._actualElapsed;
                if (_autoRepeatTimer <= 0.0f) {
                    _previous.wButtons &= ~0xF;
                    _autoRepeatTimer = g_swamp->GetInput()._autoRepeatRate;
                }
            }
        }
    } else {
        XInputClose(_handle);
        _handle = 0;
        _rumblePlaying = false;
    }
}

// FUNCTION: USA 0x000ADE2A
// PROTOTYPE: PREVIEW 0x00498CB1 identical
// NAME: proto-pdb:default.pdb
void SwampInput::PlayRumble(int player, const ResourceHandle<RumbleResource>& rumble)
{
    if (player == -1) {
        for (int i = 0; i < 4; i++) {
            if (_pads[i])
                _pads[i]->PlayRumble(rumble);
        }
    } else if (_activePad[player] > 0 && _pads[_activePad[player] - 1]) {
        _pads[_activePad[player] - 1]->PlayRumble(rumble);
    }
}

// FUNCTION: USA 0x000ADE70
// PROTOTYPE: PREVIEW 0x00498CF7 identical
// NAME: proto-pdb:default.pdb
void SwampInput::Update()
{
    int i;
    if (!_playback) {
        double last_time = _sessionTimeInternal;
        _sessionTimeInternal = BeepLib::GetTime() - _createTime;
        _actualElapsed = _sessionTimeInternal - last_time;
        if (!_pauseGame) {
            // !FAKE: c1xx front end; neutral stand-in local; retail adds to _gameTimeInternal before storing _elapsed;
            // the (double) compare gives retail's `fld K; fld st(1); fcompp`
            const double e = (_frameMaximum < (double)_actualElapsed ? _frameMaximum : _actualElapsed) * _simSpeed;
            _gameTimeInternal += e;
            _elapsed = e;
        } else {
            _elapsed = 0.0f;
        }
        _devChanges = false;
        if (!_ignoreInput) {
            unsigned long insertions;
            unsigned long removals;
            if (XGetDeviceChanges(&XDEVICE_TYPE_GAMEPAD_TABLE, &insertions, &removals)) {
                for (i = 0; i < 4; i++) {
                    if (removals & (1 << i)) {
                        delete _pads[i];
                        _pads[i] = 0;
                        _devChanges = true;
                    }
                    if (insertions & (1 << i)) {
                        _pads[i] = new("SwampInput.cpp", 0x2C5) SwampGamePad(i);
                        _devChanges = true;
                    }
                }
            }
            for (i = 0; i < 4; i++) {
                if (_pads[i])
                    _pads[i]->Update();
            }
            if (_activePad[0] == 0)
                FindActivePad(0);
        } else {
            _XINPUT_GAMEPAD gp;
            memset(&gp, 0, sizeof(gp));
            for (i = 0; i < 4; i++) {
                if (_pads[i])
                    _pads[i]->SetState(gp);
            }
        }
        if (_recording && !_pauseInput)
            AddRecord();
    } else if (!_pauseInput) {
        // !FAKE: c1xx front end; reference locals; the proto PDB lists no record or pad local (stand-ins)
        const InputRecord& p = GetRecord();
        _sessionTimeInternal = p.sessionTime;
        _elapsed = p.elapsed;
        _gameTimeInternal += _elapsed;
        SwampGamePad* v1 = (SwampGamePad*)GetPad(0);
        v1->_previous = v1->_current;
        v1->_current = p.padState;
    } else {
        _elapsed = 0.0f;
    }
    _gameTimePublic = _gameTimeInternal;
    _sessionTimePublic = _sessionTimeInternal;
}

// FUNCTION: USA 0x000AE197
// PROTOTYPE: PREVIEW 0x0049901E identical
// NAME: proto-pdb:default.pdb
void SwampInput::SetGameTime(float t)
{
    Update();
    _gameTimePublic = t;
    _gameTimeInternal = t;
}
