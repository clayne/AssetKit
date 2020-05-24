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

#include "../common.h"
#include "../skin/fix.h"
#include "edit_common.h"

const char*
ak_mesh_edit_assert1 = "you must call ak_meshBeginEdit* before this op";

AK_EXPORT
void
ak_meshBeginEdit(AkMesh * __restrict mesh) {
  ak_meshBeginEditA(mesh,
                    AK_GEOM_EDIT_FLAG_ARRAYS
                    | AK_GEOM_EDIT_FLAG_INDICES);
}

AK_EXPORT
void
ak_meshBeginEditA(AkMesh  * __restrict mesh,
                  AkGeometryEditFlags  flags) {
  AkHeap           *heap;
  AkObject         *meshobj;
  AkMeshEditHelper *edith;

  edith   = mesh->edith;
  meshobj = ak_objFrom(mesh);
  heap    = ak_heap_getheap(meshobj);

  if (edith && ak_retain(edith) > 1)
    return;

  if (!edith) {
    mesh->edith = edith = ak_heap_calloc(heap,
                                         ak_objFrom(mesh),
                                         sizeof(*edith));
    ak_heap_ext_add(heap,
                    ak__alignof(edith),
                    AK_HEAP_NODE_FLAGS_REFC);
  }

  if ((flags & AK_GEOM_EDIT_FLAG_ARRAYS)
      && !edith->buffers) {
    edith->buffers         = rb_newtree_ptr();
    edith->inputBufferMap  = ak_map_new(ak_cmp_ptr);

    ak_dsSetAllocator(heap->allocator, edith->buffers->alc);

    edith->buffers->onFreeNode = ak_meshFreeRsvBuff;

    edith->flags |= AK_GEOM_EDIT_FLAG_ARRAYS;
  }

  if ((flags & AK_GEOM_EDIT_FLAG_INDICES)
      && !edith->buffers) {
    edith->indices = rb_newtree_ptr();
    edith->flags  |= AK_GEOM_EDIT_FLAG_INDICES;
    ak_dsSetAllocator(heap->allocator, edith->indices->alc);
  }

  ak_retain(edith);
}

AK_EXPORT
void
ak_meshEndEdit(AkMesh * __restrict mesh) {
  AkMeshEditHelper *edith;

  edith = mesh->edith;
  if (!edith)
    return;

  if (ak_refc(edith) > 1) {
    ak_release(edith);
    return;
  }

  /* fix skin weights */
  ak_skinFixWeights(mesh);

  /* finish edit */
  ak_meshFillBuffers(mesh);
  ak_moveIndices(mesh);
  ak_meshMoveBuffers(mesh);

  if (edith->buffers)
    rb_destroy(edith->buffers);

  if (edith->indices)
    rb_destroy(edith->indices);

  if (edith->inputBufferMap)
    ak_map_destroy(edith->inputBufferMap);

  ak_release(edith);
  mesh->edith = NULL;
}
