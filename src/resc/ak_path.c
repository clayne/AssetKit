/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../ak_common.h"

#define CHR_SLASH       '/'
#define CHR_BACK_SLASH  '\\'
#define CHR_COLON       ':'
#define STR_SLASH       "/"
#define APPEND_SLASH                                                          \
  do {                                                                        \
    strncpy(buf++, STR_SLASH, 1);                                             \
    len++;                                                                    \
  } while (0)

#define AK_STRTRM_SET(X)                                                      \
  do {                                                                        \
    *it2 = *it1;                                                              \
    len++;                                                                    \
    it1++;                                                                    \
    it2++;                                                                    \
    skp = X;                                                                  \
  } while (0)

AK_EXPORT
size_t
ak_path_trim(char *path,
             char *trimmed) {
  char  *it1;
  char  *it2;
  size_t len;
  int    skp;

  len = skp = 0;
  it1 = path;
  it2 = trimmed;

  while (*it1 == ' ')
    it1++;

  while (*it1) {
    if (*it1 == '/' || *it1 == '\\') {
      if (skp != 0) {
        it1++;
        continue;
      }

      AK_STRTRM_SET(1);
    } else {
      AK_STRTRM_SET(0);
    }
  }

  while (*--it2 && (*it2 == ' ' || *it2 == '/' || *it2 == '\\'))
    len--;

  *(it2 += len > 0) = '\0';

  return len;
}

AK_EXPORT
int
ak_path_join(char   *fragments[],
             char   *buf,
             size_t *size) {
  const char *frag;
  const char *frag_end;
  size_t      len;
  size_t      frag_len;
  int         frag_idx;
  int         frag_idx_v;
  char        c;

  if (!fragments
      || !*fragments /* there are no fragments to join */
      || !buf
      || !size)
    return -EINVAL;

  frag_idx_v = frag_idx = len = 0;

  /* build path */
  while ((frag = fragments[frag_idx++])) {
    frag_len = strlen(frag);
    frag_end = frag + frag_len - 1;

    if (frag_len == 0)
      continue;

    /* starts with slash e.g. /usr */
    c = *frag;
    if (frag_idx_v == 0
        && (c == CHR_SLASH || c == CHR_BACK_SLASH)) {
      APPEND_SLASH;
      frag++;

      /* avoid extra slash after non-protocol frag 
         like /http:/ -> /http:// 
       */
      frag_idx_v++;
    }

    /* l-trim */
    while ((c = *frag) != '\0'
           && (c == CHR_SLASH || c == CHR_BACK_SLASH))
      frag++;

    /* r-trim */
    while ((c = *frag_end) != '\0'
           && (c == CHR_SLASH || c == CHR_BACK_SLASH))
      frag_end--;

    /* only slashes */
    if (!*frag) {
      if (len == 0)
        APPEND_SLASH;

      continue;
    }

    if (len > 1 && *(buf - 1) != CHR_SLASH)
      APPEND_SLASH;

    /* protocol e.g. http://, tcp:// */
    if (frag_idx_v == 1 && *(buf - 2) == CHR_COLON)
      APPEND_SLASH;

    frag_len = ++frag_end - frag;
    len += frag_len;

    if (*size < len)
      return -1;

    strncpy(buf, frag, frag_len);

    buf += frag_len;
    frag_idx_v++;
  };

  memset(buf + 1, '\0', 1);
  *size = len;

  return 0;
}