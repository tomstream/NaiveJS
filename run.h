#ifndef run_h
#define run_h
#include "node.h"
#include "map.h"
void initMap();
AstNode* loadVariable(const char* varname, int mapNumber);
void saveVariable(const char* varname, AstNode* astNode, int mapNumber);

void initNode();

AstNode* getExpList(AstNode* ast);
AstNode* callFunc(const char* varname, AstNode*explist, int mapNumber);
int isFactor(int i);
AstNode* runNode(AstNode* pAst,int mapNumber);

double myAbs(double d);
AstNode* cpAst(AstNode* ast);
#endif 