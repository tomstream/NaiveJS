//
//  node.h
//  JSInterpreter
//
//  Created by thomas on 15/12/22.
//  Copyright © 2015年 thomas. All rights reserved.
//

#ifndef node_h
#define node_h

#include <stdio.h>
struct NodeStruct;
typedef struct NodeStruct AstNode;
struct NodeStruct{
    int type;
    void* Content;
    struct NodeStruct* childNode;
    int num;
};


#endif /* node_h */
