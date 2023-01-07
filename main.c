#include "include/sstring.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("s1\n");
  sstr_t s1 = sstr_new(3);
  sstr_debug(s1);
  sstr_free(&s1);

  printf("s2\n");
  sstr_t s2 = sstr_new(8);
  sstr_debug(s2);

  sstr_insert(&s2, 0, "Hello,", 6);
  sstr_debug(s2);

  sstr_insert(&s2, 2, "(World!)", 8);
  sstr_debug(s2);

  sstr_clear(s2);

  sstr_insert(&s2, 0, "Start", 5);
  sstr_insert(&s2, 5, "End", 3);
  sstr_debug(s2);

  sstr_free(&s2);

  printf("s3\n");
  sstr_t s3 = sstr_from("Hello, World!", 13);
  sstr_debug(s3);

  sstr_to_lower(s3);
  sstr_debug(s3);

  sstr_to_upper(s3);
  sstr_debug(s3);

  sstr_insert(&s3, 6, "Goodbye, World! Goodbye, World!", 31);
  sstr_debug(s3);

  sstr_shrink_to_fit(&s3);
  sstr_debug(s3);

  sstr_clear(s3);
  sstr_debug(s3);

  sstr_free(&s3);

  printf("trim left\n");
  sstr_t trim_left = sstr_from("\t start", 7);
  sstr_debug(trim_left);
  sstr_trim_left(trim_left);
  sstr_debug(trim_left);
  sstr_free(&trim_left);

  printf("trim right\n");
  sstr_t trim_right = sstr_from("start\t  ", 8);
  sstr_debug(trim_right);
  sstr_trim_right(trim_right);

  sstr_debug(trim_right);
  sstr_free(&trim_right);

  printf("stack\n");
  sstr_t stack = sstr_new(4);
  sstr_debug(stack);

  sstr_push(&stack, 'a');
  sstr_push(&stack, 'b');
  sstr_push(&stack, 'c');
  sstr_push(&stack, 'd');

  sstr_debug(stack);

  int8_t res;
  char c;

  while (!(res = sstr_pop(stack, &c))) {
    printf("pop %d %c\n", res, c);
  }

  sstr_debug(stack);
  sstr_free(&stack);

  printf("compare\n");
  sstr_t c1 = sstr_from("abc", 3);
  sstr_t c2 = sstr_from("abcd", 4);
  sstr_t c3 = sstr_from("abd", 3);

  res = sstr_compare(c1, c1, sstr_len(c1));
  printf("cmp(c1, c1) %d\n", res);

  res = sstr_compare(c1, c2, sstr_len(c2));
  printf("cmp(c1, c2) %d\n", res);

  res = sstr_compare(c1, c3, sstr_len(c3));
  printf("cmp(c1, c3) %d\n", res);

  res = sstr_compare(c2, c1, sstr_len(c1));
  printf("cmp(c2, c1) %d\n", res);

  sstr_free(&c1);
  sstr_free(&c2);
  sstr_free(&c3);

  printf("find\n");
  sstr_t haystack = sstr_from("abcdefghijklmnopqrstuvwxyz", 26);

  uint32_t index = 0;
  res = sstr_find(haystack, &index, 0, "z", 1);
  printf("search %d %d\n", res, index);

  sstr_free(&haystack);

  printf("remove\n");
  sstr_t remove = sstr_from("Hello, World!", 13);
  sstr_debug(remove);

  sstr_remove(remove, 5, 7);
  sstr_debug(remove);

  sstr_free(&remove);

  return 0;
}
