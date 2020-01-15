/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "test_common.h"
#include "../../src/mem_common.h"
#include "../../src/mem_lt.h"

extern AkHeapAllocator ak__allocator;

TEST_IMPL(heap) {
  AkHeap  *heap, *other, staticHeap;
  uint32_t heapid, data;

  heap = ak_heap_new(NULL, NULL, NULL);
  ASSERT(heap->allocator == &ak__allocator);
  ASSERT(ak_heap_allocator(heap) == &ak__allocator);

  heapid = heap->heapid;
  ASSERT(heapid > 0);
  ASSERT(ak_heap_lt_find(heap->heapid) == heap);

  other = ak_heap_new(NULL, NULL, NULL);

  ak_heap_attach(heap, other);
  ASSERT(heap->chld == other);

  ak_heap_dettach(heap, other);
  ASSERT(heap->chld == NULL);

  ak_heap_attach(heap, other);
  ASSERT(heap->chld == other);

  ak_heap_setdata(heap, &data);
  ASSERT(ak_heap_data(heap) == &data);

  ak_heap_destroy(heap);
  ASSERT(ak_heap_lt_find(heapid) == NULL);

  ak_heap_init(&staticHeap, NULL, NULL, NULL);
  ASSERT(staticHeap.heapid > 0);

  ak_heap_lt_remove(staticHeap.heapid);
  ASSERT(ak_heap_lt_find(staticHeap.heapid) == NULL);

  TEST_SUCCESS
}

TEST_IMPL(heap_multiple) {
  AkHeap  *heap, *root;
  uint32_t i;

  root = ak_heap_new(NULL, NULL, NULL);

  /* multiple alloc, leak */
  for (i = 0; i < 1000; i++)
    heap = ak_heap_new(NULL, NULL, NULL);

  /* multiple alloc 2, leak */
  for (i = 0; i < 1000; i++)
    heap = ak_heap_new(NULL, NULL, NULL);

  /* multiple alloc-free 1 */
  for (i = 0; i < 1000; i++) {
    heap = ak_heap_new(NULL, NULL, NULL);
    ak_heap_destroy(heap);
  }

  /* multiple alloc-free 2 */
  for (i = 0; i < 1000; i++) {
    heap = ak_heap_new(NULL, NULL, NULL);
    ak_heap_destroy(heap);
  }

  /* multiple alloc, attach to parent */
  for (i = 0; i < 1000; i++) {
    heap = ak_heap_new(NULL, NULL, NULL);
    ak_heap_attach(root, heap);
  }

  /* multiple alloc, attach to parent */
  for (i = 0; i < 1000; i++) {
    heap = ak_heap_new(NULL, NULL, NULL);
    ak_heap_attach(root, heap);
  }

  ak_heap_destroy(root);

  root = ak_heap_new(NULL, NULL, NULL);

  /* multiple alloc, attach-detach to parent */
  for (i = 0; i < 1000; i++) {
    heap = ak_heap_new(NULL, NULL, NULL);
    ak_heap_attach(root, heap);
    ak_heap_dettach(root, heap);
  }

  TEST_SUCCESS
}
