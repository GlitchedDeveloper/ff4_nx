#include "vibration.h"

#include <switch.h>

#include "config.h"
#include "pad_manager.h"

namespace vibration {

constexpr int MAX_DEVICES = pad_manager::MAX_PLAYERS + 1;
HidVibrationDeviceHandle g_Devices[MAX_DEVICES][2];

void init() {
    hidInitializeVibrationDevices(g_Devices[0], 2, HidNpadIdType_Handheld, HidNpadStyleSet_NpadStandard);
    hidInitializeVibrationDevices(g_Devices[1], 2, HidNpadIdType_No1, HidNpadStyleSet_NpadStandard);
    hidInitializeVibrationDevices(g_Devices[2], 2, HidNpadIdType_No2, HidNpadStyleSet_NpadStandard);
    hidInitializeVibrationDevices(g_Devices[3], 2, HidNpadIdType_No3, HidNpadStyleSet_NpadStandard);
    hidInitializeVibrationDevices(g_Devices[4], 2, HidNpadIdType_No4, HidNpadStyleSet_NpadStandard);
    hidInitializeVibrationDevices(g_Devices[5], 2, HidNpadIdType_No5, HidNpadStyleSet_NpadStandard);
}

void set_rumble(float low, float high) {
    HidVibrationValue value;
    value.freq_low  = 160.0f;
    value.freq_high = 320.0f;
    value.amp_low   = low;
    value.amp_high  = high;
    for (u8 i = 0; i < MAX_DEVICES; i++) {
        hidSendVibrationValue(g_Devices[i][0], &value);
        hidSendVibrationValue(g_Devices[i][1], &value);
    }
}
}