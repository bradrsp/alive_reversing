#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Primitives.hpp"

namespace AO {

struct Path_MotionDetector : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_device_x;
    __int16 field_1C_device_y;
    __int16 field_1E_speed_x256;
    enum class StartMoveDirection : __int16
    {
        eRight_0 = 0,
        eLeft_1 = 1
    };
    StartMoveDirection field_20_start_move_direction;
    __int16 field_22_draw_flare;
    __int16 field_24_disable_id;
    __int16 field_26_alarm_id;
    __int16 field_28_alarm_ticks;
    __int16 field_2A_padding;
};
ALIVE_ASSERT_SIZEOF(Path_MotionDetector, 0x2C);

class MotionDetectorLaser : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual void VScreenChanged() override
    {
        // Empty
    }

    virtual BaseGameObject* VDestructor(signed int flags) override;


    int field_D4[4];
};
ALIVE_ASSERT_SIZEOF(MotionDetectorLaser, 0xE4);

class MotionDetector : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT MotionDetector* ctor_437A50(Path_MotionDetector* pTlv, int tlvInfo);

    EXPORT void SetDontComeBack_437E00(__int16 bDontComeBack);

    EXPORT BaseGameObject* dtor_437D70();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_438520();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT MotionDetector* Vdtor_438530(signed int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_437E90();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_438250(PrimHeader** ppOt);

    int field_D4_padding[4];
    int field_E4_tlvInfo;
    enum class States : __int16
    {
        eMoveRight_0 = 0,
        eWaitThenMoveLeft_1 = 1,
        eMoveLeft_2 = 2,
        eWaitThenMoveRight_3 = 3
    };
    States field_E8_state;
    __int16 field_EA_pad;
    int field_EC_timer;
    unsigned __int16 field_F0_disable_id;
    __int16 field_F2_alarm_trigger;
    __int16 field_F4_alarm_time;
    __int16 field_F6_bDontComeBack;
    FP field_F8_top_left_x;
    FP field_FC_top_left_y;
    FP field_100_bottom_right_x;
    FP field_104_bottom_right_y;
    MotionDetectorLaser* field_108_pLaser;
    Poly_F3 field_10C_prims[2];
    Prim_SetTPage field_13C_tPage[2];
    FP field_15C_speed;
    __int16 field_160_bObjectInLaser;
    __int16 field_162_pad;
};
ALIVE_ASSERT_SIZEOF(MotionDetector, 0x164);

}

