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

#pragma once

#include <cstdint>

namespace facebook::velox::config {

struct GlobalConfiguration {
  /// Number of shared leaf memory pools per process.
  int32_t memoryNumSharedLeafPools{32};
  /// If true, check fails on any memory leaks in memory pool and memory
  /// manager.
  bool memoryLeakCheckEnabled{false};
  /// If true, 'MemoryPool' will be running in debug mode to track the
  /// allocation and free call sites to detect the source of memory leak for
  /// testing purpose.
  bool memoryPoolDebugEnabled{false};
  /// If true, enable memory usage tracking in the default memory pool.
  bool enableMemoryUsageTrackInDefaultMemoryPool{false};
  /// Record time and volume for large allocation/free.
  bool timeAllocations{false};
  /// Use explicit huge pages
  bool memoryUseHugepages{false};
  /// If true, suppress the verbose error message in memory capacity exceeded
  /// exception. This is only used by test to control the test error output
  /// size.
  bool suppressMemoryCapacityExceedingErrorMessage{false};
  /// Whether allow to memory capacity transfer between memory pools from
  /// different tasks, which might happen in use case like Spark-Gluten
  bool memoryPoolCapacityTransferAcrossTasks{false};
  /// Enable the stacktrace for system type of VeloxException.
  bool exceptionSystemStacktraceEnabled{true};
  /// Enable the stacktrace for user type of VeloxException.
  bool exceptionUserStacktraceEnabled{false};
  /// Min time interval in milliseconds between stack traces captured in
  /// user type of VeloxException; off when set to 0 (the default).
  int32_t exceptionUserStacktraceRateLimitMs{0};
  /// Min time interval in milliseconds between stack traces captured in
  /// system type of VeloxException; off when set to 0 (the default).
  int32_t exceptionSystemStacktraceRateLimitMs{0};
};

extern GlobalConfiguration globalConfig;

} // namespace facebook::velox::config
