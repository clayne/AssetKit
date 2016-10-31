/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../ak_common.h"
#include "../ak_memory_common.h"
#include "ak_coord_common.h"
#include <cglm.h>

AK_EXPORT
void
ak_coordCvtTransform(AkCoordSys *oldCoordSystem,
                     AkFloat4x4  oldTransform,
                     AkCoordSys *newCoordSystem,
                     AkFloat4x4  newTransform) {
  mat4           rot, scale;
  vec3           scalev, angles, tmp;
  AkAxisAccessor a0, a1;
  char           eulerNew[3];
  const char     eulerXYZ[3] = {0, 1, 2};
  AkAxisRotDirection rotDirection;

  rotDirection = (oldCoordSystem->rotDirection + 1)
                      * (newCoordSystem->rotDirection + 1);

  ak_coordAxisAccessors(oldCoordSystem, newCoordSystem, &a0, &a1);

  /* decompose rotation and scaling factors */
  glm_decompose_rs(oldTransform, rot, scalev);

  /* extract euler angles XYZ */
  glm_euler_angles(rot, angles);
  AK_CVT_VEC(angles);

  /* find new euler sequence */
  AK_CVT_VEC_NOSIGN_TO(eulerXYZ, eulerNew);

  /* apply new rotation direction */
  glm_vec_scale(angles, rotDirection, angles);

  /* apply new rotation */
  glm_euler_by_order(angles,
                     glm_euler_order(eulerNew),
                     rot);

  /* find new scaling factors */
  AK_CVT_VEC_NOSIGN(scalev);

  /* apply new scaling factors */
  glm_mat4_dup(GLM_MAT4_IDENTITY, scale);
  scale[0][0] = scalev[0];
  scale[1][1] = scalev[1];
  scale[2][2] = scalev[2];

  glm_mul(rot, scale, newTransform);

  /* apply new translation */
  AK_CVT_VEC_TO(oldTransform[3], newTransform[3])

  /* duplicate w item directly to new coordinate sys without normalization */
  newTransform[0][3] = 0;
  newTransform[1][3] = 0;
  newTransform[2][3] = 0;
  newTransform[3][3] = oldTransform[3][3];
}