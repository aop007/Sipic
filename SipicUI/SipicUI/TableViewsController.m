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
        p_code_listing = [[NSMutableDictionary alloc] init];
        p_code_all     = [[NSMutableArray alloc] init];
        
        [self loadCodeLST:p_code_listing withFile:@"/Users/aop007/Documents/Projets/DawnStar/Sipic/Sipic/Sipic/InputFiles/main.lst" allLines:p_code_all];
        [self loadCodeTXT:p_code_listing withFile:@"/Users/aop007/Documents/Projets/DawnStar/Sipic/Sipic/Sipic/InputFiles/main.txt" allLines:p_code_all];
    }
    
    return self;

}

-(void)loadCodeLST:(NSMutableDictionary *)dictonary withFile:(NSString *)file allLines:(NSMutableArray *)allLines
{
    NSError    *error;
    NSScanner  *pScanner;
    NSString   *line;
    UInt32      addr;
    NSString   *fileContents;
    NSArray    *allLinedStrings;
    NSRange     range;
    NSString   *key;
    NSNumber   *addr_nbr;
    
    error           = [[NSError alloc] init];
    fileContents    = [NSString stringWithContentsOfFile:file encoding:NSASCIIStringEncoding error:&error];
    allLinedStrings = [fileContents componentsSeparatedByCharactersInSet: [NSCharacterSet newlineCharacterSet]];
    
    range.length   = 4;
    range.location = 0;
    
    for (int ix = 0 ; ix < [allLinedStrings count] ; ix++) {
        line = [allLinedStrings objectAtIndex:ix];
        
        if ([line length] > 0) {
            [allLines addObject:line];
        }
        
        if ([line length] < 5) {
            continue;
        }
        
        if ([line characterAtIndex:4] == ':') {
            pScanner = [NSScanner scannerWithString: line];
            [pScanner scanHexInt:&addr];
            addr_nbr = [[NSNumber alloc] initWithInt:addr];
            key = [addr_nbr stringValue];
            
            [dictonary setObject:line forKey:key];
        }
    }
}

-(void)loadCodeTXT:(NSMutableDictionary *)dictonary withFile:(NSString *)file allLines:(NSMutableArray *)allLines
{
    NSError    *error;
    NSScanner  *pScanner;
    NSString   *line;
    UInt32      addr;
    NSString   *fileContents;
    NSArray    *allLinedStrings;
    NSRange     range;
    NSString   *key;
    NSNumber   *addr_nbr;
    
    error           = [[NSError alloc] init];
    fileContents    = [NSString stringWithContentsOfFile:file encoding:NSASCIIStringEncoding error:&error];
    allLinedStrings = [fileContents componentsSeparatedByCharactersInSet: [NSCharacterSet newlineCharacterSet]];
    
    range.length   = 4;
    range.location = 2;
    
    for (int ix = 0 ; ix < [allLinedStrings count] ; ix++) {
        line = [allLinedStrings objectAtIndex:ix];
        
        [allLines addObject:line];
        
        if ([line length] < 5) {
            continue;
        }
        
        if (([line characterAtIndex:0] == ' ') && ([line characterAtIndex:1] == ' ') && ([line characterAtIndex:2] != ' ')) {
            pScanner = [NSScanner scannerWithString: line];
            [pScanner scanHexInt:&addr];
            addr_nbr = [[NSNumber alloc] initWithInt:addr];
            key = [addr_nbr stringValue];
            
            [dictonary setObject:line forKey:key];
        }
    }
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
            return [p_code_all count];
            
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
            
#if 1     
            
        case TABLE_VIEW_CODE_LISTING_TAG:
            return [p_code_all objectAtIndex:rowIndex];
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

- (void)tableView:(NSTableView *)aTableView willDisplayCell:(id)aCell forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    [aCell setFont:[NSFont fontWithName:@"Menlo" size:10.0]];
}

@end
