//
//  type.h
//  JSInterpreter
//
//  Created by thomas on 15/12/22.
//  Copyright © 2015年 thomas. All rights reserved.
//

#ifndef type_h
#define type_h

#include <stdio.h>
#include "node.h"

#define TOKEN_INT 0
#define TOKEN_DOUBLE 1
#define TOKEN_STRING 2
#define TOKEN_ARRAY 3
#define TOKEN_VALNAME 4
#define TOKEN_OPERATOR 5
#define TOKEN_ADD 6
#define TOKEN_SUB 7
#define TOKEN_MUL 8
#define TOKEN_DIV 9 
#define TOKEN_MOD 10
#define TOKEN_NULL 11
#define TOKEN_UNDEFINED 12

#define TOKEN_LEFT_BRACE 13
#define TOKEN_RIGHT_BRACE 14
#define TOKEN_FUNCTION 15
#define TOKEN_COMMA 16
#define TOKEN_PLUSPLUS 17
#define TOKEN_SUBSUB 18
#define TOKEN_SINGLE_PLUS 19
#define TOKEN_SINGLE_SUB 20
#define TOKEN_COMPARE_EQUAL 21
#define TOKEN_COMPARE_TRIEQUAL 22
#define TOKEN_NOT_EQUAL 23
#define TOKEN_LESS 24
#define TOKEN_GREATER 25
#define TOKEN_GREATER_EQUAL 26
#define TOKEN_LESS_EQUAL 27
#define TOKEN_AND 28
#define TOKEN_OR 29
#define TOKEN_NOT 30
#define TOKEN_BREAK 31
#define TOKEN_CONTINUE 32

#define AST_EXP 33
#define AST_EXPPLUS 34
#define AST_T 35
#define AST_TPLUS 36
#define AST_F 37
#define AST_FUNCTION 38
#define AST_NULL 39
#define AST_EXPLIST 40
#define AST_COMPARE 41
#define AST_C 42
#define AST_CPlus 43
#define AST_D 44
#define AST_DPlus 45

#define AST_BLOCK 46
#define AST_STMT 47
#define AST_STMTS 48
#define AST_EXPR 49
#define AST_IF_STMT 50
#define AST_WHILE_STMT 51
#define AST_DO_WHILE_STMT 52
#define AST_FOR_STMT 53
#define AST_FUNC_DEF_STMT 54
#define AST_VAR_DEF 55
#define AST_SWITCH_STMT 56
#define AST_RETURN_STMT 57
#define AST_NEW_STMT 58
#define AST_CASE_STMT 59
#define AST_VAR_LIST 60
#define AST_BREAK 61
#define AST_CONTINUE 62
#define AST_IF_AND_ELSE 63

#define ERROR 9999
#define NOTNULL 10000
#define AstArrayA(n) char *(*a[n][10])(char*, AstNode**)

#define AstArray(name,n) char *(* ##name[n][10])(char*, AstNode**)

#define createType(type)\
(type*)malloc(sizeof(type))

#define createTypeNum(type, num)\
(type*)malloc(sizeof(type)*(num))

#define readChaDeclare(Charname,cha)\
char* read##Charname(char *p, AstNode** pAst);\


#define readChaDefine(Charname,cha)\
char* read##Charname(char *p, AstNode** pAst){\
    if(*p!=cha)\
        return NULL;\
    *(pAst)=createType(AstNode);\
    (*(pAst))->childNode=NULL;\
    (*(pAst))->Content=NULL;\
    (*(pAst))->num=1;\
    (*(pAst))->type=AST_NULL;\
    return p+1;\
}

#define readStringDeclare(StringName,str)\
char* readString##StringName(char*p, AstNode** pAst)

#define readStringDefine(StringName,str,TOKEN)\
char* readString##StringName(char*p, AstNode** pAst){\
	long length = strlen(str);\
	if (!strncmp(p, str, length) != 0){\
		 *(pAst)=createType(AstNode);\
		(*(pAst))->childNode=NULL;\
		(*(pAst))->Content=NULL;\
		(*(pAst))->num=1;\
		(*(pAst))->type=TOKEN;\
		return p + length;\
	}\
	return NULL;\
}

#define readAstDeclare(funcName)\
char* read##funcName(char*p, AstNode** pAst)

#define readAstDefine(funcName,name,Token, n)\
char* read##funcName(char*p, AstNode** pAst)\
{\
	AstNode* astNodes[n][10];\
	int i = 0;\
	int j = 0;\
	char* ret = p;\
	for (i = 0; ##name[i][0] != 0; i++){\
		ret = p;\
		for (j = 0; ##name[i][j] != 0; j++){\
			ret = ##name[i][j](ret, &astNodes[i][j]);\
			if (ret == NULL)\
				break;\
		}\
		if (ret != NULL)break;\
	}\
	\
	if (ret == NULL)return NULL;\
	*pAst = createType(AstNode);\
	(*pAst)->type = Token;\
	(*pAst)->num = j;\
	(*pAst)->Content = NULL;\
	(*pAst)->childNode = createTypeNum(AstNode, j);\
	for (int k = 0; k < j; k++){ *((*pAst)->childNode + k) = *astNodes[i][k]; }\
	return ret;\
}\

#endif /* type_h */
