/* ===========================================================================
 * uz80as, an assembler for the Zilog Z80 and several other microprocessors.
 *
 * Motorola S-Record file output support.
 * ===========================================================================
 */

#include "srec.h"
#include "err.h"
#include "options.h"

#ifndef STDIO_H
#include <stdio.h>
#endif

#ifndef STRING_H
#include <string.h>
#endif

 /* Program name for S0 header record */
static const char* PROGRAM_NAME = "UZ80AS";

/*
 * Calculate checksum for S-Record.
 * The checksum is the one's complement of the sum of all bytes
 * including the count and address bytes.
 */
static unsigned char calc_checksum(unsigned char count, unsigned long addr,
    const unsigned char* data, int addr_bytes)
{
    unsigned char sum = count;
    int i;

    /* Add address bytes to sum */
    for (i = addr_bytes - 1; i >= 0; i--) {
        sum += (addr >> (i * 8)) & 0xFF;
    }

    /* Add data bytes to sum */
    for (i = 0; i < count - addr_bytes - 1; i++) {
        sum += data[i];
    }

    /* Return one's complement */
    return ~sum;
}

/*
 * Write a single S-Record.
 * Format: Stype[count][address][data][checksum]
 * where:
 *   type     = Record type (0-9)
 *   count    = Character pairs following count byte
 *   address  = 2-4 byte address
 *   data     = Data bytes
 *   checksum = One's complement of sum of count, address and data bytes
 */
static void write_srec(FILE* f, int type, unsigned long addr,
    const unsigned char* data, size_t data_len)
{
    int addr_bytes, count;
    unsigned char checksum;
    int i;

    /* Determine address size based on record type */
    switch (type) {
    case SREC_S0:
    case SREC_S1:
    case SREC_S5:
    case SREC_S9:
        addr_bytes = 2;
        break;
    case SREC_S2:
    case SREC_S8:
        addr_bytes = 3;
        break;
    case SREC_S3:
    case SREC_S7:
        addr_bytes = 4;
        break;
    default:
        eprint(_("invalid S-Record type: %d\n"), type);
        return;
    }

    /* Count includes address bytes, data bytes, and checksum byte */
    count = addr_bytes + (int)data_len + 1;

    /* Calculate checksum */
    checksum = calc_checksum(count, addr, data, addr_bytes);

    /* Write record type */
    fprintf(f, "S%d", type);

    /* Write count */
    fprintf(f, "%02X", count);

    /* Write address */
    for (i = addr_bytes - 1; i >= 0; i--) {
        fprintf(f, "%02X", (unsigned char)(addr >> (i * 8)));
    }

    /* Write data if present */
    for (i = 0; i < data_len; i++) {
        fprintf(f, "%02X", data[i]);
    }

    /* Write checksum */
    fprintf(f, "%02X\n", checksum);
}

/*
 * Write the entire memory contents to an S-Record format file.
 * Outputs:
 * - S0 header record
 * - S1 or S2 or S3 data records depending on address range
 * - S5 record count
 * - S9/S8/S7 termination record
 *
 * Parameters:
 *   filename - Output filename
 *   mem      - Memory buffer containing code/data
 *   minpc    - Start address of memory range to output
 *   maxpc    - End address of memory range to output
 */
void output_srec(const char* filename, const unsigned char* mem,
    int minpc, int maxpc)
{
    FILE* fout;
    int addr;
    unsigned char data[SREC_MAX_DATA_LEN];
    int record_count = 0;
    int remaining, len, i;
    int srec_type;

    fout = efopen(filename, "w");
    if (!fout) {
        eprint(_("cannot open file %s for writing\n"), filename);
        return;
    }

    /* Determine record type based on address range */
    if (maxpc <= 0xFFFF) {
        srec_type = SREC_S1;
    }
    else if (maxpc <= 0xFFFFFF) {
        srec_type = SREC_S2;
    }
    else {
        srec_type = SREC_S3;
    }

    /* Write S0 header record with program name */
    write_srec(fout, SREC_S0, 0, (const unsigned char*)PROGRAM_NAME,
        strlen(PROGRAM_NAME));

    /* If no code was generated, just write termination records */
    if (minpc < 0 || maxpc <= minpc) {
        write_srec(fout, SREC_S5, record_count, NULL, 0);
        write_srec(fout, SREC_S9, 0, NULL, 0);
        fclose(fout);
        return;
    }

    /* Process memory in chunks of SREC_MAX_DATA_LEN bytes */
    for (addr = minpc; addr < maxpc; addr += SREC_MAX_DATA_LEN) {
        remaining = maxpc - addr;
        len = (remaining > SREC_MAX_DATA_LEN) ? SREC_MAX_DATA_LEN : remaining;

        /* Copy data bytes to buffer */
        for (i = 0; i < len; i++) {
            data[i] = mem[addr + i];
        }

        /* Write data record */
        write_srec(fout, srec_type, addr, data, len);
        record_count++;
    }

    /* Write record count */
    write_srec(fout, SREC_S5, record_count, NULL, 0);

    /* Write termination record with appropriate type */
    switch (srec_type) {
    case SREC_S1:
        write_srec(fout, SREC_S9, minpc, NULL, 0);
        break;
    case SREC_S2:
        write_srec(fout, SREC_S8, minpc, NULL, 0);
        break;
    case SREC_S3:
        write_srec(fout, SREC_S7, minpc, NULL, 0);
        break;
    }

    if (ferror(fout)) {
        eprint(_("error writing to file %s\n"), filename);
        clearerr(fout);
    }

    if (fclose(fout) == EOF) {
        eprint(_("cannot close file %s\n"), filename);
    }
}