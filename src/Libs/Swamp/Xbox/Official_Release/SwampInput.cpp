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
extern "C" void* __cdecl memset(void*, int, unsigned int); // NAME: proto-pdb:default.pdb
#pragma intrinsic(memset)

typedef struct _XPP_DEVICE_TYPE { // NAME: proto-pdb:default.pdb
    unsigned long Reserved[3];         // 0x00
} XPP_DEVICE_TYPE;
DECOMP_SIZE_ASSERT(XPP_DEVICE_TYPE, 0xC);
extern "C" XPP_DEVICE_TYPE g_DeviceType_Gamepad;
extern "C" void* __stdcall XInputOpen(XPP_DEVICE_TYPE*, unsigned long, unsigned long, void*); // NAME: proto-pdb:default.pdb

typedef struct _XINPUT_GAMEPAD { // NAME: proto-pdb:default.pdb
    unsigned short wButtons;           // 0x00
    unsigned char bAnalogButtons[8];   // 0x02
    short sThumbLX;                    // 0x0A
    short sThumbLY;                    // 0x0C
    short sThumbRX;                    // 0x0E
    short sThumbRY;                    // 0x10
} XINPUT_GAMEPAD;
DECOMP_SIZE_ASSERT(XINPUT_GAMEPAD, 0x12);

namespace BeepLib { // NAME: proto-pdb:default.pdb
double GetTime();
}

class RumbleResource; // NAME: proto-pdb:default.pdb

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
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

class SwampGamePad { // NAME: proto-pdb:default.pdb
public:
    SwampGamePad(int slot);
    void PlayRumble(const ResourceHandle<RumbleResource>& rumble);
    ~SwampGamePad();
    void SetState(const _XINPUT_GAMEPAD& newState);
    bool IsAnyButton() const;

    void* _handle;                     // 0x00
    unsigned long _lastPacketNum;      // 0x04
    bool _ignoreNextUpdate;            // 0x08
    bool _rumbleDirty;                 // 0x09
    bool _rumblePlaying;               // 0x0A
    bool _rumbleLoop;                  // 0x0B
    bool _rumblePaused;                // 0x0C
    unsigned short _buttonsHeld;       // 0x0E
    float _autoRepeatTimer;            // 0x10
    char field_14[0x14];               // 0x14
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
        char field_C[0x14];            // 0x0C
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

private:
    const InputRecord& GetRecord();

public:

    char field_0[0x8];                 // 0x00
    float _gameTimePublic;             // 0x08
    float _sessionTimePublic;          // 0x0C
    double _gameTimeInternal;          // 0x10
    double _sessionTimeInternal;       // 0x18
    double _createTime;                // 0x20
    char field_28[0xC];                // 0x28
    bool _pauseGame;                   // 0x34
    char field_35[0x1];                // 0x35
    bool _recording;                   // 0x36
    bool _playback;                    // 0x37
    char field_38[0x1];                // 0x38
    bool _pauseInput;                  // 0x39
    char field_3A[0xA];                // 0x3A
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

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1)
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
        _handle = XInputOpen(&g_DeviceType_Gamepad, slot, 0, 0);
}
#endif

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
void SwampGamePad::SetState(const _XINPUT_GAMEPAD& newState)
{
    _previous = _current;
    _current = newState;
    _ignoreNextUpdate = true;
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

// FUNCTION: USA 0x000AE197
// PROTOTYPE: PREVIEW 0x0049901E identical
// NAME: proto-pdb:default.pdb
void SwampInput::SetGameTime(float t)
{
    Update();
    _gameTimePublic = t;
    _gameTimeInternal = t;
}
