#!/usr/bin/env python3

# Copyright (c) Facebook, Inc. and its affiliates.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# pyre-unsafe

from enum import Enum
from typing import List, Dict, Type

# pyre-fixme[21]: Could not find `velox.py.type`.
from velox.py.type import Type

# pyre-fixme[21]: Could not find `velox.py.type`.
from velox.py.file import File


class JoinType(Enum):
    INNER = 1
    LEFT = 2
    RIGHT = 3
    FULL = 4

class PlanNode: ...
class PlanBuilder:
    def __init__(self) -> None: ...
    def table_scan(
        self,
        # pyre-fixme[11]: Annotation `Type` is not defined as a type.
        output_schema: Type,
        aliases: Dict[str, str] = {},
        subfields: Dict[str, List[int]] = {},
        row_index: str = "",
        connector_id: str = "prism",
        # pyre-fixme[11]: Annotation `File` is not defined as a type.
        input_files: List[File] = [],
    ) -> PlanBuilder: ...
    def get_plan_node(self) -> PlanBuilder: ...
    def new_builder(self) -> PlanBuilder: ...
    def id(self) -> str: ...
