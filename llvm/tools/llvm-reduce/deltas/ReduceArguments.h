//===- ReduceArguments.h - Specialized Delta Pass -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements a function which calls the Generic Delta pass in order
// to reduce uninteresting Arguments from defined functions.
//
//===----------------------------------------------------------------------===//

#include "Delta.h"
#include "llvm/IR/Argument.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Cloning.h"

namespace llvm {
void reduceArgumentsDeltaPass(TestRunner &Test);
} // namespace llvm
