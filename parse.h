//
//  parse.h
//  JSInterpreter
//
//  Created by thomas on 15/12/8.
//  Copyright © 2015年 thomas. All rights reserved.
//

#ifndef parse_h
#define parse_h

#include <stdio.h>


char* filter(char* filename);

int isNum(char c);
int isAlpha(char c);
int isAlnum(char c);
char* readChar(char* p, char c);
char *readDefault(char *p, const char* s);
char *readValname(char*p, char*s);
char* readString(char*p, char*s);
char* readUInt(char* p, int* num);
char* readInt(char *p, int* num);
char* readLeftBrace(char *p, AstNode** pAst);
char* readRightBrace(char *p, AstNode** pAst);
char* readDotDouble(char *p, double *num);
char* readUDouble(char *p, double *pnum);
char* readDouble(char *p, double* pnum);
char* readEmpty(char*p, AstNode**);
char* readSpace(char *p, AstNode**);
char* readFactor(char *p, AstNode** pAst);
char* readT(char *p, AstNode** pAst);
char* readTPlus(char* p, AstNode** pAst);
char* readF(char*p, AstNode** pAst);
char* readExpPlus(char*p, AstNode** pAst);
char* readExp(char *p, AstNode** pAst);
char* readOpMulDiv(char *p, AstNode** pAst);
char* readOpAddMinus(char *p, AstNode** pAst);
char* readFunctionCall(char *p, AstNode** pAst);
char* readExpList(char *p, AstNode** pAst);
char* readComma(char *p, AstNode** pAst);
char* readVariable(char *p, AstNode** pAst);
char* readArray(char *p, AstNode** pAst);
char* readCompare(char *p, AstNode** pAst);
char* readC(char*p, AstNode** pAst);
char* readLogicOperator(char*p, AstNode** pAst);

char* readBlock(char*p, AstNode** pAst);
char* readStmt(char*p, AstNode** pAst);
char* readStmts(char*p, AstNode** pAst);
char* readExpr(char*p, AstNode** pAst);
char* readIfStmt(char*p, AstNode** pAst);
char* readWhileStmt(char*p, AstNode** pAst);
//char* readDoWhileStmt(char*p, AstNode** pAst);
//char* readForStmt(char*p, AstNode** pAst);
char* readFuncDefStmt(char*p, AstNode**pAst);
char* readVarDefStmt(char*p, AstNode**pAst);
char* readBreak(char*p, AstNode **pAst);
char* readContinue(char*p, AstNode **pAst);
//char* readSwitchStmt(char*p, AstNode **pAst);
char* readReturnStmt(char*p, AstNode **pAst);
char* readNewStmt(char*p, AstNode **pAst);
//char* readCaseStmt(char*p, AstNode **pAst);
char* readVarlist(char*p, AstNode **pAst);


readAstDeclare(D);
readChaDeclare(LeftBrace, '(');
readChaDeclare(RightBrace, ')');
readChaDeclare(Comma, ',');
readChaDeclare(Dot, '.');
readChaDeclare(LeftBracket, '[');
readChaDeclare(RightBracket, ']');
readChaDeclare(LeftCurlyBrace, '{');
readChaDeclare(RightCurlyBrace, '}');
readChaDeclare(Semicolon, ';');
readChaDeclare(Colon, ':');
readChaDeclare(Equ, '=');

readAstDeclare(DPlus);
readAstDeclare(CPlus);

readStringDeclare(Func, "function");
readStringDeclare(For, "for");
readStringDeclare(If, "if");
readStringDeclare(While, "while");
readStringDeclare(Case, "case");
readStringDeclare(Do, "do");
readStringDeclare(Break, "break");
readStringDeclare(Continue, "continue");
readStringDeclare(Return, "return");
readStringDeclare(New, "new");
readStringDeclare(Default, "default");
readStringDeclare(Var, "var");



#endif /* parse_h */
