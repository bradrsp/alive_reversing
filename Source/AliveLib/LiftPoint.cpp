#include "stdafx.h"
#include "LiftPoint.hpp"
#include "Function.hpp"
#include "Rope.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "Game.hpp"

struct LiftPointData
{
    int field_0_maxW;
    int field_4_maxH;
    int field_8_platfom_animation_offset;
    int field_C_pulley_animation_offset;
    int field_10_frameTableOffset;
    int field_14_maxW;
    int field_18_maxH;
};
ALIVE_ASSERT_SIZEOF(LiftPointData, 0x1C);

const LiftPointData sLiftPointData_545AC8[18] =
{
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1312, 135, 17, 8664, 8700, 69, 34 },
    { 1312, 135, 17, 8664, 8700, 69, 34 },
    { 1312, 135, 17, 8664, 8700, 69, 34 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1312, 135, 17, 8664, 8700, 69, 34 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1312, 135, 17, 8664, 8700, 69, 34 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};

const TintEntry sLiftTints_55BF50[18] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 127u, 127u, 127u },
    { 3u, 127u, 127u, 127u },
    { 4u, 127u, 127u, 127u },
    { 5u, 127u, 127u, 127u },
    { 6u, 107u, 107u, 107u },
    { 7u, 127u, 127u, 127u },
    { 8u, 127u, 127u, 127u },
    { 9u, 127u, 127u, 127u },
    { 10u, 127u, 127u, 127u },
    { 11u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};

void PlatformBase::AddDynamicCollision_4971C0(int /*maxW*/, int /*maxH*/, unsigned __int16 /*frameTableOffset*/, BYTE** /*ppAnimData*/, Path_TLV* /*pTlv*/, Map* /*pMap*/, int /*tlvInfo*/)
{
    NOT_IMPLEMENTED();
}

LiftPoint* LiftPoint::ctor_461030(Path_LiftPoint* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(&field_13C_pulleyAnim, 0x544290); // gVtbl_animation_2a_544290;
    SetVTable(&field_1D4_anim2, 0x544290); // gVtbl_animation_2a_544290;
    SetVTable(this, 0x545CC0); // vTbl_LiftPoint_545CC0

    field_C_objectId = tlvInfo;
    field_4_typeId = Types::eLiftPoint_78;

    pTlv->field_1_unknown = 3;

    field_27C_pTlv = sPath_dword_BB47C0->sub_4DB7C0(pTlv);

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kLiftResID);
    if (pTlv->field_18_scale)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    const LiftPointData& rData = sLiftPointData_545AC8[static_cast<DWORD>(gMap_5C3030.sCurrentLevelId_5C3030)];
    AddDynamicCollision_4971C0(
        rData.field_0_maxW,
        rData.field_4_maxH,
        static_cast<WORD>(rData.field_8_platfom_animation_offset),
        ppRes,
        pTlv,
        &gMap_5C3030,
        tlvInfo);

    if (field_CC_sprite_scale == FP_FromInteger(1))
    {
        field_20_animation.field_C_render_layer = 25;
    }
    else
    {
        field_20_animation.field_C_render_layer = 6;
        field_124_pCollisionLine->field_8_type = 36; // TODO: Type
    }

    SetTint_425600(sLiftTints_55BF50, gMap_5C3030.sCurrentLevelId_5C3030);

    const FP oldX = field_B8_xpos;
    MapFollowMe_408D10(TRUE);
    const short xSnapDelta = FP_GetExponent(field_B8_xpos - oldX);
    field_11C -= xSnapDelta;
    field_11E -= xSnapDelta;

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);


    BYTE** ppPulleyAnim = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kPulleyResID);
    const LiftPointData& rPulleyData = sLiftPointData_545AC8[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)];
    if (field_13C_pulleyAnim.Init_40A030(
        rPulleyData.field_C_pulley_animation_offset,
        gObjList_animations_5C1A24,
        this,
        static_cast<WORD>(rPulleyData.field_14_maxW),
        static_cast<WORD>(rPulleyData.field_18_maxH),
        ppPulleyAnim,
        1u,
        0,
        0))
    {
        if (pTlv->field_18_scale)
        {
            field_13C_pulleyAnim.field_C_render_layer = 6;
            field_13C_pulleyAnim.field_14_scale = field_CC_sprite_scale;
        }
        else
        {
            field_13C_pulleyAnim.field_C_render_layer = 25;
            field_13C_pulleyAnim.field_14_scale = field_CC_sprite_scale;
        }

        BYTE** ppAbeLiftRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeliftResID, TRUE, FALSE);

        field_13C_pulleyAnim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_13C_pulleyAnim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_13C_pulleyAnim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);

        field_12C &= ~1u;

        field_280_flags.Clear(LiftFlags::eBit1_bTopFloor);
        field_280_flags.Clear(LiftFlags::eBit2_bMiddleFloor);
        field_280_flags.Clear(LiftFlags::eBit3_bBottomFloor);
        field_280_flags.Clear(LiftFlags::eBit5);

        field_274_ppRes = ppAbeLiftRes;

        field_13C_pulleyAnim.field_9_g = static_cast<BYTE>(field_D2_g);
        field_13C_pulleyAnim.field_8_r = static_cast<BYTE>(field_D4_b);
        field_13C_pulleyAnim.field_A_b = static_cast<BYTE>(field_D0_r);
        field_13C_pulleyAnim.field_B_render_mode = 0;

        field_C4_velx = FP_FromInteger(0);
        field_C8_vely = FP_FromInteger(0);

        const FP k25 = FP_FromInteger(25);
        const FP k13 = FP_FromInteger(13);
        const FP km10 = FP_FromInteger(-10);


        auto pRope1 = alive_new<Rope>();
        if (pRope1)
        {
            pRope1->ctor_4A0A70(
                FP_GetExponent((k13 * field_CC_sprite_scale + field_B8_xpos)),
                0, // Start at the very top of the screen
                FP_GetExponent((k25 * field_CC_sprite_scale) + field_BC_ypos),
                field_CC_sprite_scale);

            field_138_rope1_id = pRope1->field_8_object_id;
        }

        auto pRope2 = alive_new<Rope>();
        if (pRope2)
        {
            pRope2->ctor_4A0A70(
                FP_GetExponent((km10 * field_CC_sprite_scale) + field_B8_xpos),
                0, // Start at the very top of the screen
                FP_GetExponent((k25 * field_CC_sprite_scale) + field_BC_ypos),
                field_CC_sprite_scale);
            field_134_rope2_id = pRope2->field_8_object_id;
        }

        pRope2->field_106_bottom = FP_GetExponent((k25 * field_CC_sprite_scale) + FP_FromInteger(field_124_pCollisionLine->field_2_y1));
        pRope1->field_106_bottom = FP_GetExponent((k25 * field_CC_sprite_scale) + FP_FromInteger(field_124_pCollisionLine->field_2_y1));

        const FP v28 = field_BC_ypos * FP_FromDouble(1.5);
        const FP v29 = FP_FromRaw(FP_GetExponent(v28 * field_CC_sprite_scale) % FP_FromInteger(pRope2->field_F6_rope_length).fpValue);

        pRope2->field_BC_ypos = FP_FromInteger(FP_GetExponent(v29 + (k25* field_CC_sprite_scale) + field_BC_ypos + FP_FromInteger(pRope2->field_F6_rope_length)));
        pRope1->field_BC_ypos = FP_FromInteger(FP_GetExponent((k25 * field_CC_sprite_scale) + field_BC_ypos + FP_FromInteger(pRope1->field_F6_rope_length) - v29));

        field_280_flags.Clear(LiftFlags::eBit4);
        sub_462C80();

        field_278_lift_point_id = static_cast<char>(pTlv->field_10_id);
        field_130_lift_point_stop_type = pTlv->field_16_lift_point_stop_type;

        switch (field_130_lift_point_stop_type)
        {
        case LiftPointStopType::eTopFloor_0:
            field_280_flags.Set(LiftFlags::eBit1_bTopFloor);
            break;

        case LiftPointStopType::eBottomFloor_1:
            field_280_flags.Set(LiftFlags::eBit3_bBottomFloor);
            break;

        case LiftPointStopType::eMiddleFloor_2:
            field_280_flags.Set(LiftFlags::eBit2_bMiddleFloor);
            break;

        default:
        case LiftPointStopType::eMiddleLockFloor_3:  // Not used ??
        case LiftPointStopType::eStartPointOnly_4:
            field_130_lift_point_stop_type = LiftPointStopType::eStartPointOnly_4;
            break;
        }

        field_280_flags.Clear(LiftFlags::eBit7);
        field_280_flags.Set(LiftFlags::eBit6);
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eListAddFailed);
    }
    return this;
}

void LiftPoint::sub_462C80()
{
    NOT_IMPLEMENTED();
}
