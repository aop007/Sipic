//
//  ProjectSymbols.m
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-26.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#import "ProjectSymbols.h"
#import "SymbolAddrName.h"

@implementation ProjectSymbols

-(id)init
{
    self = [super init];
    if (self) {
        NSMutableArray *sortingArray = [[NSMutableArray alloc] init];
        int ix;
        int rom_start_ix;
        
        
        NSString* fileContents = [NSString stringWithContentsOfFile:@"/Users/aop007/Documents/Projets/DawnStar/Sipic/Sipic/Sipic/InputFiles/main.sym"
                                                           encoding:NSUTF8StringEncoding error:nil];
        
        NSArray* allLinedStrings = [fileContents componentsSeparatedByCharactersInSet: [NSCharacterSet newlineCharacterSet]];
        
        ix           =  0;
        rom_start_ix = -1;
        
        while(ix < [allLinedStrings count]) {
            if ([[allLinedStrings objectAtIndex:ix] isEqualToString:@"ROM Allocation:"]) {
                rom_start_ix = ix;
                ix++;
                continue;
            }
            
            if (rom_start_ix != -1) {
                
                if ([[allLinedStrings objectAtIndex:ix] length] > 0) {
                    NSScanner  *pScanner = [NSScanner scannerWithString: [allLinedStrings objectAtIndex:ix]];
                    UInt        addr;
                    NSString   *fnct_name;
                    NSNumber    *number;
                    
                    [pScanner scanHexInt:&addr];
                    [pScanner setScanLocation:[pScanner scanLocation] + 2];
                    fnct_name = [[pScanner string] substringFromIndex:[pScanner scanLocation]];
                    
                    number    = [[NSNumber alloc] initWithInt:addr];
                    
                    SymbolAddrName *sym = [[SymbolAddrName alloc] initWithAddr:addr andName:fnct_name];
                    [sortingArray addObject:sym];
                }
            }
            
            if ((rom_start_ix != -1) && ([[allLinedStrings objectAtIndex:ix] isEqualToString:@"User Memory space: "])) {
                break;
            }
            
            ix++;
        }
        
        symbolList = [sortingArray sortedArrayUsingSelector:@selector(compare:)];
        
#if 1
        for (ix = 0 ; ix < [symbolList count] ; ix++) {
            SymbolAddrName *psym = [symbolList objectAtIndex:ix];
            
            NSLog(@"%X\t%@",[psym.addr intValue], psym.fnctName);
        }
#endif
    }
    
    return self;
    
}

-(NSString *)getSymbolAtAddr:(int)addr
{
    int lo_range = 0;
    int hi_range = (int)[symbolList count] - 1;
    int test_range = hi_range / 2;
    SymbolAddrName  *p_test_sym;

    while ((hi_range - lo_range) > 1) {
        p_test_sym = [symbolList objectAtIndex:test_range];
        
        if (addr < [p_test_sym.addr intValue]) {
            hi_range = test_range;
        } else {
            lo_range = test_range;
        }
        
        test_range = (lo_range + hi_range) / 2;
    }
    
    NSLog(@"%X\t%@",[p_test_sym.addr intValue], p_test_sym.fnctName);
    
    return p_test_sym.fnctName;
}

@end
