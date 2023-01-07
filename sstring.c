#include "include/sstring.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  uint32_t capacity : 32;
  uint32_t length : 32;
} sstr;

uint32_t next_pow2(uint32_t num);
int8_t sstr_realloc(sstr_t *s, uint32_t len);

void lshift(char *ptr, uint32_t len, uint32_t start);
void rshift(char *ptr, uint32_t len, uint32_t start, uint32_t count);

sstr_t get_data(char *ptr) { return ptr + sizeof(sstr); }
sstr *get_header(char *ptr) { return (sstr *)(ptr - sizeof(sstr)); }

sstr_t sstr_new(uint32_t capacity) {
  capacity = next_pow2(capacity);

  sstr *hdr = calloc(sizeof(sstr) + (sizeof(char) * capacity), 1);

  if (hdr == NULL) {
    return NULL;
  }

  sstr_t data = get_data((char *)hdr);

  hdr->capacity = capacity;
  hdr->length = 0;

  return data;
}

sstr_t sstr_from(const char *s, uint32_t len) {
  uint32_t capacity = next_pow2(len + 1);

  sstr_t data = sstr_new(capacity);
  sstr *hdr = get_header(data);

  for (uint32_t i = 0; i < len; i++) {
    data[i] = s[i];
  }

  hdr->length = len;

  return data;
}

void sstr_free(sstr_t *s) {
  free(get_header(*s));
  *s = NULL;
}

uint32_t sstr_capacity(sstr_t s) {
  sstr *sp = get_header(s);
  return sp->capacity;
}

uint32_t sstr_len(sstr_t s) {
  sstr *sp = get_header(s);
  return sp->length;
}

int8_t sstr_reserve(sstr_t *s, uint32_t additional) {
  if (s == NULL) {
    return -1;
  }

  uint32_t new_capacity = next_pow2(sstr_capacity(*s) + additional);
  sstr_realloc(s, new_capacity);

  return 0;
}

int8_t sstr_realloc(sstr_t *s, uint32_t capacity) {
  if (*s == NULL) {
    return -1;
  }

  sstr *hdr = get_header(*s);

  char *ptr = realloc(hdr, sizeof(sstr) + (sizeof(char) * capacity));

  hdr = (sstr *)ptr;
  hdr->capacity = capacity;
  *s = get_data((char *)hdr);

  return 0;
}

int8_t sstr_shrink_to_fit(sstr_t *s) {
  uint32_t capacity = sstr_len(*s);

  sstr *hdr = calloc(sizeof(sstr) + (sizeof(char) * capacity + 1), 1);
  hdr->capacity = capacity;

  sstr_t prev = *s;

  sstr_t s1 = get_data((char *)hdr);

  for (uint32_t i = 0; i < capacity; i++) {
    s1[i] = (*s)[i];
  }

  s1[capacity] = '\0';

  *s = s1;

  free(get_header(prev));

  return 0;
}

void sstr_clear(sstr_t s) {
  if (s == NULL) {
    return;
  }

  for (uint32_t i = 0; i < sstr_len(s); i++) {
    s[i] = '\0';
  }

  sstr *hdr = get_header(s);
  hdr->length = 0;
}

void sstr_debug(sstr_t s) {
  if (s == NULL) {
    return;
  }

  printf("{ cap: %d, len: %d, data: \"%s\" }\n", sstr_capacity(s), sstr_len(s),
         s);
}

void sstr_format(sstr_t *s, const char *fmt, ...) {}

int8_t sstr_insert(sstr_t *s, uint32_t index, const char *str, uint32_t len) {
  uint32_t len_s = sstr_len(*s);

  if (index > len_s) {
    return -1;
  }

  uint32_t final = len + len_s;
  uint32_t cap = sstr_capacity(*s);

  if (final >= cap) {
    sstr_realloc(s, next_pow2(final));
  }

  if (index < len_s) {
    rshift(*s, len_s, index, len);
  }

  (*s)[len_s + len] = '\0';

  for (uint32_t i = 0; i < len; i++) {
    (*s)[index + i] = str[i];
  }

  sstr *hdr = get_header(*s);
  hdr->length = final;

  return 0;
}

int8_t sstr_remove(sstr_t s, uint32_t offset, uint32_t count) {
  uint32_t len = sstr_len(s);

  if (offset > len || offset + count > len) {
    return -1;
  }

  for (uint32_t i = offset; i < len - count; i++) {
    s[i] = s[i + count];
  }

  s[len - count] = '\0';

  sstr *hdr = get_header(s);
  hdr->length = len - count;
  return 0;
}

int8_t sstr_compare(sstr_t a, const char *b, uint32_t len_b) {
  uint32_t len_a = sstr_len(a);
  uint32_t min = (len_a > len_b) ? len_b : len_a;

  for (uint32_t i = 0; i < min; i++) {
    if (a[i] == b[i]) {
      continue;
    }

    if (a[i] > b[i]) {
      return 1;
    }

    if (a[i] < b[i]) {
      return -1;
    }
  }

  if (len_a > len_b) {
    return 1;
  }

  if (len_b > len_a) {
    return -1;
  }

  return 0;
}

int8_t sstr_find(sstr_t s, uint32_t *index, uint32_t start, const char *a,
                 uint32_t len_a) {
  uint32_t len_s = sstr_len(s);

  if (len_a == 0 || len_s < len_a) {
    return 0;
  }

  for (uint32_t i = start; i < len_s - len_a + 1; i++) {
    if (s[i] == a[0]) {
      int8_t res = 0;

      for (uint32_t j = 1; j < len_a; j++) {
        if (s[i + j] != a[j]) {
          res = 1;
          break;
        }
      }

      if (res == 0) {
        *index = i;
        return 1;
      }
    }
  }

  return 0;
}

int8_t sstr_replace(sstr_t *s, uint32_t index, const char *a, uint32_t len_a,
                    const char *b, uint32_t len_b) {
  return 0;
}

void sstr_trim_left(sstr_t s) {
  if (sstr_len(s) == 0) {
    return;
  }

  uint32_t end = 0;

  while (isspace(s[end++])) {
  }

  uint32_t len = sstr_len(s);
  lshift(s, len, end - 1);

  for (uint32_t i = end; i < len; i++) {
    s[i - end] = s[i];
  }

  s[len - end + 1] = '\0';

  sstr *hdr = get_header(s);
  hdr->length = len - end;
}

void sstr_trim_right(sstr_t s) {
  if (sstr_len(s) == 0) {
    return;
  }

  uint32_t end = sstr_len(s) - 1;
  while (isspace(s[end--])) {
  }
  end++;

  s[end + 1] = '\0';

  sstr *hdr = get_header(s);
  hdr->length -= end - 1;
}

void sstr_to_lower(sstr_t s) {
  if (sstr_len(s) == 0) {
    return;
  }

  for (uint32_t i = 0; i < sstr_len(s); i++) {
    s[i] = tolower(s[i]);
  }
}

void sstr_to_upper(sstr_t s) {
  if (sstr_len(s) == 0) {
    return;
  }

  for (uint32_t i = 0; i < sstr_len(s); i++) {
    s[i] = toupper(s[i]);
  }
}

int8_t sstr_push(sstr_t *s, char c) {
  uint32_t len = sstr_len(*s);
  uint32_t cap = sstr_capacity(*s);

  if (len + 1 >= cap) {
    sstr_realloc(s, next_pow2(cap + 1));
  }

  (*s)[len] = c;
  (*s)[len + 1] = '\0';

  sstr *hdr = get_header(*s);
  hdr->length += 1;

  return 0;
}

int8_t sstr_pop(sstr_t s, char *dst) {
  uint32_t len = sstr_len(s);

  if (len == 0) {
    dst = NULL;
    return 1;
  }

  *dst = s[len - 1];
  s[len - 1] = '\0';

  sstr *hdr = get_header(s);
  hdr->length = len - 1;

  return 0;
}

uint32_t next_pow2(uint32_t num) {
  num--;
  num |= num >> 1;
  num |= num >> 2;
  num |= num >> 4;
  num |= num >> 8;
  num |= num >> 16;
  num++;
  return num;
}

void lshift(char *ptr, uint32_t len, uint32_t start) {
  for (uint32_t i = start; i < len; i++) {
    ptr[i - start] = ptr[i];
  }
}

void rshift(char *ptr, uint32_t len, uint32_t start, uint32_t count) {
  for (uint32_t i = start; i < len; i++) {
    ptr[i + count] = ptr[i];
  }
}
