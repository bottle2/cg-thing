#include <assert.h>
#include <stddef.h>

#include "expand.h"
#include "input.h"
#include "select.h"

static enum select_state  next_state[ SELECT_N_STATE][SELECT_N_EVENT];
static enum select_action next_action[SELECT_N_STATE][SELECT_N_EVENT];

void select_init(struct select *select)
{
    assert(select != NULL);

    select->state = SELECT_AWAITING;

    SELECT_MACHINE_XS(next_state,  AS_NEXT_STATE);
    SELECT_MACHINE_XS(next_action, AS_NEXT_ACTION);
}

enum select_action select_step(
    struct select    *select,
    int               mouse_x,
    int               mouse_y,
    enum input_state  mouse_state,
    bool              is_hover,
    bool              is_multi,
    bool              is_selected
) {
    assert(select != NULL);

    enum select_event event = SELECT_INVALID_EVENT;

    switch ((int)mouse_state)
    {
        case INPUT_PRESSED:
            select->mouse_x_pressed = mouse_x;
            select->mouse_y_pressed = mouse_y;

            if (is_hover)
            {
                if (is_multi)
                {
                    event = is_selected ? SELECT_PRESS_MULTI_YES_SELECTED : SELECT_PRESS_MULTI_YES_UNSELECTED;
                }
                else
                {
                    event = is_selected ? SELECT_PRESS_MULTI_NO_SELECTED : SELECT_PRESS_MULTI_NO_UNSELECTED;
                }
            }
            else
            {
                event = SELECT_PRESS_OUTSIDE;
            }
        break;

        case INPUT_RELEASED:
            event = SELECT_RELEASE;
        break;

        case INPUT_IRRELEVANT:
            event = SELECT_MOVE;
        break;

        default:
            assert(!"Unmapped mouse state");
        break;
    }

    enum select_state  state  = next_state[ select->state][event];
    enum select_action action = next_action[select->state][event];

    assert(event  != SELECT_INVALID_EVENT);
    assert(state  != SELECT_INVALID_STATE);
    assert(action != SELECT_INVALID_ACTION);

    select->state           = state;
    select->mouse_x_current = mouse_x;
    select->mouse_y_current = mouse_y;

    select->offset_x = select->mouse_x_current - select->mouse_x_pressed;
    select->offset_y = select->mouse_y_current - select->mouse_y_pressed;

    return action;
}
