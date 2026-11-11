// UNIT: src/Apps/Vince/scripts/Crypt_city/VinceCannon.cpp OBJ: Xbox\OfficialRelease\VinceCannon.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    unsigned int _pad00[7];            // 0x00
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class SwampScript;           // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb

class VinceCannon { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x0005E38A
// NAME: proto-pdb:default.pdb
VinceCannon::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("VinceCannon", creationFunc);
}
