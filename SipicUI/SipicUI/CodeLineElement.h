//
//  CodeLineElement.h
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-12-04.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CodeLineElement : NSObject
{

}

@property (assign) NSString *line;
@property (assign) int       index;
@property (assign) bool      breakOnPC;
@property (assign) bool      clearOnBreak;
@property (assign) bool      isCode;
@property (assign) int       addr;



@end

struct break_point_list;

typedef  struct  break_point_list {
    struct  break_point_list  *next;
    CodeLineElement   *cle;
} BREAK_POINT_LIST;