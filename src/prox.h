#pragma once

#include <stdbool.h>
#include <stdint.h>

struct prox;

struct prox* prox_init(int w, int h);
void         prox_free(struct prox* prox);

int          prox_width (const struct prox* prox);
int          prox_height(const struct prox* prox);
uint32_t     prox_max   (const struct prox* prox);
uint32_t     prox_get   (const struct prox* prox, int x, int y);
bool         prox_active(const struct prox* prox, int x, int y);

void         prox_resize(struct prox* prox, int w, int h);
void         prox_set   (struct prox* prox, int x, int y);
void         prox_unset (struct prox* prox, int x, int y);
