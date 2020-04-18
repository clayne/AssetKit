/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef __libassetkit__dae_common__h_
#define __libassetkit__dae_common__h_

#include "../../include/ak/assetkit.h"
#include "../../include/ak/url.h"
#include "../common.h"
#include "../utils.h"
#include "../tree.h"
#include "../xml.h"
#include "strpool.h"

#include <ds/forward-list-sep.h>
#include <string.h>
#include <xml/xml.h>
#include <xml/attrib.h>

#ifndef AK_INPUT_SEMANTIC_VERTEX 
#  define AK_INPUT_SEMANTIC_VERTEX 100001
#endif

typedef enum AkCOLLADAVersion {
  AK_COLLADA_VERSION_150 = 150,
  AK_COLLADA_VERSION_141 = 141,
  AK_COLLADA_VERSION_140 = 140
} AkCOLLADAVersion;

typedef struct AkURLQueue {
  AkURL *url;
  struct AkURLQueue *next;
} AkURLQueue;

typedef AK_ALIGN(16) struct DAEState {
  AkHeap          *heap;
  void            *tempmem;
  void            *jobs14;
  AkDoc           *doc;
  AkURLQueue      *urlQueue;
  FListItem       *accessors;
  FListItem       *instCtlrs;
  FListItem       *inputs;
  FListItem       *toRadiansSampelers;
  RBTree          *meshInfo;
  RBTree          *inputmap;
  RBTree          *texmap;
  RBTree          *instanceMap;
  AkSource        *sources;
  AkCOLLADAVersion version;
  bool             stop;
} DAEState;

typedef struct AkDaeMeshInfo {
  AkInput *pos;
  size_t   nVertex;
} AkDaeMeshInfo;

typedef struct AkDAETextureRef {
  const char *texture;
  const char *texcoord;
} AkDAETextureRef;

typedef struct AkNewParam {
  /* const char * sid; */
  struct AkNewParam *prev;
  struct AkNewParam *next;
  const char        *semantic;
  AkValue           *val;
} AkNewParam;

typedef struct AkAccessorDAE {
  AkTypeDesc          *type;
  struct AkDataParam  *param;
  AkURL                source;
  size_t               offset;
  uint32_t             stride;
  uint32_t             bound;
} AkAccessorDAE;

AK_INLINE
void
sid_seta(xml_t  * __restrict xml,
         AkHeap * __restrict heap,
         void   * __restrict memnode,
         void   * __restrict memptr) {
  const char *sid;

  if (!(sid = xmla_strdup_by(xml, heap, _s_dae_sid, memnode)))
    return;

  ak_sid_seta(memnode, memptr, sid);
}

AK_INLINE
void
sid_set(xml_t  * __restrict xml,
        AkHeap * __restrict heap,
        void   * __restrict memnode) {
  const char *sid;
  
  if (!(sid = xmla_strdup_by(xml, heap, _s_dae_sid, memnode)))
    return;
  
  ak_sid_set(memnode, sid);
}

AK_INLINE
void
url_set(DAEState   * __restrict dst,
        xml_t      * __restrict xml,
        const char * __restrict name,
        void       * __restrict memp,
        AkURL      * __restrict url) {
  AkURLQueue *urlQueue;
  xml_attr_t *att;

  if (!(att = xmla(xml, name)) || !att->val) {
    url->reserved = NULL;
    url->url      = NULL;
    return;
  }
  
  ak_url_init(memp, xmla_strdup(att, dst->heap, memp), url);
  
  urlQueue       = dst->heap->allocator->malloc(sizeof(*urlQueue));
  urlQueue->next = dst->urlQueue;
  urlQueue->url  = url;
  dst->urlQueue  = urlQueue;
}

AK_INLINE
AkURL*
url_from(xml_t      * __restrict xml,
         const char * __restrict name,
         void       * __restrict memp) {
  AkHeap     *heap;
  AkURL      *url;
  xml_attr_t *att;

  if (!(att = xmla(xml, name)) || ! att->val)
    return NULL;
  
  heap = ak_heap_getheap(memp);
  url  = ak_heap_calloc(heap, memp, sizeof(*url));
  ak_url_init(memp, xmla_strdup(att, heap, memp), url);
  
  return url;
}

#endif /* __libassetkit__dae_common__h_ */
