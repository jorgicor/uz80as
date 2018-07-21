/* ===========================================================================
 * uz80as, an assembler for the Zilog Z80 and several other microprocessors.
 *
 * Target list.
 * ===========================================================================
 */

#include "targets.h"
#include "uz80as.h"

#ifndef STRING_H
#include <string.h>
#endif

extern const struct target s_target_z80;
extern const struct target s_target_hd64180;
extern const struct target s_target_gbcpu;
extern const struct target s_target_dp2200;
extern const struct target s_target_dp2200ii;
extern const struct target s_target_i8008;
extern const struct target s_target_i8080;
extern const struct target s_target_i8085;

static const struct target *s_targets[] = {
	&s_target_z80,
	&s_target_hd64180,
	&s_target_gbcpu,
	&s_target_dp2200,
	&s_target_dp2200ii,
	&s_target_i8008,
	&s_target_i8080,
	&s_target_i8085,
	NULL,
};

static int s_index;

const struct target *find_target(const char *id)
{
	const struct target **p;

	for (p = s_targets; *p != NULL; p++) {
		if (strcmp(id, (*p)->id) == 0) {
			return *p;
		}
	}

	return NULL;
}

const struct target *first_target(void)
{
	s_index = 0;
	return next_target();
}

const struct target *next_target(void)
{
	if (s_targets[s_index] != NULL) {
		return s_targets[s_index++];
	} else {
		return NULL;
	}
}
