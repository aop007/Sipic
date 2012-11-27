//
//  TableViewsController.h
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-26.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ProjectSymbols.h"


#define  TABLE_VIEW_MEM_VIEW1_TAG       0u
#define  TABLE_VIEW_MEM_VIEW2_TAG       1u
#define  TABLE_VIEW_CALL_STACK_TAG      2u
#define  TABLE_VIEW_CODE_LISTING_TAG    3u

#define  MEM_VIEW_SIZE_BYTE             0x0C00
#define  MEM_VIEW_SIZE_WORD             (MEM_VIEW_SIZE_BYTE / 2)

@interface TableViewsController : NSObject <NSTableViewDataSource>
{
    ProjectSymbols        *p_symbols;
    NSArray               *p_code_listing;
}


@end
