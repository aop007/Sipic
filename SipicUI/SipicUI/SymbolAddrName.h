//
//  SymbolAddrName.h
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-26.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SymbolAddrName : NSObject
{

}

@property NSNumber *addr;
@property NSString *fnctName;

-(id)initWithAddr:(int)address andName:(NSString *)name;



@end
