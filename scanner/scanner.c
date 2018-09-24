/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>

#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"


extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

/***************************************************************/


void skipBlank() {
  readChar();
}

void skipComment() {
	while(1){
		readChar();
		while(charCodes[currentChar] == CHAR_TIMES){
			readChar();
			if(charCodes[currentChar] == CHAR_RPAR){
				readChar();
				return;
			}
			else if( currentChar == EOF){
				error(ERR_INVALIDSYMBOL, lineNo, colNo);
				return;
			}
		}
		
		if(currentChar == EOF){
				error(ERR_INVALIDSYMBOL, lineNo, colNo);
				return;
			}
	}
}

Token* readBasicChar(TokenType tokenType)
{
	int ln, cn;
	ln = lineNo;
	cn = colNo;
	readChar();
	return makeToken(tokenType, ln, cn);
}

Token* readIdentKeyword(void) {
	Token *token = (Token*)malloc(sizeof(Token));
	int i = 0;
	token->string[i] = currentChar;
	i++;
	token->lineNo = lineNo;
	token->colNo = colNo;
	token->tokenType = TK_IDENT;

	while(99){
		readChar();
		if(i >= 15){
				token->tokenType = TK_NONE;
				error(ERR_IDENTTOOLONG, token->lineNo, token->colNo);
				return makeToken(TK_NONE, lineNo, colNo);
		}
		switch(charCodes[currentChar]){
			case CHAR_DIGIT:
			case CHAR_LETTER:
			token->string[i] = currentChar;
			i++;
			break;

			default:
			token->tokenType = checkKeyword(token->string);
			token->tokenType = token->tokenType == TK_NONE ? TK_IDENT : token->tokenType;
			return token;
		}
	}
}

Token* readNumber(void) {
	Token *token = (Token*)malloc(sizeof(Token));
	int i = 0;
	token->string[i] = currentChar;
	i++;
	token->lineNo = lineNo;
	token->colNo = colNo;
	token->tokenType = TK_NUMBER;

	while(1){
		readChar();
		if(i > 9){
				token->tokenType = TK_NONE;
				error(ERR_NUMBERTOOLONG, token->lineNo, token->colNo);
				return token;
		}
		switch(charCodes[currentChar]){
			case CHAR_DIGIT:
			token->string[token->value] = currentChar;
			i++;
			break;

			default:
			token->value = atoi(token->string);
			return token;
		}
	}
}

Token* readConstChar(void) {
  Token *token = (Token*)malloc(sizeof(Token));
  readChar();
  token->lineNo = lineNo;
  token->colNo = colNo;
  token->tokenType = TK_CHAR;
  if(charCodes[currentChar] != CHAR_SINGLEQUOTE){
  	 token->string[0] = currentChar;
  }
  else{
  	return token;
  }
  readChar();
  if(charCodes[currentChar] != CHAR_SINGLEQUOTE){
  	token->tokenType = TK_NONE;
  	error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
  }
  readChar();
  return token;
}

Token* readPeriod(){
	int ln, cn;
	ln = lineNo;
	cn = colNo;

	readChar();

	if(charCodes[currentChar] == CHAR_RPAR){
		readChar();
		return makeToken(SB_RSEL, ln, cn);
	}

	return makeToken(SB_PERIOD, ln, cn);
	
}

Token* readGT(void){
	int ln, cn;
	ln = lineNo;
	cn = colNo;

	readChar();

	if(charCodes[currentChar] == CHAR_EQ){
		readChar();
		return makeToken(SB_GE, ln, cn);
	}

	return makeToken(SB_GT, ln, cn);
}

Token* readLT(void){
	int ln, cn;
	ln = lineNo;
	cn = colNo;

	readChar();

	if(charCodes[currentChar] == CHAR_EQ){
		readChar();
		return makeToken(SB_LE, ln, cn);
	}

	return makeToken(SB_LT, ln, cn);
}

Token* readColon(void){
	int ln, cn;
	ln = lineNo;
	cn = colNo;

	readChar();

	if(charCodes[currentChar] == CHAR_EQ){
		readChar();
		return makeToken(SB_ASSIGN, ln, cn);
	}

	return makeToken(SB_COLON, ln, cn);
}

Token* readExclaimation(void){
	int ln, cn;
	ln = lineNo;
	cn = colNo;

	readChar();

	if(charCodes[currentChar] == CHAR_EQ){
		readChar();
		return makeToken(SB_NEQ, ln, cn);
	}

	error(ERR_INVALIDSYMBOL, ln, cn);
	return makeToken(TK_NONE, ln, cn);
}


Token* getToken(void) {
  Token *token;
  int ln, cn;

  if (currentChar == EOF) 
    return makeToken(TK_EOF, lineNo, colNo);

  switch (charCodes[currentChar]) {

  case CHAR_SPACE: skipBlank(); return getToken();

  case CHAR_LETTER: return readIdentKeyword();

  case CHAR_DIGIT: return readNumber();

  case CHAR_PLUS: 
    return readBasicChar(SB_PLUS);

    case CHAR_MINUS:
	return readBasicChar(SB_MINUS);

	case CHAR_SLASH:
	return readBasicChar(SB_SLASH);

	case CHAR_TIMES:
	return readBasicChar(SB_TIMES);

	case CHAR_EQ:
	return readBasicChar(SB_EQ);

	case CHAR_SEMICOLON:
	return readBasicChar(SB_SEMICOLON);

	case CHAR_COMMA:
	return readBasicChar(SB_COMMA);

	case CHAR_PERIOD: 
	return readPeriod();

	case CHAR_GT:
	return readGT();

	case CHAR_LT:
	return readLT();

	case CHAR_COLON: 
	return readColon();

	case CHAR_EXCLAIMATION:
	return readExclaimation();



	case CHAR_LPAR:

	ln = lineNo;
	cn = colNo;

	readChar();

	if(charCodes[currentChar] == CHAR_PERIOD){
		readChar();
		return makeToken(SB_LSEL, ln, cn);
	}
	else if(charCodes[currentChar] == CHAR_TIMES){
		skipComment();
		return getToken();
	}
	else{
		return makeToken(SB_LPAR, ln, cn);
	}

	case CHAR_RPAR:
	ln = lineNo;
	cn = colNo;
	readChar();
	return makeToken(SB_RPAR, ln, cn);

  case CHAR_SINGLEQUOTE: return readConstChar();

  default:
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALIDSYMBOL, lineNo, colNo);
    readChar(); 
    return token;
  }
}


/******************************************************************/

void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;

  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;
  }
}

int scan(char *fileName) {
  Token *token;

  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  token = getToken();
  while (token->tokenType != TK_EOF) {
    printToken(token);
    free(token);
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("scanner: no input file.\n");
    return -1;
  }

  if (scan(argv[1]) == IO_ERROR) {
    printf("Can\'t read input file!\n");
    return -1;
  }
    
  return 0;
}



