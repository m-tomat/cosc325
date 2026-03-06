/* front.c - a lexical analyzer for Tiny Basic */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int nextToken;
FILE *in_fp;

void addChar();
void getChar();
void getNonBlank();
int lex();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define QUOTE 2
#define UNKNOWN 99

/* Token codes */
#define STR_LIT 8
#define INT_LIT 10
#define IDENT 11
#define LT_OP 18
#define GT_OP 19
#define EQUALS_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define LTE_OP 27
#define GTE_OP 28
#define NEQ_OP 29
#define PRINT 30
#define IF 31
#define THEN 32
#define GOTO 33
#define GOSUB 34
#define INPUT 35
#define LET 36
#define RETURN_KW 37
#define CLEAR 38
#define LIST 39
#define COMMA 40
#define RUN 41
#define END_KW 42

int main() {
  if ((in_fp = fopen("front.in", "r")) == NULL)
    printf("ERROR - cannot open front.in\n");
  else {
    getChar();
    do { lex(); } while (nextToken != EOF);
  }
}

/* lookup operators, handles multi-char relops */
int lookup(char ch) {
  switch (ch) {
  case '(': addChar(); nextToken = LEFT_PAREN; getChar(); break;
  case ')': addChar(); nextToken = RIGHT_PAREN; getChar(); break;
  case '+': addChar(); nextToken = ADD_OP; getChar(); break;
  case '-': addChar(); nextToken = SUB_OP; getChar(); break;
  case '*': addChar(); nextToken = MULT_OP; getChar(); break;
  case '/': addChar(); nextToken = DIV_OP; getChar(); break;
  case '=': addChar(); nextToken = EQUALS_OP; getChar(); break;
  case ',': addChar(); nextToken = COMMA; getChar(); break;
  case '<':
    addChar();
    getChar();
    if (nextChar == '=')      { addChar(); nextToken = LTE_OP; getChar(); }
    else if (nextChar == '>') { addChar(); nextToken = NEQ_OP; getChar(); }
    else                      { nextToken = LT_OP; }
    break;
  case '>':
    addChar();
    getChar();
    if (nextChar == '=') { addChar(); nextToken = GTE_OP; getChar(); }
    else                 { nextToken = GT_OP; }
    break;
  default:
    addChar();
    nextToken = -1;
    printf("ERROR - illegal symbol: %c\n", ch);
    getChar();
    break;
  }
  return nextToken;
}

void addChar() {
  if (lexLen <= 98) {
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = 0;
  } else
    printf("Error - lexeme is too long\n");
}

void getChar() {
  int c = getc(in_fp);
  if (c == EOF) {
    charClass = EOF;
    nextChar = 0;
  } else {
    nextChar = (char)c;
    if (isalpha((unsigned char)nextChar))
      charClass = LETTER;
    else if (isdigit((unsigned char)nextChar))
      charClass = DIGIT;
    else if (nextChar == '"')
      charClass = QUOTE;
    else
      charClass = UNKNOWN;
  }
}

void getNonBlank() {
  while (isspace(nextChar) && charClass != EOF)
    getChar();
}

/* returns keyword token or IDENT */
int keywordLookup() {
  if (strcmp(lexeme, "PRINT") == 0 || strcmp(lexeme, "PR") == 0) return PRINT;
  if (strcmp(lexeme, "IF") == 0)     return IF;
  if (strcmp(lexeme, "THEN") == 0)   return THEN;
  if (strcmp(lexeme, "GOTO") == 0)   return GOTO;
  if (strcmp(lexeme, "GOSUB") == 0)  return GOSUB;
  if (strcmp(lexeme, "INPUT") == 0)  return INPUT;
  if (strcmp(lexeme, "LET") == 0)    return LET;
  if (strcmp(lexeme, "RETURN") == 0) return RETURN_KW;
  if (strcmp(lexeme, "CLEAR") == 0)  return CLEAR;
  if (strcmp(lexeme, "LIST") == 0)   return LIST;
  if (strcmp(lexeme, "RUN") == 0)    return RUN;
  if (strcmp(lexeme, "END") == 0)    return END_KW;
  return IDENT;
}

int lex() {
  lexLen = 0;
  getNonBlank();
  switch (charClass) {
  case LETTER:
    addChar();
    getChar();
    while (charClass == LETTER || charClass == DIGIT) {
      addChar();
      getChar();
    }
    nextToken = keywordLookup();
    break;
  case DIGIT:
    addChar();
    getChar();
    while (charClass == DIGIT) {
      addChar();
      getChar();
    }
    nextToken = INT_LIT;
    break;
  case QUOTE:
    addChar();
    getChar();
    while (charClass != QUOTE && charClass != EOF) {
      addChar();
      getChar();
    }
    if (charClass == QUOTE) {
      addChar();
      getChar();
    } else {
      printf("ERROR - unterminated string\n");
    }
    nextToken = STR_LIT;
    break;
  case UNKNOWN:
    lookup(nextChar);
    break;
  case EOF:
    nextToken = EOF;
    lexeme[0] = 'E'; lexeme[1] = 'O'; lexeme[2] = 'F'; lexeme[3] = 0;
    break;
  }
  printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
  return nextToken;
}
