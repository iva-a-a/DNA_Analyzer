#pragma once

enum class CommandType {
  ExactSearch,
  AlignScore,
  Align,
  RegexMatch,
  KSimilarity,
  MinWindow,
  Unknown
};