#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */
	char *expr;
	uint32_t value;

} WP;
WP *get_head();
WP *new_wp();
void free_wp(int);
void add_to_head(WP *);
void renew(WP *);
void removeFromHead(int n);
void get_free_();
#endif
