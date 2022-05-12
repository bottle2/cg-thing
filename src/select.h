#ifndef __T1__SELECT__H__
#define __T1__SELECT__H__

// Handles mouse input regarding images.
// Detects drags and clicks and manages multiselection.

#include "input.h"

enum select_event
{
    SELECT_INVALID_EVENT,
    SELECT_PRESS_OUTSIDE,
    SELECT_PRESS_MULTI_NO_SELECTED,
    SELECT_PRESS_MULTI_NO_UNSELECTED,
    SELECT_PRESS_MULTI_YES_SELECTED,
    SELECT_PRESS_MULTI_YES_UNSELECTED,
    SELECT_MOVE,
    SELECT_RELEASE,
    SELECT_N_STATE,
};

enum select_state
{
    SELECT_INVALID_STATE,
    SELECT_AWAITING,
    SELECT_PRESSED_MULTI_NO_SELECTED,
    SELECT_PRESSED_MULTI_NO_UNSELECTED,
    SELECT_PRESSED_MULTI_YES_SELECTED,
    SELECT_PRESSED_MULTI_YES_UNSELECTED,
    SELECT_MOVING,
    SELECT_N_EVENT,
};

enum select_action
{
    SELECT_INVALID_ACTION,
    SELECT_NOOP,
    SELECT_OFFSET_USE,
    SELECT_OFFSET_APPLY,
    SELECT_DESELECT_ALL,
    SELECT_DESELECT_ALL_EXCEPT,
    SELECT_DESELECT_ONLY,
    SELECT_SELECT_TOO,
    SELECT_N_ACTION,
};

#define SELECT_MACHINE_XS(M, X)                                                                                                              \
X(M, SELECT_AWAITING                    , SELECT_PRESS_OUTSIDE             , SELECT_AWAITING                    , SELECT_DESELECT_ALL       ) \
X(M, SELECT_AWAITING                    , SELECT_PRESS_MULTI_NO_SELECTED   , SELECT_PRESSED_MULTI_NO_SELECTED   , SELECT_NOOP               ) \
X(M, SELECT_AWAITING                    , SELECT_PRESS_MULTI_NO_UNSELECTED , SELECT_PRESSED_MULTI_NO_UNSELECTED , SELECT_DESELECT_ALL_EXCEPT) \
X(M, SELECT_AWAITING                    , SELECT_PRESS_MULTI_YES_SELECTED  , SELECT_PRESSED_MULTI_YES_SELECTED  , SELECT_NOOP               ) \
X(M, SELECT_AWAITING                    , SELECT_PRESS_MULTI_YES_UNSELECTED, SELECT_PRESSED_MULTI_YES_UNSELECTED, SELECT_SELECT_TOO         ) \
X(M, SELECT_AWAITING                    , SELECT_MOVE                      , SELECT_AWAITING                    , SELECT_NOOP               ) \
X(M, SELECT_AWAITING                    , SELECT_RELEASE                   , SELECT_AWAITING                    , SELECT_NOOP               ) \
X(M, SELECT_PRESSED_MULTI_NO_SELECTED   , SELECT_MOVE                      , SELECT_MOVING                      , SELECT_OFFSET_USE         ) \
X(M, SELECT_PRESSED_MULTI_NO_SELECTED   , SELECT_RELEASE                   , SELECT_AWAITING                    , SELECT_DESELECT_ALL       ) \
X(M, SELECT_PRESSED_MULTI_NO_UNSELECTED , SELECT_MOVE                      , SELECT_MOVING                      , SELECT_OFFSET_USE         ) \
X(M, SELECT_PRESSED_MULTI_NO_UNSELECTED , SELECT_RELEASE                   , SELECT_AWAITING                    , SELECT_NOOP               ) \
X(M, SELECT_PRESSED_MULTI_YES_SELECTED  , SELECT_MOVE                      , SELECT_MOVING                      , SELECT_OFFSET_USE         ) \
X(M, SELECT_PRESSED_MULTI_YES_SELECTED  , SELECT_RELEASE                   , SELECT_AWAITING                    , SELECT_DESELECT_ONLY      ) \
X(M, SELECT_PRESSED_MULTI_YES_UNSELECTED, SELECT_MOVE                      , SELECT_MOVING                      , SELECT_OFFSET_USE         ) \
X(M, SELECT_PRESSED_MULTI_YES_UNSELECTED, SELECT_RELEASE                   , SELECT_AWAITING                    , SELECT_NOOP               ) \
X(M, SELECT_MOVING                      , SELECT_MOVE                      , SELECT_MOVING                      , SELECT_OFFSET_USE         ) \
X(M, SELECT_MOVING                      , SELECT_RELEASE                   , SELECT_AWAITING                    , SELECT_OFFSET_APPLY       )

struct select
{
    enum select_state state;

    int mouse_x_pressed;
    int mouse_y_pressed;
    int mouse_x_current;
    int mouse_y_current;

    int offset_x;
    int offset_y;
};

void select_init(struct select *select);

enum select_action select_step(
    struct select    *select,
    int               mouse_x,
    int               mouse_y,
    enum input_state  mouse_state,
    bool              is_hover,
    bool              is_multi,
    bool              is_selected
);

#endif // __T1__SELECT__H__
