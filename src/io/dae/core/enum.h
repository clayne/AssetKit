/*
 * Copyright (C) 2020 Recep Aslantas
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef dae_enums_h
#define dae_enums_h

#include "../../../xml.h"

AK_HIDE AkEnum
dae_semantic(const char * name);

AK_HIDE AkEnum
dae_morphMethod(const xml_attr_t * __restrict xatt);

AK_HIDE AkEnum
dae_nodeType(const xml_attr_t * __restrict xatt);

AK_HIDE AkEnum
dae_animBehavior(const xml_attr_t * __restrict xatt);

AK_HIDE AkEnum
dae_animInterp(const char * name, size_t len);

AK_HIDE AkEnum
dae_wrap(const xml_t * __restrict xml);

AK_HIDE AkEnum
dae_minfilter(const xml_t * __restrict xml);

AK_HIDE AkEnum
dae_mipfilter(const xml_t * __restrict xml);

AK_HIDE AkEnum
dae_magfilter(const xml_t * __restrict xml);

AK_HIDE AkEnum
dae_face(const xml_attr_t * __restrict xatt);

AK_HIDE AkEnum
dae_opaque(const xml_attr_t * __restrict xatt);

AK_HIDE AkEnum
dae_enumChannel(const char *name, size_t len);

AK_HIDE AkEnum
dae_range(const char *name, size_t len);

AK_HIDE AkEnum
dae_precision(const char *name, size_t len);

#endif /* dae_enums_h */
