#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

struct ParticleBurst_Item;

class ParticleBurst : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ParticleBurst* ctor_41CF50(FP xpos, FP ypos, unsigned int unknown_count1, FP scale, __int16 type, signed __int16 a7);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;

private:
    EXPORT FP* Random_Speed_41CEE0(FP* random);
    EXPORT ParticleBurst* vdtor_41D4E0(signed int flags);
    EXPORT void dtor_41D510();
    EXPORT void vRender_41D7B0(int** pOt);
    EXPORT void vUpdate_41D590();

private:
    int field_E4_not_used[4];
    BYTE** field_F4_ppRes;
    ParticleBurst_Item* field_F8_pRes;
    __int16 field_FC_count;
    __int16 field_FE_pad;
    int field_100_timer;
    __int16 field_104_type;
    __int16 field_106_count;
};
ALIVE_ASSERT_SIZEOF(ParticleBurst, 0x108);