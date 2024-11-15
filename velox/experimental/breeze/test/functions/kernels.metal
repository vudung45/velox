/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Copyright (c) 2024 by Rivos Inc.
 * Licensed under the Apache License, Version 2.0, see LICENSE for details.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "test/generated/functions/kernels-metal.h"

// kernel specializations

using namespace breeze::functions;

#define _C(X, Y) X##Y
#define C(X, Y) _C(X, Y)

#define NAME(F, T, BT, IPT) C(block_, F##_##T##_##BT##x##IPT)

#define GEN_LOAD(T)                                                          \
  kernel void NAME(load, T, 4, 2)(                                           \
      const device T *in [[buffer(0)]], device T *out [[buffer(1)]],         \
      const device int *num_items [[buffer(2)]],                             \
      uint thread_idx [[thread_index_in_threadgroup]]) {                     \
    block_load<4, 2>(MetalPlatform<4, WARP_THREADS>{thread_idx, 0}, in, out, \
                     *num_items);                                            \
  }

GEN_LOAD(char)
GEN_LOAD(uchar)
GEN_LOAD(int)
GEN_LOAD(uint)
GEN_LOAD(float)

#define GEN_LOAD_IF(T)                                                        \
  kernel void NAME(load_if, T, 4, 2)(                                         \
      const device T *in [[buffer(0)]],                                       \
      const device int *in_selection_flags [[buffer(1)]],                     \
      device T *out [[buffer(2)]], const device int *num_items [[buffer(3)]], \
      uint thread_idx [[thread_index_in_threadgroup]]) {                      \
    block_load_if<4, 2>(MetalPlatform<4, WARP_THREADS>{thread_idx, 0}, in,    \
                        in_selection_flags, out, *num_items);                 \
  }

GEN_LOAD_IF(char)
GEN_LOAD_IF(uchar)
GEN_LOAD_IF(int)
GEN_LOAD_IF(uint)
GEN_LOAD_IF(float)

#define GEN_LOAD_FROM(T)                                                     \
  kernel void NAME(load_from, T, 4, 2)(                                      \
      const device T *in [[buffer(0)]],                                      \
      const device int *offsets [[buffer(1)]], device T *out [[buffer(2)]],  \
      const device int *num_items [[buffer(3)]],                             \
      uint thread_idx [[thread_index_in_threadgroup]]) {                     \
    block_load_from<4, 2>(MetalPlatform<4, WARP_THREADS>{thread_idx, 0}, in, \
                          offsets, out, *num_items);                         \
  }

GEN_LOAD_FROM(char)
GEN_LOAD_FROM(uchar)
GEN_LOAD_FROM(int)
GEN_LOAD_FROM(uint)
GEN_LOAD_FROM(float)

#define GEN_STORE(T)                                                          \
  kernel void NAME(store, T, 4, 2)(                                           \
      const device T *in [[buffer(0)]], device T *out [[buffer(1)]],          \
      const device int *num_items [[buffer(2)]],                              \
      uint thread_idx [[thread_index_in_threadgroup]]) {                      \
    block_store<4, 2>(MetalPlatform<4, WARP_THREADS>{thread_idx, 0}, in, out, \
                      *num_items);                                            \
  }

GEN_STORE(char)
GEN_STORE(uchar)
GEN_STORE(int)
GEN_STORE(uint)
GEN_STORE(float)

#define GEN_STORE_IF(T)                                                       \
  kernel void NAME(store_if, T, 4, 2)(                                        \
      const device T *in [[buffer(0)]],                                       \
      const device int *selection_flags [[buffer(1)]],                        \
      device T *out [[buffer(2)]], const device int *num_items [[buffer(3)]], \
      uint thread_idx [[thread_index_in_threadgroup]]) {                      \
    block_store_if<4, 2>(MetalPlatform<4, WARP_THREADS>{thread_idx, 0}, in,   \
                         selection_flags, out, *num_items);                   \
  }

GEN_STORE_IF(char)
GEN_STORE_IF(uchar)
GEN_STORE_IF(int)
GEN_STORE_IF(uint)
GEN_STORE_IF(float)

#define GEN_STORE_AT(T)                                                     \
  kernel void NAME(store_at, T, 1, 8)(                                      \
      const device T *in [[buffer(0)]],                                     \
      const device int *offsets [[buffer(1)]], device T *out [[buffer(2)]], \
      const device int *num_items [[buffer(3)]],                            \
      uint thread_idx [[thread_index_in_threadgroup]]) {                    \
    block_store_at<1, 8>(MetalPlatform<1, WARP_THREADS>{thread_idx, 0}, in, \
                         offsets, out, *num_items);                         \
  }

GEN_STORE_AT(char)
GEN_STORE_AT(uchar)
GEN_STORE_AT(int)
GEN_STORE_AT(uint)
GEN_STORE_AT(float)

#define GEN_STORE_AT_IF(T)                                                     \
  kernel void NAME(store_at_if, T, 1, 8)(                                      \
      const device T *in [[buffer(0)]],                                        \
      const device int *offsets [[buffer(1)]],                                 \
      const device int *selection_flags [[buffer(2)]],                         \
      device T *out [[buffer(3)]], const device int *num_items [[buffer(4)]],  \
      uint thread_idx [[thread_index_in_threadgroup]]) {                       \
    block_store_at_if<1, 8>(MetalPlatform<1, WARP_THREADS>{thread_idx, 0}, in, \
                            offsets, selection_flags, out, *num_items);        \
  }

GEN_STORE_AT_IF(char)
GEN_STORE_AT_IF(uchar)
GEN_STORE_AT_IF(int)
GEN_STORE_AT_IF(uint)
GEN_STORE_AT_IF(float)

#define GEN_FILL(T)                                                        \
  kernel void NAME(fill, T, 4, 2)(                                         \
      const device T *value [[buffer(0)]], device T *out [[buffer(1)]],    \
      const device int *num_items [[buffer(2)]],                           \
      uint thread_idx [[thread_index_in_threadgroup]]) {                   \
    block_fill<4, 2>(MetalPlatform<4, WARP_THREADS>{thread_idx, 0}, value, \
                     out, *num_items);                                     \
  }

GEN_FILL(char)
GEN_FILL(uchar)
GEN_FILL(int)
GEN_FILL(uint)
GEN_FILL(float)

#define GEN_FILL_AT_IF(T)                                                     \
  kernel void NAME(fill_at_if, T, 1, 8)(                                      \
      const device T *value [[buffer(0)]],                                    \
      const device int *offsets [[buffer(1)]],                                \
      const device int *selection_flags [[buffer(2)]],                        \
      device T *out [[buffer(3)]], const device int *num_items [[buffer(4)]], \
      uint thread_idx [[thread_index_in_threadgroup]]) {                      \
    block_fill_at_if<1, 8>(MetalPlatform<1, WARP_THREADS>{thread_idx, 0},     \
                           value, offsets, selection_flags, out, *num_items); \
  }

GEN_FILL_AT_IF(char)
GEN_FILL_AT_IF(uchar)
GEN_FILL_AT_IF(int)
GEN_FILL_AT_IF(uint)
GEN_FILL_AT_IF(float)

#define add_reduce_op ReduceOpAdd
#define min_reduce_op ReduceOpMin
#define max_reduce_op ReduceOpMax

#define GEN_REDUCE_T(O, T, BT, IPT)                                         \
  kernel void NAME(reduce_##O##_##T, T, BT, IPT)(                           \
      const device T *in [[buffer(0)]], device T *out [[buffer(1)]],        \
      const device int *num_items [[buffer(2)]],                            \
      uint thread_idx [[thread_index_in_threadgroup]]) {                    \
    MetalPlatform<BT, WARP_THREADS> p{thread_idx, 0};                       \
    threadgroup BlockReduce<decltype(p), T>::Scratch scratch;               \
    block_reduce<O##_reduce_op, BT, IPT>(p, in, out, &scratch, *num_items); \
  }

#define GEN_REDUCE(O)           \
  GEN_REDUCE_T(O, int, 32, 2)   \
  GEN_REDUCE_T(O, int, 64, 1)   \
  GEN_REDUCE_T(O, uint, 32, 2)  \
  GEN_REDUCE_T(O, uint, 64, 1)  \
  GEN_REDUCE_T(O, float, 32, 2) \
  GEN_REDUCE_T(O, float, 64, 1)

GEN_REDUCE(add)
GEN_REDUCE(min)
GEN_REDUCE(max)

#define add_scan_op ScanOpAdd

#define GEN_SCAN_T(O, T, BT, IPT)                                       \
  kernel void NAME(scan_##O##_##T, T, BT, IPT)(                         \
      const device T *in [[buffer(0)]], device T *out [[buffer(1)]],    \
      const device int *num_items [[buffer(2)]],                        \
      uint thread_idx [[thread_index_in_threadgroup]]) {                \
    MetalPlatform<BT, WARP_THREADS> p{thread_idx, 0};                   \
    threadgroup BlockScan<decltype(p), T, IPT>::Scratch scratch;        \
    block_scan<O##_scan_op, BT, IPT>(p, in, out, &scratch, *num_items); \
  }

#define GEN_SCAN(O)           \
  GEN_SCAN_T(O, int, 32, 2)   \
  GEN_SCAN_T(O, int, 64, 1)   \
  GEN_SCAN_T(O, uint, 32, 2)  \
  GEN_SCAN_T(O, uint, 64, 1)  \
  GEN_SCAN_T(O, float, 32, 2) \
  GEN_SCAN_T(O, float, 64, 1)

GEN_SCAN(add)

#define GEN_RADIX_RANK(T, BT, IPT, RB)                                 \
  kernel void NAME(radix_rank, T, BT, IPT##x##RB)(                     \
      const device T *in [[buffer(0)]], device int *out [[buffer(1)]], \
      const device int *num_items [[buffer(2)]],                       \
      uint thread_idx [[thread_index_in_threadgroup]]) {               \
    MetalPlatform<BT, WARP_THREADS> p{thread_idx, 0};                  \
    threadgroup BlockRadixRank<decltype(p), IPT, RB>::Scratch scratch; \
    block_radix_rank<BT, IPT, RB>(p, in, out, &scratch, *num_items);   \
  }

GEN_RADIX_RANK(int, 64, 2, 6)
GEN_RADIX_RANK(uint, 64, 2, 6)

#define GEN_RADIX_SORT(T, BT, IPT, RB)                                    \
  kernel void NAME(radix_sort, T, BT, IPT##x##RB)(                        \
      const device T *in [[buffer(0)]], device T *out [[buffer(1)]],      \
      const device int *num_items [[buffer(2)]],                          \
      uint thread_idx [[thread_index_in_threadgroup]]) {                  \
    MetalPlatform<BT, WARP_THREADS> p{thread_idx, 0};                     \
    threadgroup BlockRadixSort<decltype(p), IPT, RB, T>::Scratch scratch; \
    block_radix_sort<BT, IPT, RB>(p, in, out, &scratch, *num_items);      \
  }

GEN_RADIX_SORT(int, 64, 2, 6)
GEN_RADIX_SORT(uint, 64, 2, 6)
