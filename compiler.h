#ifndef clox_compiler_h
#define clox_compiler_h
#include "chunk.h"

typedef void (*Parsefn)();
bool compile(const char* source, Chunk* chunk);


typedef enum {
  PREC_NONE,
  PREC_ASSIGNMENT,  // =
  PREC_OR,          // or
  PREC_AND,         // and
  PREC_EQUALITY,    // == !=
  PREC_COMPARISON,  // < > <= >=
  PREC_TERM,        // + -
  PREC_FACTOR,      // * /
  PREC_UNARY,       // ! -
  PREC_CALL,        // . ()
  PREC_PRIMARY
} Precedence;

typedef struct {
    Parsefn prefixfn;
    Parsefn infixfn;
    Precedence precedence;
} ParseRule;

#endif
