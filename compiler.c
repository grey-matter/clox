#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"
#include "chunk.h"


typedef struct {
    Token previous;
    Token current;
    Chunk* currentChunk;
    bool hadError;
    bool panicMode;
} Parser;

static ParseRule getRule(TokenType);
static void parsePrecedence(Precedence);
static void expression();
static void consume(TokenType type, const char*);

Parser parser;
Chunk* chunk;

static Chunk* currentChunk() {
    return parser.currentChunk;
}

static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static int makeConstant(Value value) {
    int index = addConstant(currentChunk(), value);
    return index;
}

static void emitConstant(Value value) {
    emitBytes(OP_CONSTANT, makeConstant(value));
}

static void number() {
    double value = strtod(parser.previous.start, NULL);
    emitConstant(value);
}

static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expected right parenthesis.");
}

static void unary() {
    TokenType operatorType = parser.previous.tokenType;
    parsePrecedence(PREC_UNARY);

    switch (operatorType) {
        case TOKEN_MINUS:
            emitByte(OP_NEGATE);
            break;
        
        default:
            break;
    }
}

static void binary() {
    TokenType operatorType = parser.previous.tokenType;
    ParseRule rule = getRule(operatorType);
    parsePrecedence(rule.precedence + 1);

    switch (operatorType) {
        case TOKEN_PLUS:
            emitByte(OP_ADD);
            break;
        case TOKEN_MINUS:
            emitByte(OP_SUBTRACT);
            break;
        case TOKEN_STAR:
            emitByte(OP_MULTIPLY);
            break;
        case TOKEN_SLASH:
            emitByte(OP_DIVIDE);
            break;
        default:
            break;
    }
}

ParseRule rules[] = {
  [TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE},
  [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   PREC_NONE}, 
  [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
  [TOKEN_PLUS]          = {NULL,     binary, PREC_TERM},
  [TOKEN_SEMICOLON]     = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
  [TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR},
  [TOKEN_BANG]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_BANG_EQUAL]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EQUAL_EQUAL]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_GREATER]       = {NULL,     NULL,   PREC_NONE},
  [TOKEN_GREATER_EQUAL] = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LESS]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LESS_EQUAL]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_IDENTIFIER]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_STRING]        = {NULL,     NULL,   PREC_NONE},
  [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
  [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FALSE]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_NIL]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_TRUE]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};

static ParseRule getRule(TokenType type) {
    return rules[type];
}

static void errorAt(Token token, const char* msg) {
    if (parser.panicMode) {
        return;
    }
    parser.panicMode = parser.hadError = true;
    fprintf(stderr, "[line %d] Error", token.line);
    if (token.tokenType == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token.tokenType != TOKEN_ERROR) {
        fprintf(stderr, " at %.*s", token.length, token.start);
    }
    fprintf(stderr, ": %s\n", msg);
}

static void errorAtCurrent(const char* errorMsg) {
    errorAt(parser.current, errorMsg);
}



static void advance() {
    parser.previous = parser.current;

    while (true) {
        parser.current = scanToken();
        if (parser.current.tokenType != TOKEN_ERROR) {
            return;
        }
        errorAtCurrent(parser.current.start);
    }
}

static void parsePrecedence(Precedence precedence) {
    advance();
    Parsefn prefixRule = getRule(parser.previous.tokenType).prefixfn;
    if (prefixRule == NULL) {
        errorAtCurrent("Expect expression.");
        return;
    }

    prefixRule();
    while (precedence <= getRule(parser.current.tokenType).precedence) {
        advance();
        Parsefn infixRule = getRule(parser.previous.tokenType).infixfn;
        infixRule();
    }
}

static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}

static void consume(TokenType type, const char* errorMsg) {
    if (parser.current.tokenType == type) {
        advance();
        return;
    }
    errorAtCurrent(errorMsg);
}

static void endCompilation() {
    emitByte(OP_RETURN);
}

bool compile(const char* source, Chunk* chunk) {
    initScanner(source);
    parser.hadError = parser.panicMode = false;
    parser.currentChunk = chunk;

    advance();
    expression();
    consume(TOKEN_EOF, "Expected end of expression.");
    
    endCompilation();
    return !parser.hadError;
}



