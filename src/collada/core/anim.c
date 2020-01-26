/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "anim.h"
#include "asset.h"
#include "source.h"
#include "enums.h"

AkResult _assetkit_hide
dae_anim(AkXmlState   * __restrict xst,
         void         * __restrict memParent,
         void        ** __restrict dest) {
  AkAnimation   *anim;
  AkSource      *last_source;
  AkChannel     *last_channel;
  AkAnimation   *last_anim;
  AkAnimSampler *last_samp;
  AkXmlElmState  xest;

  anim = ak_heap_calloc(xst->heap, memParent, sizeof(*anim));

  ak_xml_readid(xst, anim);
  anim->name = ak_xml_attr(xst, anim, _s_dae_name);

  ak_xest_init(xest, _s_dae_animation)

  last_source  = NULL;
  last_channel = NULL;
  last_anim    = NULL;
  last_samp    = NULL;

  do {
    if (ak_xml_begin(&xest))
    break;

    if (ak_xml_eqelm(xst, _s_dae_asset)) {
      (void)dae_assetInf(xst, anim, NULL);
    } else if (ak_xml_eqelm(xst, _s_dae_source)) {
      AkSource *source;
      AkResult ret;

      /* store interpolation in char */
      ret = dae_source(xst, anim, dae_enumAnimInterp, 1, &source);
      if (ret == AK_OK) {
        if (last_source)
           last_source->next = source;
        else
           anim->source = source;
        last_source = source;
      }
    } else if (ak_xml_eqelm(xst, _s_dae_sampler)) {
      AkAnimSampler *sampler;
      AkResult       ret;

      ret = dae_animSampler(xst, anim, &sampler);
      if (ret == AK_OK) {
        if (last_samp)
          last_samp->next = sampler;
        else
          anim->sampler = sampler;
        last_samp = sampler;
      }
    } else if (ak_xml_eqelm(xst, _s_dae_channel)) {
      AkChannel *channel;
      AkResult   ret;

      ret = dae_channel(xst, anim, &channel);
      if (ret == AK_OK) {
        if (last_channel)
           last_channel->next = channel;
        else
           anim->channel = channel;
        last_channel = channel;
      }
    } else if (ak_xml_eqelm(xst, _s_dae_animation)) {
      AkAnimation *subAnim;
      AkResult     ret;

      ret     = dae_anim(xst, anim, (void **)&subAnim);
      subAnim = NULL;

      if (ret == AK_OK) {
        if (last_anim)
           last_anim->next = subAnim;
        else
           anim->animation = subAnim;
        last_anim = subAnim;
      }
    } else if (ak_xml_eqelm(xst, _s_dae_extra)) {
      dae_extra(xst, anim, &anim->extra);
    } else {
      ak_xml_skipelm(xst);
    }

    /* end element */
    if (ak_xml_end(&xest))
      break;
  } while (xst->nodeRet);

  *dest = anim;

  return AK_OK;
}

AkResult _assetkit_hide
dae_animSampler(AkXmlState     * __restrict xst,
                void           * __restrict memParent,
                AkAnimSampler ** __restrict dest) {
  AkAnimSampler *sampler;
  AkInput       *last_input;
  AkXmlElmState  xest;

  sampler = ak_heap_calloc(xst->heap, memParent, sizeof(*sampler));

  ak_xml_readid(xst, sampler);

  sampler->pre  = ak_xml_attrenum(xst,
                                  _s_dae_pre_behavior,
                                  dae_enumAnimBehavior);
  sampler->post = ak_xml_attrenum(xst,
                                  _s_dae_post_behavior,
                                  dae_enumAnimBehavior);

  last_input = NULL;

  ak_xest_init(xest, _s_dae_sampler);

  do {
    if (ak_xml_begin(&xest))
      break;

    if (ak_xml_eqelm(xst, _s_dae_input)) {
      AkInput *input;

      input = ak_heap_calloc(xst->heap, sampler, sizeof(*input));
      input->semanticRaw = ak_xml_attr(xst, input, _s_dae_semantic);

      if (!input->semanticRaw)
        ak_free(input);
      else {
        AkURL *url;
        AkEnum inputSemantic;

        inputSemantic = dae_enumInputSemantic(input->semanticRaw);
        input->offset = ak_xml_attrui(xst, _s_dae_offset);
        input->set    = ak_xml_attrui(xst, _s_dae_set);

        if (inputSemantic < 0)
          inputSemantic = AK_INPUT_SEMANTIC_OTHER;

        input->semantic = inputSemantic;

        if (last_input)
          last_input->next = input;
        else
          sampler->input = input;

        last_input = input;

        url = ak_xmlAttrGetURL(xst, _s_dae_source, input);
        rb_insert(xst->inputmap, input, url);

        /* check if there are angles, because they are in degress,
           will be converted to radians, we will wait to load whole dae file
           because all sources may not be loaded at this time
         */
        if (input->semantic == AK_INPUT_SEMANTIC_OUTPUT)
          flist_sp_insert(&xst->toRadiansSampelers, sampler);

        switch (input->semantic) {
          case AK_INPUT_SEMANTIC_INPUT:
            sampler->inputInput = input;
            break;
          case AK_INPUT_SEMANTIC_OUTPUT:
            sampler->outputInput = input;
            break;
          case AK_INPUT_SEMANTIC_IN_TANGENT:
            sampler->inTangentInput = input;
            break;
          case AK_INPUT_SEMANTIC_OUT_TANGENT:
            sampler->outTangentInput = input;
            break;
          case AK_INPUT_SEMANTIC_INTERPOLATION:
            sampler->interpInput = input;
            break;
          default:
            break;
        }
      }
    } else {
      ak_xml_skipelm(xst);
    }

    /* end element */
    if (ak_xml_end(&xest))
      break;
  } while (xst->nodeRet);

  *dest = sampler;

  return AK_OK;
}

AkResult _assetkit_hide
dae_channel(AkXmlState  * __restrict xst,
            void        * __restrict memParent,
            AkChannel  ** __restrict dest) {
  AkChannel     *channel;
  AkXmlElmState  xest;

  channel = ak_heap_calloc(xst->heap, memParent, sizeof(*channel));

  ak_xml_attr_url(xst, _s_dae_source, channel, &channel->source);
  channel->target = ak_xml_attr(xst, channel, _s_dae_target);

  ak_xest_init(xest, _s_dae_channel);

  do {
    if (ak_xml_begin(&xest))
      break;
    /* end element */
    if (ak_xml_end(&xest))
       break;
  } while (xst->nodeRet);

  *dest = channel;

  return AK_OK;
}
