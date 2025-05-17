/* Implementation of the subroutine toplevel_fnc */

#include "subroutine.h"
#include <unistd.h>

// int toplevel_fnc(void)
// {
//   unsigned char buffer; // sp11
//   int buffer2 = 0; // sp12
//   int res = read(0, &buffer, 1);
//   int s0 = 0;
  
//   if (res != 1) {
//     goto skip1; // todo
//   }
//   goto skip2; // todo

//   skip4:
//   skip5:
//   res = read(0, &buffer, 1);
//   if (res != 1) {
//     goto skip3;
//   }
  
//   skip2:
//   unsigned char a1 = buffer;
//   res = subroutine_fnc(&buffer2, a1); //    10264:	f71ff0ef          	jal	ra,101d4 <subroutine_fnc>
//   if (buffer == 0) {
//     goto skip4;
//   }
//   res = write(1, &buffer, 1);
//   goto skip5;
//   // end
//   skip1:
//   skip3:
//   return a0;
// }
// int toplevel_fnc(void) {
//     char buffer;      // at sp+11
//     char buffer2 = 0; // at sp+12
//     int s0 = 0;

//     // Initial read 1 byte from stdin into buffer
//     if (read(0, &buffer, 1) != 1) {
//         s0 = 0;
//         goto exit;
//     }

// loop_start:
//     s0 = 0; // reset s0 counter
//     while (1) {
//         buffer2 = 0;

//         if (read(0, &buffer, 1) != 1) {
//             goto exit;
//         }

//         if (buffer != 1) {
//             s0 = 0;
//             goto exit;
//         }

//         int ret = subroutine_fnc(&buffer2, buffer);
//         buffer = (char)(ret & 0xFF); // zero-extend byte

//         if (buffer == 0) {
//             s0++;       // increment counter
//             continue;   // repeat read in loop
//         }

//         write(1, &buffer, 1);
//     }

// exit:
//     return s0;
// }

// void toplevel_fnc() {
//     unsigned char *sp = (unsigned char *)malloc(32);
//     unsigned char ra = 0; // Placeholder for return address
//     unsigned char s0 = 0;
//     unsigned char a0 = 0;
//     unsigned char a5 = 1;


//     a0 = read(0, sp+11, 1); 

//     if (a0 != a5) {
//         s0 = 0;
//     } else {
//         s0 = 0;
//         a0 = read(0, sp+11, 1); 
//     }

//     unsigned char a1 = *(sp + 11);
//     subroutine_fnc(sp + 12, a1);
//     a0 = a0; // Placeholder for zero-extension

//     *(sp + 11) = a0;

//     if (a0 == 0) {
//         // Skip to increment s0
//     }

//     a0 = write(1, sp + 11, 1);
//     // Free allocated memory
//     free(sp);
// }


// int toplevel_fnc(void)
// {
//   // bs 
//   int buffer;
//   int buffer2;
//   int s0 = 0;
//   int res = read(0, &buffer, 1);
//   if (1 != res) goto skip1;
//   s0 = 0;
//   goto skip2;
//   skip4:
//   s0 += 1;
//   skip5:
//   res = read(0, &buffer, 1);
//   if (1 != res) {
//     goto skip3;
//   }
//   skip2:
//   unsigned char a1 = (unsigned char)buffer; // !!!
//   int res2 = subroutine_fnc(&buffer2, a1); // !!!
//   int res2 = res2 & 0xFF;
//   buffer = res2;
//   if (res2 == 0) goto skip4;
//   res = write(1, &buffer, 1);
//   goto skip5;
//   skip1:
//   s0 = 0;
//   skip3:
//   return s0;

// }

int toplevel_fnc(void)
{
    int buffer;
    int buffer2;
    int s0 = 0;

    if (read(0, &buffer, 1) != 1) {
        return 0;
    }

    while (1) {
        unsigned char a1 = (unsigned char)buffer;
        int res = subroutine_fnc(&buffer2, a1) & 0xFF;
        buffer = res;

        if (res == 0) {
            s0 += 1;
        } else {
            write(1, &buffer, 1);
        }

        if (read(0, &buffer, 1) != 1) {
            break;
        }
    }

    return s0;
}