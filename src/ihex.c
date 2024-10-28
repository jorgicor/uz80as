/* ===========================================================================
 * uz80as, an assembler for the Zilog Z80 and several other microprocessors.
 *
 * Intel HEX file output support.
 * ===========================================================================
 */

#include "ihex.h"
#include "err.h"
#include "options.h"

#ifndef STDIO_H
#include <stdio.h>
#endif

#ifndef STRING_H
#include <string.h>
#endif

 /*
  * Write a single Intel HEX record to the output file.
  * Format: :LLAAAATTDD...CC
  * where:
  *   LL    = Length of data bytes
  *   AAAA  = 16-bit address
  *   TT    = Record type
  *   DD    = Data bytes
  *   CC    = Checksum (two's complement of sum of all bytes)
  */
static void write_hex_record(FILE* f, unsigned char type, unsigned short addr,
    const unsigned char* data, unsigned char len)
{
    unsigned char checksum;
    int i;

    /* Start record with ':' */
    fprintf(f, ":");

    /* Write record length */
    fprintf(f, "%02X", len);
    checksum = len;

    /* Write 16-bit address */
    fprintf(f, "%04X", addr);
    checksum += (addr >> 8) & 0xFF;
    checksum += addr & 0xFF;

    /* Write record type */
    fprintf(f, "%02X", type);
    checksum += type;

    /* Write data bytes if present */
    if (data != NULL) {
        for (i = 0; i < len; i++) {
            fprintf(f, "%02X", data[i]);
            checksum += data[i];
        }
    }

    /* Write checksum (two's complement of sum) */
    fprintf(f, "%02X\n", (unsigned char)(-(int)checksum));
}

/*
 * Write the entire memory contents to an Intel HEX format file.
 * Handles:
 * - Data records for memory contents
 * - Extended Linear Address records for addresses above 64K
 * - End of File record
 *
 * Parameters:
 *   filename - Output filename
 *   mem      - Memory buffer containing code/data
 *   minpc    - Start address of memory range to output
 *   maxpc    - End address of memory range to output
 */
void output_hex(const char* filename, const unsigned char* mem,
    unsigned long minpc, unsigned long maxpc)
{
    FILE* fout;
    unsigned long addr;
    unsigned short segment = 0;
    unsigned char data[IHEX_MAX_DATA_LEN];
    int remaining, len, i;

    fout = efopen(filename, "w");
    if (!fout) {
        eprint(_("cannot open file %s for writing\n"), filename);
        return;
    }

    /* If no code was generated, output empty hex file with just EOF record */
    if (minpc < 0 || maxpc <= minpc) {
        write_hex_record(fout, IHEX_TYPE_EOF, 0, NULL, 0);
        fclose(fout);
        return;
    }

    /* Process memory in chunks of IHEX_MAX_DATA_LEN bytes */
    for (addr = minpc; addr < maxpc; addr += IHEX_MAX_DATA_LEN) {
        remaining = maxpc - addr;
        len = (remaining > IHEX_MAX_DATA_LEN) ? IHEX_MAX_DATA_LEN : remaining;

        /* Check if we need an extended linear address record */
        if ((addr >> 16) != segment) {
            segment = addr >> 16;
            data[0] = segment >> 8;
            data[1] = segment & 0xFF;
            write_hex_record(fout, IHEX_TYPE_EXT_LINEAR_ADDR, 0, data, 2);
        }

        /* Copy data bytes to buffer */
        for (i = 0; i < len; i++) {
            data[i] = mem[addr + i];
        }

        /* Write data record */
        write_hex_record(fout, IHEX_TYPE_DATA, addr & 0xFFFF, data, len);
    }

    /* Write End of File record */
    write_hex_record(fout, IHEX_TYPE_EOF, 0, NULL, 0);

    if (ferror(fout)) {
        eprint(_("error writing to file %s\n"), filename);
        clearerr(fout);
    }

    if (fclose(fout) == EOF) {
        eprint(_("cannot close file %s\n"), filename);
    }
}