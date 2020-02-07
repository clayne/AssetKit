/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "geom.h"
#include "../core/asset.h"
#include "mesh.h"
#include "spline.h"
#include "../brep/brep.h"

_assetkit_hide
void
dae_geom(xml_t * __restrict xml, void * __restrict userdata) {
  DAEState    *dst;
  AkGeometry  *geom;
  AkDoc       *doc;
  AkHeap      *heap;
  xml_t       *xgeom;

  dst  = userdata;
  heap = dst->heap;
  doc  = dst->doc;
  xml  = xml->val;

  /* geometr,es */
  while (xml) {
    if (xml_tag_eq(xml, _s_dae_geometry)) {
      xgeom = xml->val;

      /* geometry */
      while (xgeom) {
        geom              = ak_heap_calloc(heap, doc, sizeof(*geom));
        geom->name        = xmla_strdup_by(xgeom, heap, _s_dae_name, geom);
        geom->materialMap = ak_map_new(ak_cmp_str);
        
        xmla_setid(xgeom, heap, geom);
        
        /* destroy heap with this object */
        ak_setAttachedHeap(geom, geom->materialMap->heap);
        ak_setypeid(geom, AKT_GEOMETRY);
        
        if (xml_tag_eq(xgeom, _s_dae_asset)) {
          dae_asset(xgeom, dst);
        } else if (xml_tag_eq(xgeom, _s_dae_mesh)
                   || xml_tag_eq(xgeom, _s_dae_convex_mesh)) {
          geom->gdata = dae_mesh(dst, xgeom, geom);
        } else if (xml_tag_eq(xgeom, _s_dae_spline)) {
          geom->gdata = dae_spline(dst, xgeom, geom);
        } else if (xml_tag_eq(xgeom, _s_dae_brep)) {
          geom->gdata = dae_brep(dst, xgeom, geom);
        } else if (xml_tag_eq(xgeom, _s_dae_extra)) {
          geom->extra = tree_fromxml(heap, geom, xgeom);
        }

        xgeom = xgeom->next;
      }
    }
    xml = xml->next;
  }
}
