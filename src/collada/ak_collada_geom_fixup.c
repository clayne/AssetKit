/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_collada_geom_fixup.h"
#include "ak_collada_mesh_fixup.h"

AkResult _assetkit_hide
ak_dae_geom_fixup_all(AkDoc * doc) {
  AkLibItem  *geomLib;
  AkGeometry *geom;

  geomLib = doc->lib.geometries;
  while (geomLib) {
    geom = geomLib->chld;
    while (geom) {
      ak_dae_geom_fixup(geom);
      geom = geom->next;
    }

    geomLib = geomLib->next;
  }

  return AK_OK;
}

AkResult _assetkit_hide
ak_dae_geom_fixup(AkGeometry * geom) {
  AkObject *primitive;

  primitive = geom->gdata;
  switch ((AkGeometryType)primitive->type) {
    case AK_GEOMETRY_TYPE_MESH:
      ak_dae_mesh_fixup(ak_objGet(primitive));
    default:
      break;
  }

  return AK_OK;
}
