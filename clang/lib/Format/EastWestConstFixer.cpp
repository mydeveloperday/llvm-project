//===--- EastWestConstFixer.cpp ---------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements EastWestConstFixer, a TokenAnalyzer that
/// enforces either east or west const depending on the style.
///
//===----------------------------------------------------------------------===//

#include "EastWestConstFixer.h"
#include "FormatToken.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/Regex.h"

#include <algorithm>

namespace clang {
namespace format {

static void replaceToken(const SourceManager &SourceMgr,
                         tooling::Replacements &Fixes,
                         const CharSourceRange &Range, std::string NewText) {
  auto Replacement = tooling::Replacement(SourceMgr, Range, NewText);
  auto Err = Fixes.add(Replacement);

  if (Err) {
    llvm::errs() << "Error while rearranging const : "
                 << llvm::toString(std::move(Err)) << "\n";
  }
}

static void removeToken(const SourceManager &SourceMgr,
                        tooling::Replacements &Fixes,
                        const FormatToken *First) {
  auto Range = CharSourceRange::getCharRange(First->getStartOfNonWhitespace(),
                                             First->Tok.getEndLoc());
  replaceToken(SourceMgr, Fixes, Range, "");
}

static void insertConstAfter(const SourceManager &SourceMgr,
                             tooling::Replacements &Fixes,
                             const FormatToken *First) {
  FormatToken *Next = First->Next;
  if (!Next) {
    return;
  }
  auto Range = CharSourceRange::getCharRange(
      Next->getStartOfNonWhitespace(), Next->Next->getStartOfNonWhitespace());

  std::string NewText = " const ";
  NewText += Next->TokenText;

  replaceToken(SourceMgr, Fixes, Range, NewText);
}

static void insertConstBefore(const SourceManager &SourceMgr,
                              tooling::Replacements &Fixes,
                              const FormatToken *First) {
  auto Range = CharSourceRange::getCharRange(
      First->getStartOfNonWhitespace(), First->Next->getStartOfNonWhitespace());

  std::string NewText = " const ";
  NewText += First->TokenText;

  replaceToken(SourceMgr, Fixes, Range, NewText);
}

static void rotateTokens(const SourceManager &SourceMgr,
                         tooling::Replacements &Fixes, const FormatToken *First,
                         const FormatToken *Last, bool Left) {
  auto *End = Last;
  auto *Begin = First;
  if (!Left) {
    End = Last->Next;
    Begin = First->Next;
  }

  std::string NewText;
  // If we are rotating to the left we move the Last token to the front.
  if (Left) {
    NewText += Last->TokenText;
    NewText += " ";
  }

  // Then move through the other tokens.
  auto *Tok = Begin;
  while (Tok != End) {
    if (!NewText.empty())
      NewText += " ";

    NewText += Tok->TokenText;
    Tok = Tok->Next;
  }

  // If we are rotating to the right we move the first token to the back.
  if (!Left) {
    NewText += " ";
    NewText += First->TokenText;
  }

  auto Range = CharSourceRange::getCharRange(First->getStartOfNonWhitespace(),
                                             Last->Tok.getEndLoc());

  replaceToken(SourceMgr, Fixes, Range, NewText);
}

static bool isCVQualifierOrType(const FormatToken *Tok) {
  return Tok && (Tok->isSimpleTypeSpecifier() ||
                 Tok->isOneOf(tok::kw_volatile, tok::kw_auto));
}

// If a token is an identifier and it's upper case, it could
// be a macro and hence we need to be able to ignore it.
static bool isPossibleMacro(const FormatToken *Tok) {
  if (!Tok)
    return false;

  if (!Tok->is(tok::identifier))
    return false;

  if (Tok->TokenText.upper() == Tok->TokenText.str())
    return true;

  return false;
}

static FormatToken *analyzeEast(const SourceManager &SourceMgr,
                                const AdditionalKeywords &Keywords,
                                tooling::Replacements &Fixes,
                                FormatToken *Tok) {
  // We only need to think about streams that begin with const.
  if (!Tok->is(tok::kw_const)) {
    return Tok;
  }
  // Don't concern yourself if nothing follows const.
  if (!Tok->Next) {
    return Tok;
  }
  if (isPossibleMacro(Tok->Next)) {
    return Tok;
  }
  FormatToken *Const = Tok;

  FormatToken *Qualifier = Tok->Next;
  FormatToken *LastQualifier = Qualifier;
  while (Qualifier && isCVQualifierOrType(Qualifier)) {
    LastQualifier = Qualifier;
    Qualifier = Qualifier->Next;
  }
  if (LastQualifier && Qualifier != LastQualifier) {
    rotateTokens(SourceMgr, Fixes, Const, LastQualifier, /*Left=*/false);
    Tok = LastQualifier;
  } else if (Tok->startsSequence(tok::kw_const, tok::identifier,
                                 TT_TemplateOpener)) {
    // Read from the TemplateOpener to
    // TemplateCloser as in const ArrayRef<int> a; const ArrayRef<int> &a;
    FormatToken *EndTemplate = Tok->Next->Next->MatchingParen;
    if (EndTemplate) {
      // Move to the end of any template class members e.g.
      // `Foo<int>::iterator`.
      if (EndTemplate->startsSequence(TT_TemplateCloser, tok::coloncolon,
                                      tok::identifier)) {
        EndTemplate = EndTemplate->Next->Next;
      }
    }
    if (EndTemplate && EndTemplate->Next &&
        !EndTemplate->Next->isOneOf(tok::equal, tok::l_paren)) {
      // Remove the const.
      insertConstAfter(SourceMgr, Fixes, EndTemplate);
      removeToken(SourceMgr, Fixes, Tok);
      return Tok;
    }
  } else if (Tok->startsSequence(tok::kw_const, tok::identifier)) {
    FormatToken *Next = Tok->Next;
    // The case  `const Foo` -> `Foo const`
    // The case  `const Foo *` -> `Foo const *`
    // The case  `const Foo &` -> `Foo const &`
    // The case  `const Foo &&` -> `Foo const &&`
    // The case  `const std::Foo &&` -> `std::Foo const &&`
    // The case  `const std::Foo<T> &&` -> `std::Foo<T> const &&`
    while (Next && Next->isOneOf(tok::identifier, tok::coloncolon)) {
      Next = Next->Next;
    }
    if (Next && Next->is(TT_TemplateOpener)) {
      Next = Next->MatchingParen;
      // Move to the end of any template class members e.g.
      // `Foo<int>::iterator`.
      if (Next && Next->startsSequence(TT_TemplateCloser, tok::coloncolon,
                                       tok::identifier)) {
        Next = Next->Next->Next;
        return Tok;
      }
      Next = Next->Next;
    }
    if (Next && Next->isOneOf(tok::star, tok::amp, tok::ampamp) &&
        !Tok->Next->isOneOf(Keywords.kw_override, Keywords.kw_final)) {
      if (Next->Previous && !Next->Previous->is(tok::kw_const)) {
        insertConstAfter(SourceMgr, Fixes, Next->Previous);
        removeToken(SourceMgr, Fixes, Const);
      }
      return Next;
    }
  }

  return Tok;
}

static FormatToken *analyzeWest(const SourceManager &SourceMgr,
                                const AdditionalKeywords &Keywords,
                                tooling::Replacements &Fixes,
                                FormatToken *Tok) {
  // if Tok is an identifier and possibly a macro then don't convert
  if (isPossibleMacro(Tok)) {
    return Tok;
  }

  FormatToken *Qualifier = Tok;
  FormatToken *LastQualifier = Qualifier;
  while (Qualifier && isCVQualifierOrType(Qualifier)) {
    LastQualifier = Qualifier;
    Qualifier = Qualifier->Next;
    if (Qualifier && Qualifier->is(tok::kw_const)) {
      break;
    }
  }
  if (LastQualifier && Qualifier != LastQualifier &&
      Qualifier->is(tok::kw_const)) {
    rotateTokens(SourceMgr, Fixes, Tok, Qualifier, /*Left=*/true);
    Tok = Qualifier->Next;
  } else if (Tok->startsSequence(tok::identifier, tok::kw_const)) {
    if (Tok->Next->Next && Tok->Next->Next->isOneOf(tok::identifier, tok::star,
                                                    tok::amp, tok::ampamp)) {
      // Don't swap `::iterator const` to `::const iterator`.
      if (!Tok->Previous ||
          (Tok->Previous && !Tok->Previous->is(tok::coloncolon))) {
        rotateTokens(SourceMgr, Fixes, Tok, Tok->Next, /*Left=*/true);
      }
    }
  }
  if (Tok->is(TT_TemplateOpener) && Tok->Next &&
      (Tok->Next->is(tok::identifier) || Tok->Next->isSimpleTypeSpecifier()) &&
      Tok->Next->Next && Tok->Next->Next->is(tok::kw_const)) {
    rotateTokens(SourceMgr, Fixes, Tok->Next, Tok->Next->Next, /*Left=*/true);
  }
  if (Tok->startsSequence(tok::identifier) && Tok->Next) {
    if (Tok->Previous &&
        Tok->Previous->isOneOf(tok::star, tok::ampamp, tok::amp)) {
      return Tok;
    }
    FormatToken *Next = Tok->Next;
    // The case  `std::Foo<T> const` -> `const std::Foo<T> &&`
    while (Next && Next->isOneOf(tok::identifier, tok::coloncolon)) {
      Next = Next->Next;
    }
    if (Next && Next->Previous &&
        Next->Previous->startsSequence(tok::identifier, TT_TemplateOpener)) {
      // Read from to the end of the TemplateOpener to
      // TemplateCloser const ArrayRef<int> a; const ArrayRef<int> &a;
      Next = Next->MatchingParen->Next;

      // Move to the end of any template class members e.g.
      // `Foo<int>::iterator`.
      if (Next && Next->startsSequence(tok::coloncolon, tok::identifier)) {
        Next = Next->Next->Next;
      }
      if (Next && Next->is(tok::kw_const)) {
        // Remove the const.
        removeToken(SourceMgr, Fixes, Next);
        insertConstBefore(SourceMgr, Fixes, Tok);
        return Next;
      }
    }
    if (Next && Next->Next &&
        Next->Next->isOneOf(tok::amp, tok::ampamp, tok::star)) {
      if (Next->is(tok::kw_const)) {
        // Remove the const.
        removeToken(SourceMgr, Fixes, Next);
        insertConstBefore(SourceMgr, Fixes, Tok);
        return Next;
      }
    }
  }
  return Tok;
}

EastWestConstFixer::EastWestConstFixer(const Environment &Env,
                                       const FormatStyle &Style)
    : TokenAnalyzer(Env, Style) {}

std::pair<tooling::Replacements, unsigned>
EastWestConstFixer::analyze(TokenAnnotator &Annotator,
                            SmallVectorImpl<AnnotatedLine *> &AnnotatedLines,
                            FormatTokenLexer &Tokens) {
  tooling::Replacements Fixes;
  const AdditionalKeywords &Keywords = Tokens.getKeywords();
  const SourceManager &SourceMgr = Env.getSourceManager();
  AffectedRangeMgr.computeAffectedLines(AnnotatedLines);

  for (size_t I = 0, E = AnnotatedLines.size(); I != E; ++I) {
    FormatToken *First = AnnotatedLines[I]->First;
    const auto *Last = AnnotatedLines[I]->Last;

    for (auto *Tok = First; Tok && Tok != Last && Tok->Next; Tok = Tok->Next) {
      if (Tok->is(tok::comment)) {
        continue;
      }
      if (Style.ConstPlacement == FormatStyle::CS_East) {
        Tok = analyzeEast(SourceMgr, Keywords, Fixes, Tok);
      } else if (Style.ConstPlacement == FormatStyle::CS_West) {
        Tok = analyzeWest(SourceMgr, Keywords, Fixes, Tok);
      }
    }
  }
  return {Fixes, 0};
}
} // namespace format
} // namespace clang
