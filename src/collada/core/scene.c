/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "dae_scene.h"
#include "../core/dae_asset.h"
#include "dae_node.h"
#include "dae_evalscn.h"
#include "../../../include/ak/light.h"

AkResult _assetkit_hide
dae_visualScene(AkXmlState * __restrict xst,
                void * __restrict memParent,
                void ** __restrict dest) {
  AkVisualScene   *visualScene;
  AkNode          *last_node;
  AkEvaluateScene *last_evaluateScene;
  AkXmlElmState    xest;

  visualScene = ak_heap_calloc(xst->heap,
                               memParent,
                               sizeof(*visualScene));
  ak_setypeid(visualScene, AKT_SCENE);

  ak_xml_readid(xst, visualScene);
  visualScene->name = ak_xml_attr(xst, visualScene, _s_dae_name);

  last_node          = NULL;
  last_evaluateScene = NULL;

  visualScene->cameras = ak_heap_calloc(xst->heap,
                                        visualScene,
                                        sizeof(*visualScene->cameras));
  visualScene->lights = ak_heap_calloc(xst->heap,
                                       visualScene,
                                       sizeof(*visualScene->lights));

  ak_xest_init(xest, _s_dae_visual_scene)

  do {
    if (ak_xml_begin(&xest))
      break;

    if (ak_xml_eqelm(xst, _s_dae_asset)) {
      (void)dae_assetInf(xst, visualScene, NULL);
    } else if (ak_xml_eqelm(xst, _s_dae_node)) {
      AkNode  *node;
      AkResult ret;

      ret = dae_node(xst, visualScene, visualScene, &node);
      if (ret == AK_OK) {
        if (last_node)
          last_node->next = node;
        else
          visualScene->node = node;

        node->prev = last_node;
        last_node  = node;
      }
    } else if (ak_xml_eqelm(xst, _s_dae_evaluate_scene)) {
      AkEvaluateScene *evaluateScene;
      AkResult ret;

      ret = dae_evaluateScene(xst, visualScene, &evaluateScene);
      if (ret == AK_OK) {
        if (last_evaluateScene)
          last_evaluateScene->next = evaluateScene;
        else
          visualScene->evaluateScene = evaluateScene;

        last_evaluateScene = evaluateScene;
      }
    } else if (ak_xml_eqelm(xst, _s_dae_extra)) {
      dae_extra(xst, visualScene, &visualScene->extra);
    } else {
      ak_xml_skipelm(xst);
    }

    /* end element */
    if (ak_xml_end(&xest))
      break;
  } while (xst->nodeRet);

  if (visualScene->lights->count < 1
      && ak_opt_get(AK_OPT_ADD_DEFAULT_LIGHT)) {
    AkLight *light;
    AkNode  *rootNode;
    rootNode = visualScene->node;
    if (rootNode) {
      AkHeap         *heap;
      AkDoc          *doc;
      AkInstanceBase *lightInst;

      heap  = ak_heap_getheap(rootNode);
      doc   = ak_heap_data(heap);
      light = ak_defaultLight(rootNode);

      lightInst = ak_instanceMake(heap, rootNode, light);
      ak_instanceListEmpty(visualScene->lights);
      ak_instanceListAdd(visualScene->lights, lightInst);

      lightInst->next = rootNode->light;
      rootNode->light = lightInst;

      ak_libAddLight(doc, light);
    }
  }

  if (ak_opt_get(AK_OPT_COORD_CONVERT_TYPE) != AK_COORD_CVT_DISABLED)
    ak_fixSceneCoordSys(visualScene);

  *dest = visualScene;

  return AK_OK;
}

AkResult _assetkit_hide
dae_instanceVisualScene(AkXmlState * __restrict xst,
                        void * __restrict memParent,
                        AkInstanceBase ** __restrict dest) {
  AkInstanceBase *visualScene;
  AkXmlElmState   xest;

  visualScene = ak_heap_calloc(xst->heap,
                               memParent,
                               sizeof(*visualScene));

  ak_xml_readsid(xst, visualScene);

  visualScene->name = ak_xml_attr(xst, visualScene, _s_dae_name);

  ak_xml_attr_url(xst,
                  _s_dae_url,
                  visualScene,
                  &visualScene->url);

  ak_xest_init(xest, _s_dae_instance_visual_scene)

  do {
    if (ak_xml_begin(&xest))
      break;

    if (ak_xml_eqelm(xst, _s_dae_extra)) {
      dae_extra(xst, visualScene, &visualScene->extra);
    } else {
      ak_xml_skipelm(xst);
    }

    /* end element */
    if (ak_xml_end(&xest))
      break;
  } while (xst->nodeRet);

  *dest = visualScene;

  return AK_OK;
}

void _assetkit_hide
dae_scene(xml_t * __restrict xml,
          void  * __restrict userdata) {
  DAEState       *dst;
  AkInstanceBase *visualScene;
  AkDoc          *doc;
  AkHeap         *heap;

  dst  = userdata;
  heap = dst->heap;
  doc  = dst->doc;
  xml  = xml->val;

  while (xml) {
    if (xml_tag_eq(xml, _s_dae_instance_visual_scene)) {
      AkInstanceBase *visualScene;
      AkResult ret;
      
      ret = dae_instanceVisualScene(xst, memParent, &visualScene);
      if (ret == AK_OK)
        dest->visualScene = visualScene;
      
    } else if (xml_tag_eq(xml, _s_dae_extra)) {
      doc->scene.extra = tree_fromxml(heap, doc, xml);
    }
    
    geom              = ak_heap_calloc(heap, doc, sizeof(*geom));
    geom->name        = xmla_strdup_by(xml, heap, _s_dae_name, geom);
    geom->materialMap = ak_map_new(ak_cmp_str);
    
    xmla_setid(xml, heap, geom);
  }
  
  AkXmlElmState xest;

  ak_xest_init(xest, _s_dae_scene)

  do {
    if (ak_xml_begin(&xest))
      break;

    if (ak_xml_eqelm(xst, _s_dae_instance_visual_scene)) {
      AkInstanceBase *visualScene;
      AkResult ret;

      ret = dae_instanceVisualScene(xst, memParent, &visualScene);
      if (ret == AK_OK)
        dest->visualScene = visualScene;

    } else if (ak_xml_eqelm(xst, _s_dae_extra)) {
      dae_extra(xst, memParent, &dest->extra);
    }

    /* end element */
    if (ak_xml_end(&xest))
      break;
  } while (xst->nodeRet);
  
  return AK_OK;
}
