//
//  str.c
//  JSInterpreter
//
//  Created by thomas on 15/11/28.
//  Copyright © 2015年 thomas. All rights reserved.
//
#define _CRT_SECURE_NO_WARNINGS
#include "str.h"
#include <string.h>
#include <stdlib.h>

char * cp_str(const char* s)
{
    long len = strlen(s);
    char* ret = (char*)malloc(sizeof(char)*(len+1));
    strcpy(ret, s);
    return ret;
}

char * plus_str(const char* s1, const char* s2)
{
    long len1 = strlen(s1);
    long len2 = strlen(s2);
    char* ret = (char*)malloc(sizeof(char)*(len1+len2+1));
    memcpy(ret, s1, len1);
    memcpy(ret+len1, s2, len2+1);
    return ret;
}

char *read_until(char s[], char c, char* ret)
{
    char *p=s;
    int len=0;
    for(;*p!=0&&*p!=c;)
    {
        len++;
    }
    ret=(char*)malloc(sizeof(char)*(len+1));
    memcpy(ret,s,len);
    ret[len]=0;
    return s+len;
}