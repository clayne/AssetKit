/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef __libassetio__aio_collada_technique__h_
#define __libassetio__aio_collada_technique__h_

#include "../../include/assetio.h"

typedef struct _xmlNode xmlNode;

int _assetio_hide
aio_load_collada_technique(xmlNode * __restrict xml_node,
                           aio_technique ** __restrict dest);

int _assetio_hide
aio_load_collada_techniquec(xmlNode * __restrict xml_node,
                            aio_technique_common ** __restrict dest);

#endif /* __libassetio__aio_collada_technique__h_ */