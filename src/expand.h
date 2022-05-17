#ifndef __T1__EXPAND__H__
#define __T1__EXPAND__H__

// X Macros bullshit.

#define COMMA     ,
#define SEMICOLON ;
#define NOTHING
#define SUM       +

#define AS_NEXT_STATE( MATRIX, STATE, EVENT, NEXT_STATE, ACTION) MATRIX[STATE][EVENT] = NEXT_STATE;
#define AS_NEXT_ACTION(MATRIX, STATE, EVENT, NEXT_STATE, ACTION) MATRIX[STATE][EVENT] = ACTION;
// State machine stuff.

#define AS_ONE(...) 1

#endif // __T1__EXPAND__H__
