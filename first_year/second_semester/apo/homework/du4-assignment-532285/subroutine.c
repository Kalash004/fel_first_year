/* Implementation of the subroutine subroutine_fnc */

#include "subroutine.h"

// int subroutine_fnc(int* a0, int a1)
// {
//   int *a5 = a0;
//   unsigned char *base_addr = (int *)0x00011000;
//   unsigned char a4 = base_addr[676]; // char to skip

//   if (a4 != (unsigned char)a1) {
//     goto skip1;
//   }
 
//   int a3 = *a5;
//   a4 = (unsigned char)(a3 + 1);
//   if (a3 == 0) goto skip2;
//   a0 = 0;
//   goto skip2;

//   // done
//   skip1:
//   a4 = 0;
 
//   skip2:
//   *a5 = a4;
//   return a0;
// }

// int subroutine_fnc(int *a0, int a1) {
//   int *a5 = a0;
//   volatile unsigned char *base_addr = (unsigned char *)0x00011000;
//   volatile unsigned char a4 = base_addr[676]; // char to skip
//   if (a4 != a1) {
//     int a3 = *a5;
//     a4 = a3 + 1;
//     if (a3 != 0) {
//       return 0;
//     }
//     } else {
//       a4 = 0;
//     }
//     *a5 = a4;
//     return a0;
// }

// int subroutine_fnc(int *a0, int a1) {
//   int a5 = (int)a0;
//   a0 = a1;
//   int a4 = *(int *)((char *)0x00011000 + 676);
//   if (a4 != a1) goto skip1;
//   int a3 = *((int *)a5);
//   a4 = a3 + 1;
//   if (a3 == 0) goto skip2;
//   a0 = 0;
//   skip1:
//   a4 = 0;
//   skip2:
//   *((int *)a5) = a4;
//   return a0;
// }

int subroutine_fnc(int *ptr, int val) {
    int ref_val = 0x0A;

    if (ref_val != val) {
        *ptr = 0;
        return val;
    }

    int current = *ptr;
    int new_val = current + 1;

    if (current == 0) {
        *ptr = new_val;
        return val;
    }

    *ptr = new_val;
    return 0;
}