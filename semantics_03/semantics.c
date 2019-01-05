/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

extern SymTab* symtab;
extern Token* currentToken;

Object* lookupObject(char *name) {
  // TODO
  Scope *scope = symtab->currentScope;
  Object *obj = NULL;
  while(scope != NULL){
    obj = findObject(scope->objList, name);
    if(obj != NULL)
      return obj;
    scope = scope->outer;
  }
  return findObject(symtab->globalObjectList, name);
}

void checkFreshIdent(char *name) {
  // TODO
  if (findObject(symtab->currentScope->objList, name) != NULL)
    error(ERR_DUPLICATE_IDENT, currentToken->lineNo, currentToken->colNo);
}

Object* checkDeclaredIdent(char* name) {
  // TODO
  Object *obj = lookupObject(name);

  if(obj == NULL)
    error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);
  return obj;
}

Object* checkDeclaredConstant(char* name) {
  // TODO
  Object *obj = lookupObject(name);

  if (obj == NULL)
    error(ERR_UNDECLARED_CONSTANT,currentToken->lineNo, currentToken->colNo);
  else if (obj->kind != OBJ_CONSTANT)
    error(ERR_INVALID_CONSTANT,currentToken->lineNo, currentToken->colNo);

  return obj;
}

Object* checkDeclaredType(char* name) {
  // TODO
  Object *obj = lookupObject(name);

  if (obj == NULL)
    error(ERR_UNDECLARED_TYPE,currentToken->lineNo, currentToken->colNo);
  else if (obj->kind != OBJ_TYPE)
    error(ERR_INVALID_TYPE,currentToken->lineNo, currentToken->colNo);

  return obj;
}

Object* checkDeclaredVariable(char* name) {
  // TODO
  Object *obj = lookupObject(name);

  if (obj == NULL)
    error(ERR_UNDECLARED_VARIABLE,currentToken->lineNo, currentToken->colNo);
  else if (obj->kind != OBJ_VARIABLE)
    error(ERR_INVALID_VARIABLE,currentToken->lineNo, currentToken->colNo);

  return obj;
}

Object* checkDeclaredFunction(char* name) {
  // TODO
  Object *obj = lookupObject(name);

  if (obj == NULL)
    error(ERR_UNDECLARED_FUNCTION,currentToken->lineNo, currentToken->colNo);
  else if (obj->kind != OBJ_FUNCTION)
    error(ERR_INVALID_FUNCTION,currentToken->lineNo, currentToken->colNo);

  return obj;
}

Object* checkDeclaredProcedure(char* name) {
  // TODO
  Object *obj = lookupObject(name);

  if (obj == NULL)
    error(ERR_UNDECLARED_PROCEDURE,currentToken->lineNo, currentToken->colNo);
  else if (obj->kind != OBJ_PROCEDURE)
    error(ERR_INVALID_PROCEDURE,currentToken->lineNo, currentToken->colNo);

  return obj;
}

Object* checkDeclaredLValueIdent(char* name) {
  // TODO
  Object *obj = lookupObject(name);

  if(obj == NULL)
    error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);
  
  switch(obj->kind){
    case OBJ_VARIABLE:
      if( symtab->currentScope->owner->kind == OBJ_FUNCTION &&
        strcmp(symtab->currentScope->owner->name, currentToken->string) == 0 &&
        obj->varAttrs->type->typeClass == symtab->currentScope->owner->funcAttrs->returnType->typeClass
        ){
          error(ERR_INVALID_LVALUE, currentToken->lineNo, currentToken->colNo);
        }
    case OBJ_PARAMETER:
      break;
    case OBJ_FUNCTION:
      if(obj != symtab->currentScope->owner)
        error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);
      break;
    default:
      error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);
  }

  return obj;
}

