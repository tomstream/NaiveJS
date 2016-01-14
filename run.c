#define _CRT_SECURE_NO_WARNINGS
#include "run.h"
#include "node.h"
#include "type.h"
#include "str.h"
#include "float.h"
#include "stdio.h" 
#include "string.h"
map_ast_t nodeMap[1000];
int mapCount = 0;

AstNode nodeNull;
AstNode nodeUndefine;
//AstNode nodeNotNull;
AstNode nodeError;
AstNode nodeBreak;
AstNode nodeContinue;
AstNode nodeReturn;

int inWhile = 0;

void initMap(){
	map_init(&nodeMap[0]);
}

void initNode(){
	nodeNull.type = AST_NULL;
	nodeUndefine.type = TOKEN_UNDEFINED;
	nodeError.type = ERROR;
	nodeContinue.type = AST_CONTINUE;
	nodeBreak.type = AST_BREAK;
	nodeReturn.type = AST_RETURN_STMT;
	//nodeNotNull.type = NOTNULL;
}

AstNode* loadVariable(const char* varname, int i){
	AstNode *val = map_get(&nodeMap[i], varname);
	if (val) {
		return val;
		//printf("value: %d\n", *val);
	}
	else {
		val = map_get(&nodeMap[0], varname);
		if (val){
			return val;
		}

		return &nodeUndefine;
	}
}

void saveVariable(const char* varname, AstNode* astNode,int i){
	map_set(&nodeMap[i], varname, *astNode);
}

int isFactor(int i){
	if (i == TOKEN_VALNAME || i == TOKEN_DOUBLE || i == TOKEN_STRING ||i==TOKEN_ARRAY)
		return 1;
	else return 0;
}

AstNode* runNode(AstNode* pAst, int mapNumber){
	switch (pAst->type){
	case ERROR:{
		return &nodeError;
		break;
	}
	case TOKEN_UNDEFINED:{
		return &nodeUndefine;
		break;
	}
	case TOKEN_NULL:{
		return &nodeNull;
		break;
	}
	case AST_F:{
		if (isFactor(pAst->childNode->type)){
			if (pAst->childNode->type == TOKEN_STRING || pAst->childNode->type == TOKEN_DOUBLE){
				return pAst->childNode;
			}
			else if(pAst->childNode->type==TOKEN_VALNAME){
				return loadVariable((char*)(pAst->childNode->Content),mapNumber);
			}
			else{
				AstNode* p = runNode(pAst->childNode,mapNumber);
				return loadVariable((char*)(p->Content), mapNumber);
			}
		}
		else if (pAst->num == 3){//exp
			return runNode((pAst->childNode + 1),mapNumber);
		}
		else if (pAst->num == 2){//{+,-,++,--,!} E 
			AstNode* p = runNode(pAst->childNode + 1,mapNumber);
			if (p == NULL || p->type != TOKEN_DOUBLE)return &nodeError;
			AstNode* ret = cpAst(p);
			double *d = (double*)ret->Content;
			switch (pAst->childNode->type){
			case TOKEN_PLUSPLUS:*d += 1; break;
			case TOKEN_SUBSUB:*d -= 1; break;
			case TOKEN_NOT:*d = (d != 0) ? 1 : 0; break;
			case TOKEN_SINGLE_PLUS:break;
			case TOKEN_SINGLE_SUB:*d *= -1; break;
			}
			return ret;
		}
		else if (pAst->childNode->type == TOKEN_ARRAY){
			return runNode(pAst->childNode,mapNumber);
		}
		else if (pAst->childNode->type = AST_FUNCTION){
			return runNode(pAst->childNode,mapNumber)->childNode;
		}
		else if (pAst->childNode->type = TOKEN_ARRAY){
			return runNode(pAst->childNode,mapNumber);
		}
		else return &nodeUndefine;
		break;
	}
	case AST_TPLUS:{
		AstNode *p = pAst->childNode;
		if (p->type == AST_NULL)return &nodeNull;
		AstNode *F = runNode(p + 1,mapNumber);
		AstNode *Tp = runNode(p + 2,mapNumber);
		AstNode* ret = cpAst(F);
		double *d = (double*)ret->Content;
		
		double tP = (Tp->type == AST_NULL) ? 1 : *((double*)Tp->Content);
		if (p->type == TOKEN_MUL){
			*d = *d*tP;
		}
		else if (p->type == TOKEN_DIV){
			if (*d == 0)return &nodeError;
			*d = 1.0 / *d * tP;
		}
		return ret;
		break;
	}
	case AST_T:{
		AstNode *p1 = runNode(pAst->childNode,mapNumber);
 		AstNode *p2 = runNode(pAst->childNode + 1,mapNumber);
		if (p1->type == TOKEN_DOUBLE&&p2->type == TOKEN_DOUBLE){
			AstNode* ret = cpAst(p1);
			double *d = (double*)ret->Content;
			double d2 = *(double*)p2->Content;
			*d = *d*d2;
			return ret;
		}
		else if (p1->type == TOKEN_DOUBLE&&p2->type == AST_NULL){
			return p1;
		}
		else if (p1->type == TOKEN_STRING&&p2->type == AST_NULL){
			return p1;
		}
		else if (p2->type == AST_NULL)return &nodeNull;
		else return &nodeError;
		break;
	}
	case AST_DPlus:{
		AstNode *p = pAst->childNode;
		if (p->type == AST_NULL)return &nodeNull;
		AstNode *F = runNode(p + 1,mapNumber);
		AstNode *Dp = runNode(p+2,mapNumber);
		AstNode* ret = cpAst(F);
		if (ret->type == TOKEN_DOUBLE&&(Dp->type==TOKEN_DOUBLE||Dp->type==AST_NULL)){
			double *d = (double*)(ret->Content);
			double tP = (Dp->type == AST_NULL) ? 0 : *((double*)Dp->Content);
			if (p->type == TOKEN_ADD){
				*d = *d + tP;
			}
			else if (p->type == TOKEN_SUB){
				*d = -*d + tP;
			}
		}
		if (ret->type == TOKEN_STRING && Dp->type == TOKEN_STRING){
			ret->Content = plus_str(ret->Content, Dp->Content);
		}
		return ret;
		break;
	}
	case AST_D:{
		AstNode *p1 = runNode(pAst->childNode,mapNumber);
		AstNode *p2 = runNode(pAst->childNode + 1,mapNumber);
		if (p1->type == TOKEN_DOUBLE&&p2->type == TOKEN_DOUBLE){
			AstNode* ret = cpAst(p1);
			double *d = (double*)ret->Content;
			double d2 = *(double*)p2->Content;
			*d = *d + d2;
			return ret;
		}
		if (p1->type == TOKEN_STRING&&p2->type == TOKEN_STRING){
			AstNode* ret = cpAst(p1);
			char *s1 = (char*)ret->Content;
			char *s2 = (char*)p2->Content;
			char *s3 = plus_str(s1, s2);
			ret->Content = s3;
			return ret;
		}
		else if (p2->type == AST_NULL)return cpAst(p1);
		else return &nodeError;
		break;
	}
	case AST_C:{
		if (pAst->num == 1)return runNode(pAst->childNode,mapNumber);
		AstNode* p1 = runNode(pAst->childNode,mapNumber);
		AstNode* p2 = runNode(pAst->childNode + 2,mapNumber);
		if (p1->type == TOKEN_DOUBLE&&p2->type == TOKEN_DOUBLE){
			AstNode* ret = cpAst(p1);
			double *d1 = (double*)ret->Content;
			double *d2 = (double*)p2->Content;
			int type = (pAst->childNode + 1)->type;
			if (type == TOKEN_COMPARE_EQUAL){
				*d1 = (myAbs(*d1 - *d2) < DBL_MIN);
				return ret;
			}
			else if (type == TOKEN_LESS){
				*d1 = (*d1 - *d2 < 0);
				return ret;
			}
			else if (type == TOKEN_LESS_EQUAL){
				*d1 = (*d1 - *d2 - DBL_MIN < 0);
				return ret;
			}
			else if (type == TOKEN_GREATER){
				*d1 = (*d1 - *d2>0);
				return ret;
			}
			else if (type == TOKEN_GREATER_EQUAL){
				*d1 = (*d1 - *d2 + DBL_MIN > 0);
				return ret;
			}
			else{
				return &nodeError;
			}
		}
		else if (p1->type == TOKEN_STRING&&p2->type == TOKEN_STRING){
			AstNode* ret = cpAst(p1);
			int type = (pAst->childNode + 1)->type;
			char* s1 = (char*)(ret->Content);
			char* s2 = (char*)(p2->Content);
			if (type == TOKEN_COMPARE_EQUAL || type == TOKEN_NOT_EQUAL){
				double d = !strcmp(s1, s2);
				ret->type = TOKEN_DOUBLE;
				*(double *)(ret->Content) = (type == TOKEN_COMPARE_EQUAL) ? d : !d;
				return ret;
			}
			else{
				return &nodeError;
			}
		}
		else {
			return &nodeError;
		}
		break;
	}
	case AST_EXP:{
		return runNode(pAst->childNode,mapNumber);
		break;
	}
	case TOKEN_ARRAY:{
		AstNode* indexNode = runNode(pAst->childNode+2,mapNumber);
		AstNode* nameNode = cpAst(pAst->childNode);
		if (nameNode->type==TOKEN_VALNAME&&indexNode->type == TOKEN_DOUBLE){
			int index = *(double*)(indexNode->Content);
			static char buffer[100];
			nameNode->type = TOKEN_VALNAME;
			sprintf(buffer, "%s[%d]",(char*)(nameNode->Content),index);
			nameNode->Content = cp_str(buffer);
		}
		return nameNode;
		break;
	}
	case AST_EXPLIST:{
		if (pAst->childNode->type == AST_NULL){
			pAst->type = AST_EXPLIST;
			pAst->num = 0;
			return cpAst(pAst);
		}
		if (pAst->num == 1){
			AstNode* p = runNode(pAst->childNode,mapNumber);
			AstNode* ret = cpAst(pAst);
			ret->type = AST_EXPLIST;
			ret->num = 1;
			ret->childNode = createType(AstNode);
			*(ret->childNode) = *p;
			return ret;
		}
		AstNode* p1 = runNode(pAst->childNode,mapNumber);
		AstNode* p2 = runNode(pAst->childNode+2,mapNumber);
		int num = ++(p2->num);
		AstNode* childNode = createTypeNum(AstNode, num);
		*childNode = *p1;
		for (int i = 1; i < num; i++){
			childNode[i] = p2->childNode[i - 1];
		}
		free(p2->childNode);
		p2->childNode = childNode;
		return p2;
		break;
	}
	case AST_EXPR:{
		if (pAst->num == 1){
			return runNode(pAst->childNode,mapNumber);
		}
		else{
			AstNode* p1 =pAst->childNode;
			AstNode* p2 = runNode(pAst->childNode + 2, mapNumber);
			if (p1->type == TOKEN_VALNAME){
				char* varname = (char*)p1->Content;
				saveVariable(varname, p2, mapNumber);
			}
			else if (p1->type == TOKEN_ARRAY){
				p1 = runNode(p1, mapNumber);
				char* varname = (char*)p1->Content;
				saveVariable(varname, p2, mapNumber);
			}
		}
		return &nodeNull;
		break;
	}
	case AST_STMTS:{
		if (pAst->num == 1){
			return &nodeNull;
		}
		AstNode* p=runNode(pAst->childNode,mapNumber);
		if (p->type == AST_CONTINUE || p->type == AST_BREAK ||p->type==AST_RETURN_STMT){
			return p;
		}
		return runNode(pAst->childNode+1,mapNumber);
		break;
	}
	case AST_STMT:{
		return runNode(pAst->childNode,mapNumber);
		break;
	}
	case AST_BLOCK:{
		return runNode(pAst->childNode + 1,mapNumber);
		break;
	}
	case AST_IF_STMT:{
		AstNode* p1 = runNode(pAst->childNode + 2,mapNumber);
		if (p1->type != TOKEN_DOUBLE)return &nodeNull;
		double d = *(double*)(p1->Content);
		if (myAbs(d) < DBL_MIN)return &nodeNull;
		return 
			runNode(pAst->childNode + 4,mapNumber);
	}
	case AST_WHILE_STMT:{
		inWhile = 1;
		while (1){
			AstNode* p1 = runNode(pAst->childNode + 2,mapNumber);
			if (p1->type != TOKEN_DOUBLE){ 
				inWhile = 0;
				return &nodeNull; 
			}
			double d = *(double*)(p1->Content);
			if (myAbs(d) < DBL_MIN){
				inWhile = 0;
				return &nodeNull; 
			}
			AstNode *p=runNode(pAst->childNode + 4,mapNumber);
			if (p->type == AST_CONTINUE)continue;
			else if (p->type==AST_BREAK){
				inWhile = 0;
				return &nodeNull;
			}
		}
	}
	case AST_BREAK:{
		if (inWhile == 0)return &nodeNull;
		return &nodeBreak;
		break;
	}
	case AST_CONTINUE:{
		if (inWhile == 0)return &nodeNull;
		return &nodeContinue;
		break;
	}
	case AST_FUNC_DEF_STMT:{
		return &nodeNull;
		break;
	}
	case AST_FUNCTION:{
		if (pAst->childNode->type != TOKEN_VALNAME)
			return &nodeError;
		char *name = (char*)(pAst->childNode->Content);
		AstNode* explist = runNode(pAst->childNode+2,mapNumber);
		return callFunc(name, explist,mapNumber);
	}
	case AST_RETURN_STMT:{
		AstNode* retValue = runNode(pAst->childNode + 1, mapNumber);
		nodeReturn.childNode = retValue;
		return &nodeReturn;
		break;
	}
	}

}
AstNode* callFunc(const char* varname, AstNode*explist,int mapNumber){
	
	if (!strcmp(varname, "print")){
		AstNode* nodes = explist->childNode;
		printf("[out] ");
		for (int i = 0; i < explist->num; i++){
			if (nodes[i].type == TOKEN_DOUBLE){
				double d = *(double*)nodes[i].Content;
				if (myAbs(d - (int)(d)) < DBL_MIN){
					printf("%d ", (int)d);
				}
				else{
					printf("%f ", d);
				}
			}
			else if (nodes[i].type == TOKEN_STRING){
				printf("%s ", (char*)(nodes[i].Content));
			}
			else if (nodes[i].type == AST_NULL){
				printf("NULL ");
			}
			else if (nodes[i].type == TOKEN_UNDEFINED){
				printf("UNDEFINED ");
			}
		}
		printf("\n");
		nodeReturn.childNode = &nodeNull;
	}
	else if (!strcmp(varname, "getchar")){
		getchar();
		nodeReturn.childNode = &nodeNull;
	}
	else if (!strcmp(varname, "str")){
		AstNode* nodes = explist->childNode;
		if (explist->type != TOKEN_DOUBLE){
			return &nodeError;
		}
		AstNode* ret = createType(AstNode);
		ret->type = TOKEN_STRING;
		static char buffer[300];
		double d = *(double*)nodes[0].Content;
		if (myAbs(d - (int)(d))<DBL_MIN){
			sprintf(buffer, "%d", (int)(d));
			ret->Content = cp_str(buffer);
		}
		else{
			sprintf(buffer, "%f", d);
			ret->Content = cp_str(buffer);
		}
		nodeReturn.childNode = &nodeNull;
	}
	else if (!strcmp(varname, "newObject")){
		nodeReturn.childNode = &nodeNull;
	}
	else if (!strcmp(varname, "newArray")){
		nodeReturn.childNode = &nodeNull;
	}
	else{
		AstNode* function = loadVariable(varname,mapNumber);
		if (function->type != AST_FUNC_DEF_STMT){
			nodeReturn.childNode= &nodeError;
			return &nodeReturn;
		}
		
		AstNode* expNodes = explist->childNode;
		AstNode* varNodes = (function->childNode + 1)->childNode;
		int expCount = explist->num;
		int varCount = (function->childNode+1)->num;
		int count = (expCount < varCount) ? expCount : varCount;
		mapCount += 1;
		map_init(&nodeMap[mapCount]);
		for (int i = 0; i < count; i++){
			saveVariable((char*)varNodes[i].Content, &expNodes[i], mapCount);
		}
		AstNode* ret = runNode(function->childNode+2, mapCount);
		map_deinit(&nodeMap[mapCount]);
		if (ret->type != AST_RETURN_STMT){
			nodeReturn.childNode = &nodeNull;
			return &nodeReturn;
		}
		mapCount -= 1;
		return ret;
	}
	return &nodeReturn;
}

double myAbs(double d){
	return (d >= 0) ? d : -d;
}

AstNode* cpAst(AstNode* ast){
	AstNode* ret = createType(AstNode);
	*ret = *ast;
	if (ast->type == TOKEN_DOUBLE){
		ret->Content=createType(double);
		*(double *)(ret->Content) = *(double*)(ast->Content);
	}
	else if (ast->type == TOKEN_STRING||ast->type==TOKEN_ARRAY){
		ret->Content = cp_str((char*)(ast->Content));
	}
	return ret;
}