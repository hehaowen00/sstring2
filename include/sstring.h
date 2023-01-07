#ifndef __SSTRING_H__
#define __SSTRING_H__

#include <stdint.h>

typedef char *sstr_t;

sstr_t sstr_new(uint32_t capacity);
sstr_t sstr_from(const char *s, uint32_t s_len);
sstr_t sstr_copy(sstr_t *s);

void sstr_free(sstr_t *s);

uint32_t sstr_capacity(sstr_t s);
uint32_t sstr_len(sstr_t s);

int8_t sstr_reserve(sstr_t *s, uint32_t additional);
int8_t sstr_shrink_to_fit(sstr_t *s);

void sstr_clear(sstr_t s);
void sstr_debug(sstr_t s);

void sstr_format(sstr_t *s, const char *fmt, ...);

int8_t sstr_insert(sstr_t *s, uint32_t index, const char *str, uint32_t len);
int8_t sstr_remove(sstr_t s, uint32_t offset, uint32_t count);

int8_t sstr_compare(sstr_t a, const char *b, uint32_t len_b);
int8_t sstr_find(sstr_t s, uint32_t *index, uint32_t start, const char *a,
                 uint32_t len_a);
int8_t sstr_replace(sstr_t *s, uint32_t index, const char *a, uint32_t len_a,
                    const char *b, uint32_t len_b);

void sstr_trim_left(sstr_t s);
void sstr_trim_right(sstr_t s);

void sstr_to_lower(sstr_t s);
void sstr_to_upper(sstr_t s);

int8_t sstr_append(sstr_t *s, const char *a, uint32_t len);
int8_t sstr_prepend(sstr_t *s, const char *a, uint32_t len);

int8_t sstr_push(sstr_t *s, char c);
int8_t sstr_pop(sstr_t s, char *dst);

#endif
