#ifndef __T2__UTIL__H__
#define __T2__UTIL__H__

// Beware multiple evaluation! Don't pass as parameters:
// - Non-idempotent and non-pure function calls;
// - Increment ++ or decrement -- operators.

#define MAX_2(A, B) ((A) > (B) ? (A) : (B))
#define MIN_2(A, B) ((A) < (B) ? (A) : (B))

#endif // __T2__UTIL__H__
