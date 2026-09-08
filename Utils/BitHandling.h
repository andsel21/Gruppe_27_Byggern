
/*
 * BitHandling.S
 *
 * Created: 08.09.2026 10:20:15
 *  Author: andrksel
 */ 

 #pragma once

 #define  SET_BIT(x,n)  ((x) |= (1u<<(n)))
 #define  CLEAR_BIT(x,n)  ((x) &= ~(1u<<(n)))
 #define  TOGGLE_BIT(x,n)  ((x) ^= (1u<<(n)))
 #define  CHECK_BIT(x,n)  ((x) >> (n)) & 1u)
