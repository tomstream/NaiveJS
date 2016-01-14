//
//  node.c
//  JSInterpreter
//
//  Created by thomas on 15/12/22.
//  Copyright © 2015年 thomas. All rights reserved.
//

#include "node.h"
#include <stdlib.h>

AstNode* createAstNode()
{
    return (AstNode*)malloc(sizeof(AstNode));
}