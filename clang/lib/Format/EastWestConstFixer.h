//===--- EastWestConstFixer.h -----------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file declares EastWestConstFixer, a TokenAnalyzer that
/// enforces either east or west const depending on the style.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_FORMAT_EASTWESTCONSTFIXER_H
#define LLVM_CLANG_LIB_FORMAT_EASTWESTCONSTFIXER_H

#include "TokenAnalyzer.h"

namespace clang {
namespace format {

class EastWestConstFixer : public TokenAnalyzer {
public:
  EastWestConstFixer(const Environment &Env, const FormatStyle &Style);

  std::pair<tooling::Replacements, unsigned>
  analyze(TokenAnnotator &Annotator,
          SmallVectorImpl<AnnotatedLine *> &AnnotatedLines,
          FormatTokenLexer &Tokens) override;
};

} // end namespace format
} // end namespace clang

#endif
