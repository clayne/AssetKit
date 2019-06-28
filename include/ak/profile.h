/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef ak_profile_h
#define ak_profile_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "material.h"

struct AkEffect;

typedef enum AkProfileType {
  AK_PROFILE_TYPE_UNKOWN =-1,
  AK_PROFILE_TYPE_COMMON = 0,
  AK_PROFILE_TYPE_CG     = 1,
  AK_PROFILE_TYPE_GLES   = 2,
  AK_PROFILE_TYPE_GLES2  = 3,
  AK_PROFILE_TYPE_GLSL   = 4,
  AK_PROFILE_TYPE_BRIDGE = 5,

  AK_PROFILE_TYPE_GLTF   = 6
} AkProfileType;

typedef struct AkTechniqueFx {
  /* const char * id; */
  /* const char * sid; */
  AkAnnotate           *annotate;
  AkTechniqueFxCommon  *common;
  AkPass               *pass;
  AkTree               *extra;
  struct AkTechniqueFx *next;
} AkTechniqueFx;

typedef struct AkTechniqueOverride {
  const char * ref;
  const char * pass;
} AkTechniqueOverride;

typedef struct AkTechniqueHint {
  struct AkTechniqueHint *next;
  const char             *platform;
  const char             *ref;
  const char             *profile;
  AkProfileType           profileType;
} AkTechniqueHint;

typedef struct AkProfile {
  /* const char * id; */
  AkProfileType     type;
  AkNewParam       *newparam;
  AkTechniqueFx    *technique;
  AkTree           *extra;
  struct AkProfile *next;
} AkProfile;

typedef AkProfile AkProfileCommon;

typedef struct AkProfileCG {
  AkProfile base;

  AkCode     * code;
  AkInclude  * include;
  const char * platform;
} AkProfileCG;

typedef struct AkProfileGLES {
  AkProfile base;

  const char * platform;
} AkProfileGLES;

typedef struct AkProfileGLES2 {
  AkProfile base;

  AkCode     * code;
  AkInclude  * include;
  const char * language;
  const char * platforms;
} AkProfileGLES2;

typedef struct AkProfileGLSL {
  AkProfile base;

  AkCode     * code;
  AkInclude  * include;
  const char * platform;
} AkProfileGLSL;

typedef struct AkProfileBridge {
  AkProfile base;

  const char * platform;
  const char * url;
} AkProfileBridge;

typedef AkProfile AkProfileGLTF;

AkProfile*
ak_profile(struct AkEffect * __restrict effect,
           AkProfile       * __restrict after);

AkProfileType
ak_profileType(struct AkEffect * __restrict effect);

uint32_t
ak_supportedProfiles(AkProfileType ** profileTypes);

void
ak_setSupportedProfiles(AkProfileType profileTypes[],
                        uint32_t      count);

const char*
ak_platform(void);

void
ak_setPlatform(const char platform[64]);

#ifdef __cplusplus
}
#endif
#endif /* ak_profile_h */
