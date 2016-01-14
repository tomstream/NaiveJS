#define _CRT_SECURE_NO_WARNINGS
//
//  parse.c
//  JSInterpreter
//
//  Created by thomas on 15/12/8.
//  Copyright © 2015年 thomas. All rights reserved.
//

#include "node.h"
#include "str.h"
#include "type.h"
#include "run.h"
#include "parse.h"
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <string.h>


static char s[4000];
static char buffer[3000];

char* filter(char *filename){
	FILE *fp = fopen(filename, "r");
	if (!fp){
		printf("This file does not exist.");
		return 0;
	}
	int m = 0;
	while (!feof(fp)){
		s[m++] = fgetc(fp);
	}
	s[m++] = 0;
	int i = 0;
	int j = 0;
	int state = 0;
	
	while (1){
		if (s[i] == 0)break;
		if (state == 0){
			if (s[i] == '\''){
				state = 1;
				buffer[j++] = s[i];
			}
			else if (s[i] != ' '&&s[i] != '\t'&&s[i] != '\n'&&s[i] != '\r'){
				buffer[j++] = s[i];
			}
		}
		else{
			if (s[i] == '\''){
				buffer[j++] = s[i];
				state = 0;
			}else  buffer[j++] = s[i];
		}
		i++;
	}
	buffer[j++] = 0;
	return buffer;
}



readChaDefine(LeftBrace, '(');
readChaDefine(RightBrace, ')');
readChaDefine(Comma, ',');
readChaDefine(Dot, '.');
readChaDefine(LeftBracket, '[');
readChaDefine(RightBracket, ']');
readChaDefine(Equ, '=');
readChaDefine(LeftCurlyBrace, '{');
readChaDefine(RightCurlyBrace, '}');
readChaDefine(Semicolon, ';');

readStringDefine(Func, "function", AST_NULL);
readStringDefine(For, "for", AST_NULL);
readStringDefine(If, "if", AST_NULL);
readStringDefine(While, "while", AST_NULL);
readStringDefine(Case, "case", AST_NULL);
readStringDefine(Do, "do", AST_NULL);
readStringDefine(Break, "break", AST_NULL);
readStringDefine(Continue, "continue", AST_NULL);
readStringDefine(Return, "return", AST_NULL);
readStringDefine(New, "new", AST_NULL);
readStringDefine(Default, "default", AST_NULL);
readStringDefine(Var, "var", AST_NULL);

AstArray(Block, 3) = { { *readLeftCurlyBrace, *readStmts, *readRightCurlyBrace, 0 }, { 0 } };
AstArray(Stmts, 3) = { { *readStmt, *readStmts, 0 }, { *readEmpty, 0 }, 0 };
AstArray(Stmt, 20) =
{ { *readFuncDefStmt, 0 },
{ *readBreak, 0 },
{ *readContinue, 0 },
{ *readReturnStmt, 0 },
{ *readIfStmt, 0 },
{ *readWhileStmt, 0 },
{ *readExpr, *readSemicolon, 0 },
//{*readDoWhileStmt,0},
//{*readForStmt,0},

{ *readBlock, 0 },
{ *readVarDefStmt, 0 },
//{*readSwitchStmt,0},

{ *readNewStmt, 0 },
{ 0 } };
AstArray(NewStmt, 3) = { { *readStringNew, *readSpace, *readFactor, *readLeftBrace, *readExpList, *readRightBrace, 0 }, { 0 } };
AstArray(ReturnStmt, 3) = { { *readStringReturn, *readExp, *readSemicolon, 0 }, { *readStringReturn, *readSemicolon, 0 }, 0 };
//AstArray(CaseStmt, 4) = { { *readStringCase, *readFactor, *readColon, *readStmts, *readCaseStmt, 0 }, { *readEmpty, 0 }, 0 };
//AstArray(FuncDefStmt, 4) = { {*readStringFunc,*readVarlist,*readLeftCurlyBrace,*readBlock,*readRightCurlyBrace,0},0 };
AstArray(Varlist, 4) = { { *readFactor, *readComma, *readVarlist, 0 }, { *readFactor, *readEmpty, 0 }, { 0 } };
AstArray(VarDefStmt, 4) = { { *readStringVar, *readEqu, *readFactor, 0 }, { *readStringVar, *readEqu, *readNewStmt, 0 }, { 0 } };
AstArray(Break, 2) = { { *readStringBreak, *readSemicolon, 0 }, { 0 } };
AstArray(Continue, 2) = { { *readStringContinue, *readSemicolon, 0 }, { 0 } };
AstArray(IfStmt, 3) = { { *readStringIf, *readLeftBrace, *readExp, *readRightBrace, *readBlock, 0 } };
AstArray(WhileStmt, 3) = { { *readStringWhile, *readLeftBrace, *readExp, *readRightBrace, *readBlock, 0 } };
AstArray(Expr, 4) = { { *readArray, *readEqu, *readExp, 0 }, { *readFactor, *readEqu, *readExp, 0 }, { *readExp, 0 }, { 0 } };


readAstDefine(Block, Block, AST_BLOCK, 3);
readAstDefine(Stmt, Stmt, AST_STMT, 20);
readAstDefine(Stmts, Stmts, AST_STMTS, 3);
readAstDefine(NewStmt, NewStmt, AST_NEW_STMT, 3);
readAstDefine(ReturnStmt, ReturnStmt, AST_RETURN_STMT, 4);
//readAstDefine(FuncDefStmt, FuncDefStmt, AST_FUNC_DEF_STMT, 4);
readAstDefine(Varlist, Varlist, AST_VAR_LIST, 4);
readAstDefine(VarDefStmt, VarDefStmt, AST_VAR_DEF, 4);
readAstDefine(Break, Break, AST_BREAK, 3);
readAstDefine(Continue, Continue, AST_CONTINUE, 3);
readAstDefine(IfStmt, IfStmt, AST_IF_STMT, 3);
readAstDefine(WhileStmt, WhileStmt, AST_WHILE_STMT, 3);
readAstDefine(Expr, Expr, AST_EXPR, 4);


char* readFuncDefStmt(char*p, AstNode** pAst)
{
	AstArrayA(5) = { { *readStringFunc, *readFactor, *readLeftBrace, *readVarlist, *readRightBrace,
		*readBlock, 0 }, { 0 } };
	AstNode* astNodes[5][10];
	int i = 0;
	int j = 0;
	char* ret = p;
	for (i = 0; a[i][0] != 0; i++){
		ret = p;
		for (j = 0; a[i][j] != 0; j++){
			ret = a[i][j](ret, &astNodes[i][j]);
			if (ret == NULL)
				break;
		}
		if (ret != NULL)break;
	}


	if (ret == NULL)return NULL;
	//varname
	AstNode* tmpNode = astNodes[i][1];
	char* name = cp_str(tmpNode->Content);
	if (tmpNode->type != TOKEN_VALNAME)return NULL;
	int m = 0;
	static AstNode* nodes[20];
	char *tmpP = p;
	tmpP = readStringFunc(tmpP, &tmpNode);
	tmpP = readFactor(tmpP, &tmpNode);
	tmpP = readLeftBrace(tmpP, &tmpNode);
	for (m = 0; m < 20; m++){
		tmpP = readFactor(tmpP, &nodes[m]);
		if (tmpP == NULL)break;
		tmpP = readComma(tmpP, &tmpNode);
		if (tmpP == NULL)break;
	}
	int count = m + 1;
	AstNode* expListNode = createType(AstNode);
	expListNode->type = AST_EXPLIST;
	expListNode->num = count;
	expListNode->childNode = createTypeNum(AstNode, m);

	for (int k = 0; k <= m; k++){
		(expListNode->childNode)[k] = *nodes[k];
	}

	//save the function

	//puts("FuncDef");
	*pAst = createType(AstNode);
	(*pAst)->type = AST_FUNC_DEF_STMT;
	(*pAst)->num = 3;
	(*pAst)->Content = NULL;
	(*pAst)->childNode = createTypeNum(AstNode, 3);
	(*pAst)->childNode[0] = *astNodes[i][1];//varname
	(*pAst)->childNode[1] = *expListNode;
	(*pAst)->childNode[2] = *astNodes[i][5];//block
	saveVariable(name, *pAst, 0);
	return ret;
}



char* readLogicOperator(char*p, AstNode** pAst){
	if (strlen(p) < 2)return NULL;
	if (!strncmp(p, "&&", 2)){
		*pAst = createType(AstNode);
		(*pAst)->type = TOKEN_AND;
		return p + 2;
	}
	else if (!strncmp(p, "||", 2)){
		*pAst = createType(AstNode);
		(*pAst)->type = TOKEN_OR;
		return p + 2;
	}
	return NULL;
}

char* readSingleOperator(char*p, AstNode** pAst){
	long length = strlen(p);
	if (length <= 2)return NULL;

	if (p[0] == '+'){
		if (p[1] == '+'){
			*pAst = createType(AstNode);
			(*pAst)->type = TOKEN_PLUSPLUS;
			return p + 2;
		}
		else{
			*pAst = createType(AstNode);
			(*pAst)->type = TOKEN_SINGLE_PLUS;
			return p + 1;
		}
	}
	else if (p[0] == '-'){
		if (p[1] == '-'){
			*pAst = createType(AstNode);
			(*pAst)->type = TOKEN_SUBSUB;
			return p + 2;
		}
		else{
			*pAst = createType(AstNode);
			(*pAst)->type = TOKEN_SINGLE_SUB;
			return p + 1;
		}
	}
	else if (p[0] == '!'){
		*pAst = createType(AstNode);
		(*pAst)->type = TOKEN_NOT;
		return p + 1;
	}
	return NULL;
}

char* readCompareOperator(char*p, AstNode** pAst){
	long length = strlen(p);
	if (length < 3)return NULL;
	if (!strncmp(p, "===", 3)){
		*pAst = createType(AstNode);
		(*pAst)->type = TOKEN_COMPARE_TRIEQUAL;
		return p + 3;
	}
	else if (!strncmp(p, "==", 2)){
		*pAst = createType(AstNode);
		(*pAst)->type = TOKEN_COMPARE_EQUAL;
		return p + 2;
	}
	else if (!strncmp(p, "!=", 2)){
		*pAst = createType(AstNode);
		(*pAst)->type = TOKEN_NOT_EQUAL;
		return p + 2;
	}
	else if (!strncmp(p, "<=", 2)){
		*pAst = createType(AstNode);
		(*pAst)->type = TOKEN_LESS_EQUAL;
		return p + 2;
	}
	else if (!strncmp(p, ">=", 2)){
		*pAst = createType(AstNode);
		(*pAst)->type = TOKEN_GREATER_EQUAL;
		return p + 2;
	}
	else if (!strncmp(p, "<", 1)){
		*pAst = createType(AstNode);
		(*pAst)->type = TOKEN_LESS;
		return p + 1;
	}
	else if (!strncmp(p, ">", 1)){
		*pAst = createType(AstNode);
		(*pAst)->type = TOKEN_GREATER;
		return p + 1;
	}
	return NULL;
}

char* readExpPlus(char*p, AstNode** pAst)
{
	AstArrayA(3) = { { *readLogicOperator, *readC, *readExpPlus, 0 }, { *readEmpty, 0 }, { 0 } };
	AstNode* astNodes[3][10];
	int i = 0;
	int j = 0;
	char* ret = p;
	for (i = 0; a[i][0] != 0; i++){
		ret = p;
		for (j = 0; a[i][j] != 0; j++){
			ret = a[i][j](ret, &astNodes[i][j]);
			if (ret == NULL)
				break;
		}
		if (ret != NULL)break;
	}

	if (ret == NULL)return NULL;
	//puts("ExpPlus");
	*pAst = createType(AstNode);
	(*pAst)->type = AST_EXPPLUS;
	(*pAst)->num = j;
	(*pAst)->Content = NULL;
	(*pAst)->childNode = createTypeNum(AstNode, j);
	for (int k = 0; k < j; k++){ *((*pAst)->childNode + k) = *astNodes[i][k]; }
	return ret;
}

char* readExp(char*p, AstNode** pAst)
{
	AstArrayA(2) = { { *readC, *readExpPlus, 0 }, { 0 } };
	AstNode* astNodes[2][10];
	int i = 0;
	int j = 0;
	char* ret = p;
	for (i = 0; a[i][0] != 0; i++){
		ret = p;
		for (j = 0; a[i][j] != 0; j++){
			ret = a[i][j](ret, &astNodes[i][j]);
			if (ret == NULL)
				break;
		}
		if (ret != NULL)break;
	}

	if (ret == NULL)return NULL;
	//puts("Exp");
	*pAst = createType(AstNode);
	(*pAst)->type = AST_EXP;
	(*pAst)->num = j;
	(*pAst)->Content = NULL;
	(*pAst)->childNode = createTypeNum(AstNode, j);
	for (int k = 0; k < j; k++){ *((*pAst)->childNode + k) = *astNodes[i][k]; }
	return ret;
}

char* readD(char*p, AstNode** pAst)
{
	AstArrayA(4) = { { *readT, *readDPlus, 0 }, { 0 } };
	AstNode* astNodes[4][10];
	int i = 0;
	int j = 0;
	char* ret = p;
	for (i = 0; a[i][0] != 0; i++){
		ret = p;
		for (j = 0; a[i][j] != 0; j++){
			ret = a[i][j](ret, &astNodes[i][j]);
			if (ret == NULL)
				break;
		}
		if (ret != NULL)break;
	}

	if (ret == NULL)return NULL;
	//puts("Exp");
	*pAst = createType(AstNode);
	(*pAst)->type = AST_D;
	(*pAst)->num = j;
	(*pAst)->Content = NULL;
	(*pAst)->childNode = createTypeNum(AstNode, j);
	for (int k = 0; k < j; k++){ *((*pAst)->childNode + k) = *astNodes[i][k]; }
	return ret;
}

char* readDPlus(char*p, AstNode** pAst)
{
	AstArrayA(4) = { { *readOpAddMinus, readT, *readDPlus, 0 }, { *readEmpty, 0 }, { 0 } };
	AstNode* astNodes[4][10];
	int i = 0;
	int j = 0;
	char* ret = p;
	for (i = 0; a[i][0] != 0; i++){
		ret = p;
		for (j = 0; a[i][j] != 0; j++){
			ret = a[i][j](ret, &astNodes[i][j]);
			if (ret == NULL)
				break;
		}
		if (ret != NULL)break;
	}

	if (ret == NULL)return NULL;
	//puts("Exp");
	*pAst = createType(AstNode);
	(*pAst)->type = AST_DPlus;
	(*pAst)->num = j;
	(*pAst)->Content = NULL;
	(*pAst)->childNode = createTypeNum(AstNode, j);
	for (int k = 0; k < j; k++){ *((*pAst)->childNode + k) = *astNodes[i][k]; }
	return ret;
}


char* readT(char*p, AstNode** pAst)
{
	AstArrayA(2) = { { *readF, *readTPlus, 0 }, { 0 } };
	AstNode* astNodes[2][10];
	int i = 0;
	int j = 0;
	char* ret = p;
	for (i = 0; a[i][0] != 0; i++){
		ret = p;
		for (j = 0; a[i][j] != 0; j++){
			ret = a[i][j](ret, &astNodes[i][j]);
			if (ret == NULL)
				break;
		}
		if (ret != NULL)break;
	}

	if (ret == NULL)return NULL;
	//puts("T");
	*pAst = createType(AstNode);
	(*pAst)->type = AST_T;
	(*pAst)->num = j;
	(*pAst)->Content = NULL;
	(*pAst)->childNode = createTypeNum(AstNode, j);
	for (int k = 0; k < j; k++){ *((*pAst)->childNode + k) = *astNodes[i][k]; }
	return ret;
}

char* readTPlus(char*p, AstNode** pAst)
{
	AstArrayA(3) = { { *readOpMulDiv, *readF, readTPlus, 0 }, { *readEmpty, 0 }, { 0 } };
	AstNode* astNodes[3][10];
	int i = 0;
	int j = 0;
	char* ret = p;
	for (i = 0; a[i][0] != 0; i++){
		ret = p;
		for (j = 0; a[i][j] != 0; j++){
			ret = a[i][j](ret, &astNodes[i][j]);
			if (ret == NULL)
				break;
		}
		if (ret != NULL)break;
	}

	if (ret == NULL)return NULL;
	//puts("TPlus");
	*pAst = createType(AstNode);
	(*pAst)->type = AST_TPLUS;
	(*pAst)->num = j;
	(*pAst)->Content = NULL;
	(*pAst)->childNode = createTypeNum(AstNode, j);
	for (int k = 0; k < j; k++){ *((*pAst)->childNode + k) = *astNodes[i][k]; }
	return ret;
}

char* readC(char*p, AstNode** pAst)
{
	AstArrayA(4) = { { *readD, *readCompareOperator, *readD, 0 }, { *readD, 0 }, { 0 } };
	AstNode* astNodes[4][10];
	int i = 0;
	int j = 0;
	char* ret = p;
	for (i = 0; a[i][0] != 0; i++){
		ret = p;
		for (j = 0; a[i][j] != 0; j++){
			ret = a[i][j](ret, &astNodes[i][j]);
			if (ret == NULL)
				break;
		}
		if (ret != NULL)break;
	}

	if (ret == NULL)return NULL;
	//puts("C");
	*pAst = createType(AstNode);
	(*pAst)->type = AST_C;
	(*pAst)->num = j;
	(*pAst)->Content = NULL;
	(*pAst)->childNode = createTypeNum(AstNode, j);
	for (int k = 0; k < j; k++){ *((*pAst)->childNode + k) = *astNodes[i][k]; }
	return ret;
}

char* readF(char*p, AstNode** pAst)
{
	AstArrayA(8) = { { *readFunctionCall, 0 },
	{ *readArray, 0 },
	{ *readSingleOperator, *readExp, 0 },
	{ *readFactor, 0 },
	{ *readLeftBrace, *readExp, *readRightBrace, 0 },
	{ 0 } };

	AstNode* astNodes[8][10];
	int i = 0;
	int j = 0;
	char* ret = p;
	for (i = 0; a[i][0] != 0; i++){
		ret = p;
		for (j = 0; a[i][j] != 0; j++){
			ret = a[i][j](ret, &astNodes[i][j]);
			if (ret == NULL)
				break;
		}
		if (ret != NULL)break;
	}
	if (ret == NULL)return NULL;
	//puts("F");

	*pAst = createType(AstNode);
	(*pAst)->type = AST_F;
	(*pAst)->num = j;
	(*pAst)->Content = NULL;
	(*pAst)->childNode = createTypeNum(AstNode, j);
	for (int k = 0; k < j; k++){ *((*pAst)->childNode + k) = *astNodes[i][k]; }
	return ret;
}

char* readCompare(char *p, AstNode** pAst){
	AstArrayA(4) = { { *readF, *readCompareOperator, *readF, 0 }, { 0 } };
	AstNode* astNodes[4][10];
	int i = 0;
	int j = 0;
	char* ret = p;
	for (i = 0; a[i][0] != 0; i++){
		ret = p;
		for (j = 0; a[i][j] != 0; j++){
			ret = a[i][j](ret, &astNodes[i][j]);
			if (ret == NULL)
				break;
		}
		if (ret != NULL)break;
	}
	if (ret == NULL)return NULL;
	//puts("Compare");
	*pAst = createType(AstNode);
	(*pAst)->type = AST_COMPARE;
	(*pAst)->num = j;
	(*pAst)->Content = NULL;
	(*pAst)->childNode = createTypeNum(AstNode, j);
	for (int k = 0; k < j; k++){ *((*pAst)->childNode + k) = *astNodes[i][k]; }
	return ret;
}

char* readArray(char *p, AstNode** pAst)
{
	AstArrayA(4) = { { *readFactor, *readLeftBracket, *readExp, *readRightBracket, 0 }, { 0 } };
	AstNode* astNodes[4][10];
	int i = 0;
	int j = 0;
	char* ret = p;
	for (i = 0; a[i][0] != 0; i++){
		ret = p;
		for (j = 0; a[i][j] != 0; j++){
			ret = a[i][j](ret, &astNodes[i][j]);
			if (ret == NULL)
				break;
		}
		if (ret != NULL)break;
	}
	if (ret == NULL)return NULL;
	//puts("Array");
	*pAst = createType(AstNode);
	(*pAst)->type = TOKEN_ARRAY;
	(*pAst)->num = j;
	(*pAst)->Content = NULL;
	(*pAst)->childNode = createTypeNum(AstNode, j);
	for (int k = 0; k < j; k++){ *((*pAst)->childNode + k) = *astNodes[i][k]; }
	return ret;

}

char* readFunctionCall(char *p, AstNode** pAst)
{
	AstArrayA(4) = { { *readFactor, *readLeftBrace, *readExpList, *readRightBrace, 0 }, { 0 } };
	AstNode* astNodes[2][10];
	int i = 0;
	int j = 0;
	char* ret = p;
	for (i = 0; a[i][0] != 0; i++){
		ret = p;
		for (j = 0; a[i][j] != 0; j++){
			ret = a[i][j](ret, &astNodes[i][j]);
			if (ret == NULL)
				break;
		}
		if (ret != NULL)break;
	}
	if (ret == NULL)return NULL;
	//puts("FunctionCall");
	*pAst = createType(AstNode);
	(*pAst)->type = AST_FUNCTION;
	(*pAst)->num = j;
	(*pAst)->Content = NULL;
	(*pAst)->childNode = createTypeNum(AstNode, j);
	for (int k = 0; k < j; k++){ *((*pAst)->childNode + k) = *astNodes[i][k]; }
	return ret;

}

char* readExpList(char *p, AstNode** pAst)
{
	AstArrayA(4) = { { *readExp, *readComma, *readExpList, 0 }, { *readExp, 0 }, { *readEmpty, 0 }, 0 };
	AstNode* astNodes[4][10];
	int i = 0;
	int j = 0;
	char* ret = p;
	for (i = 0; a[i] != 0; i++){
		ret = p;
		for (j = 0; a[i][j] != 0; j++){
			ret = a[i][j](ret, &astNodes[i][j]);
			if (ret == NULL)
				break;
		}
		if (ret != NULL)break;
	}
	if (ret == NULL)return NULL;
	//puts("ExpList");
	*pAst = createType(AstNode);
	(*pAst)->type = AST_EXPLIST;
	(*pAst)->num = j;
	(*pAst)->Content = NULL;
	(*pAst)->childNode = createTypeNum(AstNode, j);
	for (int k = 0; k < j; k++){ *((*pAst)->childNode + k) = *astNodes[i][k]; }
	return ret;

}


char* readEmpty(char*p, AstNode** pAst){
	while (*p == '\t' || *p == ' '){
		p++;
	}
	*pAst = createType(AstNode);
	(*pAst)->type = AST_NULL;
	return p;
}

char* readSpace(char*p, AstNode** pAst){
	if (*p != '\t'&&*p != ' ')return NULL;
	while (*p == '\t' || *p == ' '){
		p++;
	}
	*pAst = createType(AstNode);
	(*pAst)->type = AST_NULL;
	return p;
}

int isNum(char c){
	return c >= '0'&&c <= '9';
}
int isAlpha(char c){
	return (c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z');
}
int isAlnum(char c){
	return isNum(c) || isAlpha(c);
}

char* readChar(char* p, char c){
	if (*p == c){
		return p + 1;
	}
	return NULL;
}

char *readDefault(char *p, const char* s){
	while (s != 0){
		if (*(p++) != *(s++)){
			return NULL;
		}
	}
	return p;
}

char* readString(char*p, char*s){
	if (*p != '\'')return NULL;
	char *ret = p;
	int condition = 0;
	while (1){
		ret++;
		if (*ret == 0)return NULL;
		if (!condition){
			if (*ret == '\''){
				memcpy(s, p + 1, ret - p + 2);
				s[ret - p - 1] = 0;
				return ret + 1;
			}
			if (*ret == '\\'){
				condition = 1;
			}
		}
		else{
			condition = 0;
		}
	}
	return NULL;
}

char *readValname(char*p, char*s){
	char *ret = p;
	if (!isAlpha(*ret))return NULL;
	while (isAlnum(*ret) || *ret == '.'){
		ret++;
	}
	if (*(ret - 1) == '.')return NULL;
	char*ptr = p;
	int i = 0;
	for (; p < ret; p++, i++){
		s[i] = *p;
	}
	s[i] = 0;
	return ret;
}
char* readUInt(char* p, int* num){
	char *ret = p;
	if (!isNum(*ret))return NULL;
	while (isNum(*ret)){
		ret++;
	}
	char *s = (char*)malloc(ret - p + 1);
	memcpy(s, p, ret - p);
	s[ret - p + 1] = 0;
	sscanf(s, "%d", num);
	return ret;
}

char *readInt(char *p, int* num){
	char *ret = p;
	if (isNum(*p)){
		return readUInt(p, num);
	}
	else if (*p == '-'){
		ret = readUInt(p + 1, num);
		*num = -*num;
		return ret;
	}
	return NULL;
}



char* readDotDouble(char *p, double *num){
	char *ret = p;
	char *tmpRet;
	int numInt = 0;
	if (*ret != '.'){
		return NULL;
	}
	tmpRet = readUInt(p + 1, &numInt);
	if (tmpRet == NULL)return NULL;
	*num = numInt;
	while (*num>1 - DBL_EPSILON){
		*num /= 10;
	}
	return tmpRet;
}

char* readUDouble(char *p, double *pnum){
	char *ret = p;
	int i;
	ret = readInt(p, &i);
	if (ret == NULL){
		ret = readDotDouble(p, pnum);
		if (ret == NULL){
			return NULL;
		}
		else{
			return ret;
		};
	}
	else{
		p = ret;
		ret = readDotDouble(p, pnum);
		if (ret == NULL){
			*pnum = i;
			return p;
		}
		else{
			*pnum = *pnum + i;
			return ret;
		}
	}
}
char *readDouble(char *p, double* pnum){
	char *ret = p;
	if (isNum(*p) || *p == '.'){
		return readUDouble(p, pnum);
	}
	else if (*p == '-'){
		ret = readUDouble(p + 1, pnum);
		*pnum = -*pnum;
		return ret;
	}
	return NULL;
}

char *readFactor(char *p, AstNode** pAst){
	char *ret;
	char s[1000];
	double d;
	(*pAst) = createType(AstNode);
	(*pAst)->num = 1;
	(*pAst)->childNode = NULL;
	if ((ret = readUDouble(p, &d))){
		(*pAst)->type = TOKEN_DOUBLE;
		(*pAst)->Content = createType(double);
		*(double*)(*pAst)->Content = d;
		return ret;
	}
	else
		if ((ret = readValname(p, s))){
		(*pAst)->type = TOKEN_VALNAME;
		(*pAst)->Content = cp_str(s);
		return ret;
		}
		else
			if ((ret = readString(p, s))){
		(*pAst)->type = TOKEN_STRING;
		(*pAst)->Content = cp_str(s);
		return ret;
			}
	return NULL;
}

char* readOpMulDiv(char *p, AstNode** pAst){
	if (*p == '*' || *p == '/' || *p == '%'){
		(*pAst) = createType(AstNode);
		(*pAst)->num = 1;
		(*pAst)->childNode = NULL;
		(*pAst)->type = (*p == '*') ? TOKEN_MUL : (*p == '/') ? TOKEN_DIV : TOKEN_MOD;
		return p + 1;
	}
	return NULL;
}
char* readOpAddMinus(char *p, AstNode** pAst){
	if (*p == '+' || *p == '-'){
		(*pAst) = createType(AstNode);
		(*pAst)->num = 1;
		(*pAst)->childNode = NULL;
		(*pAst)->type = (*p == '+') ? TOKEN_ADD : TOKEN_SUB;
		return p + 1;
	}
	return NULL;
}