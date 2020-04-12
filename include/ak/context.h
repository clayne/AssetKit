/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef ak_context_h
#define ak_context_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "map.h"
#include "util.h"

typedef struct AkContext {
  AkDoc              *doc;
  AkTechniqueHint    *techniqueHint;
  AkInstanceMaterial *instanceMaterial;
  //  AkMap              *bindVertexInput;
} AkContext;

#ifdef __cplusplus
}
#endif
#endif /* ak_context_h */
