/* front.c - a lexical analyzer for Tiny Basic
   Based on the lexer from Sebesta (page 171),
   extended to handle the full Tiny Basic grammar. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Global declarations */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp;

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();
int lookup(char ch);
int keywordLookup();
const char *tokenName(int tok);

/* Character classes */
#define LETTER  0
#define DIGIT   1
#define QUOTE   2
#define NEWLINE 3
#define UNKNOWN 99

/* Token codes */
#define STR_LIT     8
#define INT_LIT     10
#define IDENT       11

/* Relational operators */
#define LT_OP       15
#define GT_OP       16
#define LTEQ_OP     17
#define GTEQ_OP     18
#define NEQ_OP      19
#define EQUALS_OP   20

/* Arithmetic operators */
#define ADD_OP      21
#define SUB_OP      22
#define MULT_OP     23
#define DIV_OP      24

/* Delimiters */
#define LEFT_PAREN  25
#define RIGHT_PAREN 26
#define COMMA       27
#define CR          28

/* Keywords */
#define KEY_PRINT   30
#define KEY_IF      31
#define KEY_THEN    32
#define KEY_GOTO    33
#define KEY_GOSUB   34
#define KEY_INPUT   35
#define KEY_LET     36
#define KEY_RETURN  37
#define KEY_CLEAR   38
#define KEY_LIST    39
#define KEY_RUN     40
#define KEY_END     41

/******************************************************/
/* tokenName - returns a descriptive name for a token */
const char *tokenName(int tok) {
    switch (tok) {
        case STR_LIT:     return "STR_LIT";
        case INT_LIT:     return "INT_LIT";
        case IDENT:       return "IDENT";
        case LT_OP:       return "LT_OP";
        case GT_OP:       return "GT_OP";
        case LTEQ_OP:     return "LTEQ_OP";
        case GTEQ_OP:     return "GTEQ_OP";
        case NEQ_OP:      return "NEQ_OP";
        case EQUALS_OP:   return "EQUALS_OP";
        case ADD_OP:      return "ADD_OP";
        case SUB_OP:      return "SUB_OP";
        case MULT_OP:     return "MULT_OP";
        case DIV_OP:      return "DIV_OP";
        case LEFT_PAREN:  return "LEFT_PAREN";
        case RIGHT_PAREN: return "RIGHT_PAREN";
        case COMMA:       return "COMMA";
        case CR:          return "CR";
        case KEY_PRINT:   return "KEY_PRINT";
        case KEY_IF:      return "KEY_IF";
        case KEY_THEN:    return "KEY_THEN";
        case KEY_GOTO:    return "KEY_GOTO";
        case KEY_GOSUB:   return "KEY_GOSUB";
        case KEY_INPUT:   return "KEY_INPUT";
        case KEY_LET:     return "KEY_LET";
        case KEY_RETURN:  return "KEY_RETURN";
        case KEY_CLEAR:   return "KEY_CLEAR";
        case KEY_LIST:    return "KEY_LIST";
        case KEY_RUN:     return "KEY_RUN";
        case KEY_END:     return "KEY_END";
        case EOF:         return "EOF";
        default:          return "UNKNOWN";
    }
}

/******************************************************/
/* main driver */
int main()
{
  if ((in_fp = fopen("front.in", "r")) == NULL) {
    printf("ERROR - cannot open front.in\n");
    return 1;
  }
  getChar();
  do {
    lex();
  } while (nextToken != EOF);
  fclose(in_fp);
  return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses
            and return the token */

int lookup(char ch)
{
  switch (ch)
  {
  case '(':
    addChar();
    getChar();
    nextToken = LEFT_PAREN;
    break;

  case ')':
    addChar();
    getChar();
    nextToken = RIGHT_PAREN;
    break;

  case '+':
    addChar();
    getChar();
    nextToken = ADD_OP;
    break;

  case '-':
    addChar();
    getChar();
    nextToken = SUB_OP;
    break;

  case '*':
    addChar();
    getChar();
    nextToken = MULT_OP;
    break;

  case '/':
    addChar();
    getChar();
    nextToken = DIV_OP;
    break;

  case '=':
    addChar();
    getChar();
    nextToken = EQUALS_OP;
    break;

  case '<':
    addChar();
    getChar();
    /* Check for <= and <> */
    if (nextChar == '=') {
      addChar();
      getChar();
      nextToken = LTEQ_OP;
    } else if (nextChar == '>') {
      addChar();
      getChar();
      nextToken = NEQ_OP;
    } else {
      nextToken = LT_OP;
    }
    break;

  case '>':
    addChar();
    getChar();
    /* Check for >= and >< */
    if (nextChar == '=') {
      addChar();
      getChar();
      nextToken = GTEQ_OP;
    } else if (nextChar == '<') {
      addChar();
      getChar();
      nextToken = NEQ_OP;
    } else {
      nextToken = GT_OP;
    }
    break;

  case ',':
    addChar();
    getChar();
    nextToken = COMMA;
    break;

  default:
    addChar();
    printf("ERROR - illegal symbol: '%c' (ASCII %d) in lexeme: %s\n",
           ch, ch, lexeme);
    getChar();
    nextToken = EOF;
    break;
  }
  return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar()
{
  if (lexLen <= 98)
  {
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = 0;
  }
  else
    printf("Error - lexeme is too long \n");
}
/*****************************************************/
/* getChar - a function to get the next character of
             input and determine its character class */
void getChar()
{
  int c = getc(in_fp);
  if (c == EOF)
  {
    charClass = EOF;
    nextChar = 0;
  }
  else
  {
    nextChar = (char)c;
    if (isalpha((unsigned char)nextChar))
      charClass = LETTER;
    else if (isdigit((unsigned char)nextChar))
      charClass = DIGIT;
    else if (nextChar == '"')
      charClass = QUOTE;
    else if (nextChar == '\n')
      charClass = NEWLINE;
    else
      charClass = UNKNOWN;
  }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
                 returns a non-whitespace character */
/* getNonBlank - skip spaces, tabs, and \r
                 but NOT newlines (which are CR tokens) */
void getNonBlank()
{
  while (nextChar == ' ' || nextChar == '\t' || nextChar == '\r')
    getChar();
}

/* keywordLookup - examines current lexeme and returns
                   specific keyword token or IDENT */
int keywordLookup() {
  if (strcmp(lexeme, "PRINT") == 0 || strcmp(lexeme, "PR") == 0)
    return KEY_PRINT;
  else if (strcmp(lexeme, "IF") == 0)
    return KEY_IF;
  else if (strcmp(lexeme, "THEN") == 0)
    return KEY_THEN;
  else if (strcmp(lexeme, "GOTO") == 0)
    return KEY_GOTO;
  else if (strcmp(lexeme, "GOSUB") == 0)
    return KEY_GOSUB;
  else if (strcmp(lexeme, "INPUT") == 0)
    return KEY_INPUT;
  else if (strcmp(lexeme, "LET") == 0)
    return KEY_LET;
  else if (strcmp(lexeme, "RETURN") == 0)
    return KEY_RETURN;
  else if (strcmp(lexeme, "CLEAR") == 0)
    return KEY_CLEAR;
  else if (strcmp(lexeme, "LIST") == 0)
    return KEY_LIST;
  else if (strcmp(lexeme, "RUN") == 0)
    return KEY_RUN;
  else if (strcmp(lexeme, "END") == 0)
    return KEY_END;
  else
    return IDENT;
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
         expressions 
   // depends on charClass and nextChar already being set by the caller
         */
int lex()
{
  lexLen = 0;
  getNonBlank();
  switch (charClass)
  {
    /* Parse identifiers and keywords */
  case LETTER:
    addChar();
    getChar();
    while (charClass == LETTER || charClass == DIGIT)
    {
      addChar();
      getChar();
    }
    nextToken = keywordLookup();
    break;

    /* Parse integer literals */
  case DIGIT:
    addChar();
    getChar();
    while (charClass == DIGIT)
    {
      addChar();
      getChar();
    }
    nextToken = INT_LIT;
    break;

    /* Parse string literals */
  case QUOTE:
    addChar();
    getChar();
    while (charClass != QUOTE && charClass != EOF && charClass != NEWLINE)
    {
      addChar();
      getChar();
    }
    if (charClass == QUOTE) {
      addChar();
      getChar();
      nextToken = STR_LIT;
    } else {
      printf("ERROR - unterminated string literal: %s\n", lexeme);
      nextToken = EOF;
    }
    break;

    /* Handle newline as CR token */
  case NEWLINE:
    lexeme[0] = '\\';
    lexeme[1] = 'n';
    lexeme[2] = 0;
    lexLen = 2;
    nextToken = CR;
    getChar();
    break;

    /* Operators, delimiters, and unknown symbols */
  case UNKNOWN:
    lookup(nextChar);
    break;

    /* EOF */
  case EOF:
    nextToken = EOF;
    lexeme[0] = 'E';
    lexeme[1] = 'O';
    lexeme[2] = 'F';
    lexeme[3] = 0;
    break;
  } /* End of switch */
  printf("Next token is: %3d  %-12s  Next lexeme is %s\n",
         nextToken, tokenName(nextToken), lexeme);
  return nextToken;
} /* End of function lex */
