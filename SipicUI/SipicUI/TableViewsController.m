//
//  TableViewsController.m
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-26.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#import "TableViewsController.h"
#import "sim.h"

@implementation TableViewsController

-(id)init
{
    self = [super init];
    if (self) {
        p_symbols = [[ProjectSymbols alloc] init];
    }
    
    return self;

}


- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    switch ([aTableView tag]) {
        case TABLE_VIEW_MEM_VIEW1_TAG:
        case TABLE_VIEW_MEM_VIEW2_TAG:
            return MEM_VIEW_SIZE_WORD;
            
        case TABLE_VIEW_CALL_STACK_TAG:
            return Sim_GetCallDepth();
            
        case TABLE_VIEW_CODE_LISTING_TAG:
            return [p_code_listing count];
            
        default:
            return 0;
    }
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    NSString *returning;  
    
    switch ([aTableView tag]) {
        case TABLE_VIEW_MEM_VIEW1_TAG:
        case TABLE_VIEW_MEM_VIEW2_TAG:
            returning = [self GetDataMemoryTable:aTableColumn row:rowIndex];
            return returning;
            

        case TABLE_VIEW_CALL_STACK_TAG:
            returning = [self GetCallStackTable:aTableColumn row:rowIndex];
            return returning;
            
#if 0     
            
        case TABLE_VIEW_CODE_LISTING_TAG:
            return [p_code_listing count];
#endif
        default:
            return 0;
    }
}


- (NSString *)GetCallStackTable:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    NSNumber  *number;
    NSString  *value;
    
    value = [aTableColumn identifier];
    
    if ([[aTableColumn identifier] isEqualToString:@"Addr"]) {
        number = [[NSNumber alloc]initWithInt:(NSInteger)Sim_AddrForDepth(rowIndex)];
        value  = [NSString stringWithFormat:@"%06X", [number intValue]];
    } else {
        
        value = [p_symbols getSymbolAtAddr:Sim_AddrForDepth(rowIndex)];
    }
    
    return value;
}

- (NSString *)GetDataMemoryTable:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    NSNumber  *number;
    NSString  *identifier;
    NSString  *value;
    
    
    identifier = [aTableColumn identifier];
//    number     = [[NSNumber alloc] init];
    
    
    if ([[aTableColumn identifier] isEqualToString:@"Addr"]) {
        number = [[NSNumber alloc]initWithLong:(rowIndex * 2)];
        value  = [NSString stringWithFormat:@"%04X", [number intValue]];
    } else {
        number = [[NSNumber alloc] initWithInt:Sim_GetValueFromDataMem(rowIndex*2)];
        value = [NSString stringWithFormat:@"%04X", [number intValue]];
    }
    
    return value;
}


@end
