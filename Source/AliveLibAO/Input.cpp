#include "stdafx_ao.h"
#include "Input.hpp"
#include "Function.hpp"

namespace AO {

ALIVE_VAR(1, 0x5009E8, InputObject, sInputObject_5009E8, {});
ALIVE_VAR(1, 0x5076B8, unsigned __int16, sCurrentControllerIndex_5076B8, 0);
ALIVE_VAR(1, 0x508A60, int, sJoystickEnabled_508A60, 0);

const InputCommands sInputKey_Right_4C6590 = eRight;
const InputCommands sInputKey_Left_4C6594 = eLeft;
const InputCommands sInputKey_Up_4C6598 = eUp;
const InputCommands sInputKey_Down_4C659C = eDown;
const InputCommands sInputKey_Hop_4C65A0 = eHop;
const InputCommands sInputKey_DoAction_4C65A4 = eDoAction;
const InputCommands sInputKey_Run_4C65A8 = eRun;
const InputCommands sInputKey_Sneak_4C65AC = eSneak;
const InputCommands sInputKey_FartRoll_4C65B0 = eFartOrRoll;
const InputCommands sInputKey_ThrowItem_4C65B4 = eThrowItem;

const InputCommands sInputKey_LeftGameSpeakEnabler_4C65B8 = eGameSpeak6;
const InputCommands sInputKey_GameSpeak1_4C65C8 = eHop;
const InputCommands sInputKey_GameSpeak2_4C65BC = eDoAction;
const InputCommands sInputKey_GameSpeak3_4C65C0 = eThrowItem;
const InputCommands sInputKey_GameSpeak4_4C65C4 = eFartOrRoll;

const InputCommands sInputKey_RightGameSpeakEnabler_4C65DC = eGameSpeak3;
const InputCommands sInputKey_GameSpeak5_4C65EC = eFartOrRoll;
const InputCommands sInputKey_GameSpeak6_4C65E8 = eHop;
const InputCommands sInputKey_GameSpeak7_4C65E4 = eThrowItem;
const InputCommands sInputKey_GameSpeak8_4C65E0 = eDoAction;

EXPORT void InputObject::InitPad_4331A0(unsigned int /*padCount*/)
{
    NOT_IMPLEMENTED();

    for (PSX_Pad& pad : field_0_pads)
    {
        pad = {};
    }

    field_20_demo_playing &= ~1u;
}

EXPORT void InputObject::Update_433250()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC InputObject::Shutdown_433230()
{
    NOT_IMPLEMENTED();
}


void InputObject::SetDemoRes_433470(BYTE** ppDemoRes)
{
    field_1C_demo_command_index = 2051;
    field_18_demo_res = ppDemoRes;
    field_20_demo_playing |= 1u;
    field_28_command_duration = 0;
}

int InputObject::IsDemoPlaying_4334A0()
{
    return field_20_demo_playing & 1;
}

bool InputObject::IsPressed(DWORD command)
{
    return (field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed & command) != 0;
}

bool InputObject::IsHeld(DWORD command)
{
    return (field_0_pads[sCurrentControllerIndex_5076B8].field_6_held & command) != 0;
}

bool InputObject::IsReleased(DWORD command)
{
     return (field_0_pads[sCurrentControllerIndex_5076B8].field_8_released & command) != 0;
}

BOOL CC Input_IsChanting_4334C0()
{
    return (sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed & sInputKey_Chant) == sInputKey_Chant;
}

void CC Input_SetKeyState_48E610(int /*key*/, char /*bIsDown*/)
{
    NOT_IMPLEMENTED();
}

EXPORT char CC Input_IsVKPressed_48E5D0(int /*key*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC Input_Init_44EB60()
{
    NOT_IMPLEMENTED();
}


EXPORT void Input_DisableInput_48E690()
{
    NOT_IMPLEMENTED();
}

EXPORT void Input_GetCurrentKeyStates_48E630()
{
    NOT_IMPLEMENTED();
}

void Input_InitKeyStateArray_48E5F0()
{
    NOT_IMPLEMENTED();
}

}
