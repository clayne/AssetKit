/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "array.h"
#include <string.h>

AkResult _assetkit_hide
ak_strtod_array(AkHeap         * __restrict heap,
                void           * __restrict memParent,
                char                       *content,
                AkDoubleArray ** __restrict array) {
  AkDoubleArray *arr;
  size_t         count;

  count = ak_strtok_count_fast(content, NULL);
  if (count == 0)
    return AK_ERR;

  arr = ak_heap_alloc(heap,
                      memParent,
                      sizeof(*arr)
                      + sizeof(AkDouble) * count);
  ak_strtod_fast(content, arr->items, count);
  arr->count = count;

  *array = arr;

  return AK_OK;
}

AkResult _assetkit_hide
ak_strtoui_array(AkHeap       * __restrict heap,
                 void         * __restrict memParent,
                 char                     *content,
                 AkUIntArray ** __restrict array) {
  AkUIntArray *arr;
  size_t       count;

  count = ak_strtok_count_fast(content, NULL);
  if (count == 0)
    return AK_ERR;

  arr = ak_heap_alloc(heap,
                      memParent,
                      sizeof(*arr)
                      + sizeof(AkUInt) * count);
  ak_strtoui_fast(content, arr->items, count);
  arr->count = count;

  *array = arr;

  return AK_OK;
}

AkResult _assetkit_hide
ak_strtostr_array(AkHeap         * __restrict heap,
                  void           * __restrict memParent,
                  char                       *content,
                  char                        separator,
                  AkStringArray ** __restrict array) {
  AkStringArray  *stringArray;
  char           *pData;
  char           *tok;
  char            separatorStr[4];
  size_t          arrayIndex;
  size_t          itemCount;
  size_t          arraySize;
  size_t          arrayDataSize;

  arrayIndex = 0;
  itemCount  = 0;

  separatorStr[0] = separator;
  separatorStr[1] = '\n';
  separatorStr[2] = '\r';
  separatorStr[3] = '\t';

  pData = content;
  while (*pData != '\0'
         && *++pData == separator)
    itemCount++;

  /*
   |pSTR1|pSTR2|pSTR3|STR1\0STR2\0STR3|

   the last one is pointer to all data
   */
  arraySize = sizeof(char *) * (itemCount + 1);
  arrayDataSize = strlen(content) + itemCount /* NULL */;

  stringArray = ak_heap_alloc(heap,
                              memParent,
                              sizeof(*stringArray) + arraySize);

  pData = ak_heap_alloc(heap,
                        stringArray,
                        arrayDataSize);

  stringArray->count = itemCount;
  stringArray->items[itemCount] = pData;

  tok = strtok(content, separatorStr);
  while (tok) {
    strcpy(pData, tok);
    stringArray->items[arrayIndex++] = pData;

    pData += strlen(tok);
    *pData++ = '\0';

    tok = strtok(NULL, separatorStr);
  }

  *array = stringArray;

  return AK_OK;
}

AkResult _assetkit_hide
ak_strtostr_arrayL(AkHeap * __restrict heap,
                   void * __restrict memParent,
                   char * stringRep,
                   char separator,
                   AkStringArrayL ** __restrict array) {
  AkStringArrayL *stringArray;
  char           *pData;
  char           *tok;
  char            separatorStr[4];
  size_t          arrayIndex;
  size_t          itemCount;
  size_t          arraySize;
  size_t          arrayDataSize;

  arrayIndex = 0;
  itemCount  = 0;

  separatorStr[0] = separator;
  separatorStr[1] = '\n';
  separatorStr[2] = '\r';
  separatorStr[3] = '\t';

  pData = stringRep;
  while (*pData != '\0'
         && *++pData == separator)
    itemCount++;

  /*
   |pSTR1|pSTR2|pSTR3|STR1\0STR2\0STR3|

   the last one is pointer to all data
   */
  arraySize = sizeof(char *) * (itemCount + 1);
  arrayDataSize = strlen(stringRep) + itemCount /* NULL */;

  stringArray = ak_heap_alloc(heap,
                              memParent,
                              sizeof(*stringArray) + arraySize);

  pData = ak_heap_alloc(heap,
                        stringArray,
                        arrayDataSize);

  stringArray->count = itemCount;
  stringArray->items[itemCount] = pData;

  tok = strtok(stringRep, separatorStr);
  while (tok) {
    strcpy(pData, tok);
    stringArray->items[arrayIndex++] = pData;

    pData += strlen(tok);
    *pData++ = '\0';

    tok = strtok(NULL, separatorStr);
  }

  *array = stringArray;

  return AK_OK;
}
