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

@synthesize p_table    = _p_table;
@synthesize p_win_ctrl = _p_win_ctrl;

-(id)init
{
    CPU_INT32U  index;
    
    self = [super init];
    if (self) {
        p_symbols = [[ProjectSymbols alloc] init];
        p_code_listing = [[NSMutableDictionary alloc] init];
        p_code_all     = [[NSMutableArray alloc] init];
        p_last_click   = [[NSDate alloc] initWithTimeIntervalSinceNow:0];
        
        index = 0;
        
        [self loadCode:p_code_listing withFile:@"/Users/aop007/Documents/Projets/DawnStar/Sipic/Sipic/Sipic/InputFiles/main.lst" allLines:p_code_all index:&index method:0];
        [self loadCode:p_code_listing withFile:@"/Users/aop007/Documents/Projets/DawnStar/Sipic/Sipic/Sipic/InputFiles/main.txt" allLines:p_code_all index:&index method:1];
        

    }
    
    return self;

}

-(void)loadCode:(NSMutableDictionary *)dictonary withFile:(NSString *)file allLines:(NSMutableArray *)allLines index:(CPU_INT32U *)p_index method:(int)method
{
    NSError            *error;
    NSScanner          *pScanner;
    NSString           *line;
    UInt32              addr;
    NSString           *fileContents;
    NSArray            *allLinedStrings;
    NSRange             range;
    NSString           *key;
    NSNumber           *addr_nbr;
    int                 index;
    CodeLineElement    *cle;
    CodeLineElement    *current_key;
    
    index           = *p_index;
    error           = [[NSError alloc] init];
    fileContents    = [NSString stringWithContentsOfFile:file encoding:NSASCIIStringEncoding error:&error];
    allLinedStrings = [fileContents componentsSeparatedByCharactersInSet: [NSCharacterSet newlineCharacterSet]];
    
    range.length   = 4;
    range.location = 0;
    
    for (int ix = 0 ; ix < [allLinedStrings count] ; ix++) {
        line = [allLinedStrings objectAtIndex:ix];
        
        if ([line length] > 0) {
            
            index++;
        } else {
            continue;
        }

        cle = [[CodeLineElement alloc] init];
        
        cle.index     = index - 1;
        cle.line      = [line retain];
        cle.breakOnPC = false;
        cle.isCode    = false;
        cle.addr      = 0;
        
        [allLines addObject:cle];
        
        if ([line length] < 5) {
            continue;
        }

        if (((method == 0) && ([line characterAtIndex:4] == ':')) ||                                                                                /* LST */
            ((method == 1) && ([line characterAtIndex:0] == ' ') && ([line characterAtIndex:1] == ' ') && ([line characterAtIndex:2] != ' '))) {    /* TXT */
            
            pScanner = [NSScanner scannerWithString: line];
            [pScanner scanHexInt:&addr];
            addr_nbr = [[NSNumber alloc] initWithInt:addr];
            key = [addr_nbr stringValue];
            
            cle.isCode = true;
            cle.addr   = addr;
            
            current_key = [dictonary objectForKey:key];
            
            if (current_key == nil) {
                [dictonary setObject:cle forKey:key];
            }
        }
    }
    
    *p_index = index;
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
    NSString         *returning;  
    
    switch ([aTableView tag]) {
        case TABLE_VIEW_MEM_VIEW1_TAG:
        case TABLE_VIEW_MEM_VIEW2_TAG:
            returning = [self GetDataMemoryTable:aTableColumn row:rowIndex];
            return returning;
            

        case TABLE_VIEW_CALL_STACK_TAG:
            returning = [self GetCallStackTable:aTableColumn row:rowIndex];
            return returning;
            
            
        case TABLE_VIEW_CODE_LISTING_TAG:
            returning = [self GetCodeTable:aTableColumn row:rowIndex];
            return returning;
            

        default:
            return @"";
    }
}


- (NSString *)GetCallStackTable:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    NSNumber  *number;
    NSString  *value;
    
    value = [aTableColumn identifier];
    
    if ([[aTableColumn identifier] isEqualToString:@"Addr"]) {
        number = [[NSNumber alloc]initWithInt:Sim_AddrForDepth((CPU_INT32U)rowIndex)];
        value  = [NSString stringWithFormat:@"%06X", [number intValue]];
    } else {
        
        value = [p_symbols getSymbolAtAddr:Sim_AddrForDepth((CPU_INT32U)rowIndex)];
    }
    
    return value;
}

- (NSString *)GetCodeTable:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    NSNumber         *number;
    NSString         *value;
    CodeLineElement  *cle;

    
    value = [aTableColumn identifier];
    
    if ([[aTableColumn identifier] isEqualToString:@"Line"]) {
        number = [[NSNumber alloc]initWithInt:rowIndex];
        value  = [number stringValue];
    } else {
        cle = [p_code_all objectAtIndex:rowIndex];
        if (cle.line != nil) {
            return cle.line;
        } else {
            return @"";
        }
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
    NSCell          *cell;
    CodeLineElement *cle;
    
    cle  = [p_code_all objectAtIndex:rowIndex];
    cell = (NSCell *)aCell;
    
    if (cle.breakOnPC == true) {
        [aCell setTextColor:[NSColor redColor]];
        //[cell setBackgroundColor:[NSColor redColor]];
        //[cell setForegroundColor:[NSColor whiteColor]];
    } else {
        [aCell setTextColor:[NSColor blackColor]];
    }
    [aCell setFont:[NSFont fontWithName:@"Menlo" size:10.0]];
}

- (void)doubleClick
{
    NSLog(@"DoubleClick");
}

-(IBAction)tableAction:(id)sender
{
    NSTimeInterval     time_int;
    NSTableView       *p_tblview;
    CodeLineElement   *cle;
    WindowController  *wc;
    
    time_int = -[p_last_click timeIntervalSinceNow];
    wc = (WindowController *)_p_win_ctrl;
    
    if (time_int > MAX_DCLICK_TIME) {
        p_last_click = [p_last_click initWithTimeIntervalSinceNow:0];       // No double click. Return.
        return;
    } else {
        p_last_click = [p_last_click initWithTimeIntervalSinceNow:-1];
    }
    
    p_tblview = (NSTableView *)sender;
    
    //NSLog(@"table action from %@ at row %lu %f",[p_tblview identifier], [p_tblview clickedRow], time_int);
    
    /* If code, toggle break on PC property. */
    cle = [p_code_all objectAtIndex:[p_tblview clickedRow]];
    
    cle.breakOnPC = !cle.breakOnPC;
    
    /* Add or remove from break list. */
    if (cle.breakOnPC == true) {
        NSLog(@"Add %x:%@ to breaklist", cle.addr, cle.line);
        [wc AddBreakPoint:cle];
    } else {
        NSLog(@"Remove %x:%@ to breaklist", cle.addr, cle.line);
    }
    //[_p_win_ctrl UpdateWindowControls];
}

- (BOOL)tableView:(NSTableView *)aTableView shouldEditTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    return false;
}

-(NSInteger)RowForPC:(NSInteger)pc
{
    NSNumber         *pc_val;
    CodeLineElement  *cle;
    NSString         *key;
    
    
    pc_val = [[NSNumber alloc] initWithInt:(int)pc];
    key    = [pc_val stringValue];
    
    cle = [p_code_listing objectForKey:key];
 
    return cle.index;
}

@end
