#include "stdafx.h"
#include "SlamDoor.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "Sfx.hpp"

struct SlamDoor_Data
{
    int field_0_frameTableOffset;
    int field_4_ppRes;
    int field_8_frameTableOffset;
    __int16 field_C_maxH;
    __int16 field_E_maxW;
};

SlamDoor_Data sSlamDoorData_547168[18] =
{
    { 2672, 2688, 2640, 29, 68 },
    { 2672, 2688, 2640, 29, 68 },
    { 2368, 2384, 2396, 32, 68 },
    { 2368, 2384, 2396, 32, 68 },
    { 2368, 2384, 2396, 32, 68 },
    { 2672, 2688, 2640, 29, 68 },
    { 2672, 2688, 2640, 29, 68 },
    { 2368, 2384, 2396, 32, 68 },
    { 2672, 2688, 2640, 29, 68 },
    { 2672, 2688, 2640, 29, 68 },
    { 2672, 2688, 2640, 29, 68 },
    { 2368, 2384, 2396, 32, 68 },
    { 2672, 2688, 2640, 29, 68 },
    { 2672, 2688, 2640, 29, 68 },
    { 2672, 2688, 2640, 29, 68 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 }
};

TintEntry sSlamDoorTints_5603B0[18] =
{
    { 1, 102u, 87u, 118u },
    { 2, 102u, 87u, 118u },
    { 3, 102u, 87u, 118u },
    { 4, 102u, 87u, 118u },
    { 5, 102u, 87u, 118u },
    { 6, 102u, 87u, 118u },
    { 7, 102u, 87u, 118u },
    { 8, 102u, 87u, 118u },
    { 9, 102u, 87u, 118u },
    { 0x0A, 102u, 87u, 118u },
    { 0x0B, 102u, 87u, 118u },
    { 0x0C, 102u, 87u, 118u },
    { 0x0D, 102u, 87u, 118u },
    { 0x0E, 102u, 87u, 118u },
    { -1, 102u, 87u, 118u },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};


void SlamDoor::VUpdate()
{
    vUpdate_4AFD50();
}

SlamDoor * SlamDoor::ctor_4AF700(Path_SlamDoor * pTlv, TlvItemInfoUnion tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x547288);
    field_C_objectId = tlvInfo.all; // todo: check this
    field_6_flags.Set(Options::eCanExplode);

    field_128_switch_id = pTlv->field_14_id;

    field_118_flags.Clear(SlamDoor_Flags_118::e118_Bit2_Open);
    field_118_flags.Clear(SlamDoor_Flags_118::e118_Bit4_Inverted);
    field_118_flags.Clear(SlamDoor_Flags_118::e118_Bit5_Delete);

    if (!pTlv->field_10_starts_shut)
    {
        field_118_flags.Set(SlamDoor_Flags_118::e118_Bit2_Open);
    }
    
    if (pTlv->field_16_inverted)
    {
        field_118_flags.Set(SlamDoor_Flags_118::e118_Bit4_Inverted);
    }

    if (pTlv->field_18_delete)
    {
        field_118_flags.Set(SlamDoor_Flags_118::e118_Bit5_Delete);
    }

    const int currentLevelId = static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030);

    Animation_Init_424E10(
        sSlamDoorData_547168[currentLevelId].field_8_frameTableOffset,
        sSlamDoorData_547168[currentLevelId].field_C_maxH,
        sSlamDoorData_547168[currentLevelId].field_E_maxW,
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 2020),
        1,
        1u);

    field_4_typeId = Types::eSlamDoor_122;

    field_B8_xpos = FP_FromInteger(((pTlv->field_8_top_left.field_0_x
        + pTlv->field_C_bottom_right.field_0_x)
        / 2));

    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_12C_tlvInfo = tlvInfo;

    if (pTlv->field_12_half_scale && pTlv->field_12_half_scale == 1)
    {
            field_CC_sprite_scale = FP_FromDouble(0.5);
            field_20_animation.field_C_render_layer = 6;
            field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromDouble(1.0);
        field_20_animation.field_C_render_layer = 25;
        field_D6_scale = 1;
    }

    if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit4_Inverted))
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit6_FlipY);
        field_D8_yOffset = FP_GetExponent(field_CC_sprite_scale * FP_FromDouble(-68.0));
    }

    int switchState = SwitchStates_Get_466020(field_128_switch_id);
    int bitIsOpen = static_cast<int>(field_118_flags.Get(SlamDoor_Flags_118::e118_Bit2_Open));

    if (switchState == bitIsOpen)
    {
        field_118_flags.Set(SlamDoor_Flags_118::e118_Bit1);
    }
    else
    {
        field_118_flags.Clear(SlamDoor_Flags_118::e118_Bit1);
    }

    SetTint_425600(sSlamDoorTints_5603B0, gMap_5C3030.sCurrentLevelId_5C3030);

    FP hitX;
    FP hitY;

    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromDouble(24.0),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        field_BC_ypos = hitY;
    }

    FP v15;
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        v15 = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromDouble(2.0)) + FP_FromInteger(FP_GetExponent(field_B8_xpos));
    }
    else
    {
        v15 = FP_FromInteger(FP_GetExponent(field_B8_xpos)) - (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromDouble(2.0));
    }

    field_124_x1 = FP_GetExponent(v15);
    field_126_y1 = FP_GetExponent(field_BC_ypos);

    if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1))
    {
        PathLine * pPathLine = nullptr;

        if (field_CC_sprite_scale == FP_FromDouble(1.0))
        {
            const FP lineHeight = FP_FromDouble(80.0);

            field_11C_pCollisionLine_6_2 = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line_417FA0(
                field_124_x1,
                FP_GetExponent(FP_FromInteger(field_126_y1) - lineHeight),
                field_124_x1,
                field_126_y1,
                2);
            const FP x2 = FP_FromInteger(field_124_x1) + ScaleToGridSize_4498B0(field_CC_sprite_scale);
            const FP y1 = FP_FromInteger(field_126_y1)
                - (field_CC_sprite_scale * FP_FromDouble(80.0));
            const FP x1 = ScaleToGridSize_4498B0(field_CC_sprite_scale) + FP_FromInteger(field_124_x1);
            pPathLine = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line_417FA0(
                FP_GetExponent(x1),
                FP_GetExponent(y1),
                FP_GetExponent(x2),
                field_126_y1,
                1);
        }
        else
        {
            const FP lineHeight = field_CC_sprite_scale * FP_FromDouble(80.0);

            field_11C_pCollisionLine_6_2 = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line_417FA0(
                field_124_x1,
                FP_GetExponent(FP_FromInteger(field_126_y1) - lineHeight),
                field_124_x1,
                field_126_y1,
                6);
            const FP x2 = FP_FromInteger(field_124_x1) + ScaleToGridSize_4498B0(field_CC_sprite_scale);
            const FP y1 = FP_FromInteger(field_126_y1);
            -(field_CC_sprite_scale * FP_FromDouble(80.0));
            const FP x1 = ScaleToGridSize_4498B0(field_CC_sprite_scale) + FP_FromInteger(field_124_x1);
            pPathLine = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line_417FA0(
                FP_GetExponent(x1),
                FP_GetExponent(y1),
                FP_GetExponent(x2),
                field_126_y1,
                5);
        }
        field_120_pCollisionLine_5_1 = pPathLine;
        field_20_animation.Set_Animation_Data_409C80(
            sSlamDoorData_547168[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_4_ppRes,
            0);
    }
    else
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_11C_pCollisionLine_6_2 = 0;
        field_120_pCollisionLine_5_1 = 0;
    }

    field_118_flags.Set(SlamDoor_Flags_118::e118_Bit3);
    field_DC_bApplyShadows |= 2u;

    return this;
}

void SlamDoor::vUpdate_4AFD50()
{
    NOT_IMPLEMENTED();

    bool stateUnchanged; // edi
    PSX_RECT *v5; // edx
    PSX_RECT *v6; // eax
    PSX_Point v7; // ebp
    FP v8; // edi
    PSX_RECT *v11; // eax
    int v14; // eax
    PathLine *pPathLine; // eax
    PSX_RECT *v27; // eax
    PSX_RECT *v30; // eax
    PSX_Point v31; // [esp+10h] [ebp-20h]
    PSX_Point a4; // [esp+14h] [ebp-1Ch]
    __int16 idx; // [esp+24h] [ebp-Ch]
    PSX_RECT bRect; // [esp+28h] [ebp-8h]
    idx = 0;
    if (Event_Get_422C00(kEventDeathReset))
    {
        this->field_6_flags.Set(Options::eDead);
    }
    stateUnchanged = SwitchStates_Get_466020(this->field_128_switch_id) == static_cast<int>(field_118_flags.Get(SlamDoor_Flags_118::e118_Bit2_Open));
    if (!(field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1)))
    {
        if (this->field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (this->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
            {
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render); // todo: check
                if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit5_Delete))
                {
                    this->field_6_flags.Set(Options::eDead);
                }
                SFX_Play_46FBA0(0x39u, 100, 900, 0x10000);
                SFX_Play_46FBA0(0x39u, 100, -100, 0x10000);
                this->field_118_flags.Set(SlamDoor_Flags_118::e118_Bit3);
            }
        }
    }
    if (this->field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1))
    {
        if (this->field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (!field_118_flags.Get(SlamDoor_Flags_118::e118_Bit3))
            {
                field_118_flags.Set(SlamDoor_Flags_118::e118_Bit3);
                SFX_Play_46FBA0(0x39u, 100, 900, 0x10000);
                SFX_Play_46FBA0(0x39u, 100, -100, 0x10000);
            }
        }
    }
    if (stateUnchanged != field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1))
    {
        // TODO: figure out and optimize this
        this->field_118_flags.Raw().all = field_118_flags.Raw().all & ~4 ^ (LOBYTE(field_118_flags.Raw().all) & ~0xFF04u ^ (unsigned __int8)~(LOBYTE(field_118_flags.Raw().all) & ~4)) & 1;

        if (stateUnchanged)
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render); 

            field_20_animation.Set_Animation_Data_409C80(
                sSlamDoorData_547168[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_8_frameTableOffset,
                0);

            if (field_CC_sprite_scale == FP_FromDouble(1.0))
            {
                const FP lineHeight = FP_FromDouble(80.0);

                field_11C_pCollisionLine_6_2 = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line_417FA0(
                    field_124_x1,
                    FP_GetExponent(FP_FromInteger(field_126_y1) - lineHeight),
                    field_124_x1,
                    field_126_y1,
                    2);
                const FP x2 = FP_FromInteger(field_124_x1) + ScaleToGridSize_4498B0(field_CC_sprite_scale);
                const FP y1 = FP_FromInteger(field_126_y1)
                    - (field_CC_sprite_scale * FP_FromDouble(80.0));
                const FP x1 = ScaleToGridSize_4498B0(field_CC_sprite_scale) + FP_FromInteger(field_124_x1);
                pPathLine = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line_417FA0(
                    FP_GetExponent(x1),
                    FP_GetExponent(y1),
                    FP_GetExponent(x2),
                    field_126_y1,
                    1);
            }
            else
            {
                const FP lineHeight = field_CC_sprite_scale * FP_FromDouble(80.0);

                field_11C_pCollisionLine_6_2 = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line_417FA0(
                    field_124_x1,
                    FP_GetExponent(FP_FromInteger(field_126_y1) - lineHeight),
                    field_124_x1,
                    field_126_y1,
                    6);
                const FP x2 = FP_FromInteger(field_124_x1) + ScaleToGridSize_4498B0(field_CC_sprite_scale);
                const FP y1 = FP_FromInteger(field_126_y1);
                -(field_CC_sprite_scale * FP_FromDouble(80.0));
                const FP x1 = ScaleToGridSize_4498B0(field_CC_sprite_scale) + FP_FromInteger(field_124_x1);
                pPathLine = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line_417FA0(
                    FP_GetExponent(x1),
                    FP_GetExponent(y1),
                    FP_GetExponent(x2),
                    field_126_y1,
                    5);
            }

            this->field_120_pCollisionLine_5_1 = pPathLine;
            PSX_RECT v35;
            v27 = vGetBoundingRect_424FD0(
                &v35,
                1);
            if (this->field_118_flags.Get(SlamDoor_Flags_118::e118_Bit4_Inverted))
            {
                v27->x += FP_GetExponent(FP_FromInteger(-110) * field_CC_sprite_scale);
                //LOWORD(v28) = v32; // HMMM?
                v27->w += FP_GetExponent(FP_FromInteger(-110) * field_CC_sprite_scale);
            }
            for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
            {
                BaseAliveGameObject * pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);

                if (!pObj)
                {
                    break;
                }

                if (pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
                {
                    if (pObj->field_4_typeId != Types::eSlamDoor_122)
                    {
                        v30 = pObj->vGetBoundingRect_424FD0(
                            &bRect,
                            1);

                        // Todo: check this strange shit
                        if (v27->y <= v30->w
                            && v27->w >= (v30->x + 3)
                            && v27->w >= v30->y
                            && v27->x <= v30->h
                            && pObj->field_CC_sprite_scale == this->field_CC_sprite_scale)
                        {
                            ClearInsideSlamDoor_4B0530(pObj, v27->y, v27->h);
                        }
                    }
                }
            }
        }
        else
        {
            field_20_animation.Set_Animation_Data_409C80(
                sSlamDoorData_547168[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_0_frameTableOffset,
                0);
            Rect_Clear_418040((PSX_RECT *)this->field_11C_pCollisionLine_6_2);
            v5 = (PSX_RECT *)this->field_120_pCollisionLine_5_1;
            this->field_11C_pCollisionLine_6_2 = 0;
            Rect_Clear_418040(v5);
            this->field_120_pCollisionLine_5_1 = 0;
        }
    }
    if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1))
    {
        v6 = this->vGetBoundingRect_424FD0(
            &bRect,
            1);
        v7 = *(PSX_Point *)&v6->x;
        v31 = *(PSX_Point *)&v6->x;
        a4 = *(PSX_Point *)&v6->w;
        if (this->field_118_flags.Get(SlamDoor_Flags_118::e118_Bit4_Inverted))
        {
            v31.field_2_y += FP_GetExponent(field_CC_sprite_scale * FP_FromInteger(-110));
            v8 = field_CC_sprite_scale * FP_FromInteger(-110);
            v7.field_0_x = v31.field_0_x;
            a4.field_2_y += FP_GetExponent(v8) - FP_GetExponent(FP_FromInteger(20) * field_CC_sprite_scale);
        }

        for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
        {
            BaseAliveGameObject * pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);

            if (!pObj)
            {
                break;
            }

            if (pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
            {
                if (pObj->field_4_typeId != Types::eSlamDoor_122 && pObj->field_4_typeId != Types::eGrenade_65)
                {
                    v11 = pObj->vGetBoundingRect_424FD0(
                        &bRect,
                        1);

                    v14 = FP_GetExponent(pObj->field_B8_xpos);

                    if (v14 > v31.field_0_x
                        && v14 < a4.field_0_x
                        && v31.field_0_x <= v11->w
                        && a4.field_0_x >= v11->x
                        && a4.field_2_y >= v11->y
                        && v31.field_2_y <= v11->h)
                    {
                        if (pObj->field_CC_sprite_scale == field_CC_sprite_scale
                            || pObj->field_4_typeId == Types::eSlog_126 && field_CC_sprite_scale == FP_FromDouble(1.0))
                        {
                            ClearInsideSlamDoor_4B0530(pObj, v7.field_0_x, a4.field_0_x);
                        }
                    }
                }
            }
        }
    }
    if (this->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        field_6_flags.Set(Options::eCanExplode);
    }
    else
    {
        field_6_flags.Clear(Options::eCanExplode);
    }
}

void SlamDoor::ClearInsideSlamDoor_4B0530(BaseAliveGameObject * pObj, __int16 a3, __int16 a4)
{
    if (FP_GetExponent(pObj->field_B8_xpos) - a3 >= a4 - FP_GetExponent(pObj->field_B8_xpos))
    {
        pObj->field_B8_xpos = (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromDouble(0.5)) + FP_FromDouble(1.0) + pObj->field_B8_xpos;
    }
    else
    {
        pObj->field_B8_xpos = pObj->field_B8_xpos - (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromDouble(0.5));
    }

    if (pObj->field_4_typeId == Types::eType_81 || pObj->field_4_typeId == Types::eMudokon_110 || pObj->field_4_typeId == Types::eType_Abe_69)
    {
        pObj->VTakeDamage_408730(this);
    }

    if (pObj->field_4_typeId == Types::eBone_11)
    {
        pObj->field_C8_vely = FP_FromInteger(-7);
        pObj->field_C4_velx = -pObj->field_C4_velx;
    }
}

void SlamDoor_ForceLink()
{
}
