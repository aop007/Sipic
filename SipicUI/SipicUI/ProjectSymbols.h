//
//  ProjectSymbols.h
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-26.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#import <Foundation/Foundation.h>
#define  SYMBOL_FILE_PATH  "/Users/aop007/Documents/Projets/DawnStar/Sipic/Sipic/Sipic/InputFiles/main.sym"
#define  ROM_START_TOKEN   "ROM Allocation:"

@interface ProjectSymbols : NSObject
{
    NSArray *symbolList;
}

-(NSString *)getSymbolAtAddr:(int)addr;

@end
