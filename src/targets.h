#ifndef TARGETS_H
#define TARGETS_H

struct target;

const struct target *find_target(const char *id);

const struct target *first_target(void);
const struct target *next_target(void);

#endif
