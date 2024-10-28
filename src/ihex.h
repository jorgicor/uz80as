/* ===========================================================================
 * uz80as, an assembler for the Zilog Z80 and several other microprocessors.
 *
 * Intel HEX file output support.
 * ===========================================================================
 */

#ifndef IHEX_H
#define IHEX_H

#ifndef STDIO_H
#include <stdio.h>
#endif

 /* Intel HEX record types */
#define IHEX_TYPE_DATA              0x00    /* Data record */
#define IHEX_TYPE_EOF              0x01    /* End of File record */
#define IHEX_TYPE_EXT_SEG_ADDR     0x02    /* Extended Segment Address record */
#define IHEX_TYPE_START_SEG_ADDR   0x03    /* Start Segment Address record */
#define IHEX_TYPE_EXT_LINEAR_ADDR  0x04    /* Extended Linear Address record */
#define IHEX_TYPE_START_LINEAR_ADDR 0x05   /* Start Linear Address record */

/* Maximum data bytes per record */
#define IHEX_MAX_DATA_LEN   32

/* Write memory contents to Intel HEX format file */
void output_hex(const char* filename, const unsigned char* mem,
    unsigned long minpc, unsigned long maxpc);

#endif /* IHEX_H */