//
//  SymbolAddrName.m
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-26.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#import "SymbolAddrName.h"

@implementation SymbolAddrName

@synthesize addr     = _addr;
@synthesize fnctName = _fnctName;


-(id)initWithAddr:(int)address andName:(NSString *)name
{
    self.addr     = [[NSNumber alloc] initWithInt:address];
    self.fnctName = [name retain];
    
    return self;
}

- (NSComparisonResult)compare:(SymbolAddrName *)otherObject {
    return [self.addr compare:otherObject.addr];
}

@end
