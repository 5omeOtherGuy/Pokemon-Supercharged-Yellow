#ifndef GUARD_SC_TITLE_CONTROLS_H
#define GUARD_SC_TITLE_CONTROLS_H

#include "gba/io_reg.h"

enum ScTitleAction
{
    SC_TITLE_WAIT,
    SC_TITLE_ENTER,
    SC_TITLE_CLEAR_SAVE,
    SC_TITLE_RESET_RTC,
    SC_TITLE_RESTART,
};

// Maintenance chords keep their original precedence over ordinary entry/idle.
static inline enum ScTitleAction ScTitleChooseAction(unsigned held, unsigned pressed,
                                                     unsigned canResetRtc, unsigned idle)
{
    const unsigned clear = B_BUTTON | SELECT_BUTTON | DPAD_UP;
    const unsigned rtc = B_BUTTON | SELECT_BUTTON | DPAD_LEFT;
    if ((held & clear) == clear)
        return SC_TITLE_CLEAR_SAVE;
    if ((held & rtc) == rtc && canResetRtc)
        return SC_TITLE_RESET_RTC;
    if (pressed & (A_BUTTON | START_BUTTON))
        return SC_TITLE_ENTER;
    if (idle)
        return SC_TITLE_RESTART;
    return SC_TITLE_WAIT;
}

#endif
