//
//  main.c
//  JSInterpreter
//
//  Created by thomas on 15/11/28.
//  Copyright © 2015年 thomas. All rights reserved.
//
#define _CRT_SECURE_NO_WARNINGS
#include "node.h"
#include "str.h"
#include "type.h"
#include "run.h"
#include "parse.h"
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <string.h>


int main(int argc, const char * argv[]) {
	initMap();
	initNode();
	char* code;
	if(argc==1)code =filter("test3.js");
	else{
		code = filter(argv[1]);
		if (!code){
			puts("this file does not exist");
			return;
		}
	}
	//puts(code);
    AstNode *p;
	char* ret;
    if(!(ret=readStmts(code,&p)))
        puts("error");
	else{
		puts(ret);
	}
	AstNode* node=runNode(p,0);
    getchar();
    return 0;
}
