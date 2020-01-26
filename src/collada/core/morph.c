/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "morph.h"
#include "source.h"
#include "enums.h"
#include "../../array.h"

AkResult _assetkit_hide
dae_morph(AkXmlState * __restrict xst,
          void * __restrict memParent,
          bool asObject,
          AkMorph ** __restrict dest) {
  AkObject    *obj;
  AkMorph     *morph;
  AkSource    *last_source;
  void         *memPtr;
  AkXmlElmState xest;

  if (asObject) {
    obj = ak_objAlloc(xst->heap,
                      memParent,
                      sizeof(*morph),
                      AK_CONTROLLER_MORPH,
                      true);

    morph = ak_objGet(obj);

    memPtr = obj;
  } else {
    morph = ak_heap_calloc(xst->heap, memParent, sizeof(*morph));
    memPtr = morph;
  }

  ak_xml_attr_url(xst, _s_dae_source, memPtr, &morph->baseGeom);

  morph->method = ak_xml_attrenum_def(xst,
                                      _s_dae_method,
                                      dae_enumMorphMethod,
                                      AK_MORPH_METHOD_NORMALIZED);
  last_source = NULL;

  ak_xest_init(xest, _s_dae_morph)

  do {
    if (ak_xml_begin(&xest))
      break;

    if (ak_xml_eqelm(xst, _s_dae_source)) {
      AkSource *source;
      AkResult ret;

      ret = dae_source(xst, memPtr, NULL, 0, &source);
      if (ret == AK_OK) {
        if (last_source)
          last_source->next = source;
        else
          morph->source = source;

        last_source = source;
      }
    } else if (ak_xml_eqelm(xst, _s_dae_targets)) {
      AkTargets    *targets;
      AkInput      *last_input;
      AkXmlElmState xest2;

      targets = ak_heap_calloc(xst->heap,
                               memPtr,
                               sizeof(*targets));

      last_input = NULL;

      ak_xest_init(xest2, _s_dae_targets)

      do {
        if (ak_xml_begin(&xest2))
            break;

        if (ak_xml_eqelm(xst, _s_dae_input)) {
          AkInput *input;

          input = ak_heap_calloc(xst->heap, targets, sizeof(*input));
          input->semanticRaw = ak_xml_attr(xst, input, _s_dae_semantic);

          if (!input->semanticRaw)
            ak_free(input);
          else {
            AkURL *url;
            AkEnum inputSemantic;

            inputSemantic = dae_enumInputSemantic(input->semanticRaw);

            if (inputSemantic < 0)
              inputSemantic = AK_INPUT_SEMANTIC_OTHER;

            input->semantic = inputSemantic;

            url = ak_xmlAttrGetURL(xst, _s_dae_source, input);
            rb_insert(xst->inputmap, input, url);

            if (last_input)
              last_input->next = input;
            else
              targets->input = input;

            last_input = input;
          }
        } else if (ak_xml_eqelm(xst, _s_dae_extra)) {
          dae_extra(xst, targets, &targets->extra);
        } else {
          ak_xml_skipelm(xst);
        }

        /* end element */
        if (ak_xml_end(&xest2))
          break;
      } while (xst->nodeRet);

      morph->targets = targets;
    } else if (ak_xml_eqelm(xst, _s_dae_extra)) {
      dae_extra(xst, memPtr, &morph->extra);
    } else {
      ak_xml_skipelm(xst);
    }

    /* end element */
    if (ak_xml_end(&xest))
      break;
  } while (xst->nodeRet);

  *dest = morph;

  return AK_OK;
}
