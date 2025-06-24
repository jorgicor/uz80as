/* ===========================================================================
 * uz80as, an assembler for the Zilog Z80 and several other microprocessors.
 *
 * Motorola S-Record file output support.
 * ===========================================================================
 */

#ifndef SREC_H
#define SREC_H

#ifndef STDIO_H
#include <stdio.h>
#endif

 /* S-Record types */
#define SREC_S0   0    /* Header record */
#define SREC_S1   1    /* 16-bit address data record */
#define SREC_S2   2    /* 24-bit address data record */
#define SREC_S3   3    /* 32-bit address data record */
#define SREC_S5   5    /* 16-bit count of S1, S2, S3 records */
#define SREC_S7   7    /* 32-bit execution start address */
#define SREC_S8   8    /* 24-bit execution start address */
#define SREC_S9   9    /* 16-bit execution start address */

/* Maximum data bytes per record */
#define SREC_MAX_DATA_LEN   32

/* Write memory contents to S-Record format file */
void output_srec(const char* filename, const unsigned char* mem,
    int minpc, int maxpc);

#endif /* SREC_H */