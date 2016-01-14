//
//  str.h
//  JSInterpreter
//
//  Created by thomas on 15/11/28.
//  Copyright © 2015年 thomas. All rights reserved.
//

#ifndef str_h
#define str_h

#include <stdio.h>

char * cp_str(const char* s);

char * plus_str(const char* s1, const char* s2);

char *read_until(char s[], char c, char* ret);


#endif /* str_h */
