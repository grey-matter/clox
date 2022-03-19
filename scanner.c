#include <stdio.h>    
#include <string.h>   

#include "common.h"   
#include "scanner.h"


// TODO
// String interpolation


typedef struct {
  const char* start;
  const char* current; // start points to the beginning of current lexeme, current to the current char being scanned
  int line;
} Scanner;

Scanner scanner;

Token makeToken(TokenType type) {
    Token ret;
    ret.line = scanner.line;
    ret.tokenType = type;
    ret.length = (int)(scanner.current - scanner.start);
    ret.start = scanner.start;
    return ret;
}

Token errorToken(const char* msg) {
    Token ret;
    ret.start = msg;
    ret.length = (int)strlen(msg);
    ret.tokenType = TOKEN_ERROR;
    ret.line = scanner.line;
    return ret;
}

bool isAtEnd() {
    return *scanner.start == '\0';
}


char advance() {
    return *scanner.current++;
}

bool match(char expected) {
    if (isAtEnd() || *scanner.current != expected) {
        return false;
    }
    scanner.current++;
    return true;
}

char peek() {
    return *scanner.current;
}
char peekNext() {
    if (isAtEnd()) {
        return '\0';
    }
    return *(scanner.current + 1);
}

void skipWhiteSpace() {
    while (true) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    while (!isAtEnd() && peek() != '\n')
                        advance();
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}


Token string() {
    while (!isAtEnd() && peek() != '"') {
        if (peek() == '\n') {
            scanner.line++;
        }
        advance();
    }
    if (isAtEnd()) {
        return errorToken("Expected '\"'");
    }
    advance();
    return makeToken(TOKEN_STRING);
}


bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

Token number() {
    while (isDigit(peek())) {
        advance();
    }
    
    if (peek() == '.' && isDigit(peekNext())) {
        advance();
        while (isDigit(peek())) {
            advance();
        }
    }

    return makeToken(TOKEN_NUMBER);
}

bool isAlpha(char c) {
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

TokenType checkKeyword(int start, int length, const char* rest, TokenType type) {
    if (length + start == (int)(scanner.current - scanner.start) && !memcmp(scanner.start + start, rest, length)) {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

TokenType identifierType() {
    switch (scanner.start[0]) {
        case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
        case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
        case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
        case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
        case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
        case 'f':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
                }
            }
        break;
        case 't':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
                    case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
        break;
    }
    return TOKEN_IDENTIFIER;
}

Token identifier() {
    while (isAlpha(peek()) || isDigit(peek()) || peek() == '_') {
        advance();
    }
    return makeToken(identifierType());
}

Token scanToken() {
    skipWhiteSpace();
    scanner.start = scanner.current;

    if (isAtEnd()) {
        return makeToken(TOKEN_EOF);
    }

    char c = advance();
    if (isAlpha(c) || c == '_') {
        return identifier();
    }

    if (isDigit(c)) {
        return number();
    }

    switch (c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);
        case '!':
            return makeToken(
                match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return makeToken(
                match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return makeToken(
                match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return makeToken(
                match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    
        case '"':
            return string();
    }
    return errorToken("Unexpected character.");

}

void initScanner(const char* source) {
    scanner.start = scanner.current = source;
    scanner.line = 1;
}