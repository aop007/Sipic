//
//  TableViewsController.h
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-26.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ProjectSymbols.h"
#import "CodeLineElement.h"
#import "cpu.h"
#import "WindowController.h"


#define  TABLE_VIEW_MEM_VIEW1_TAG       0u
#define  TABLE_VIEW_MEM_VIEW2_TAG       1u
#define  TABLE_VIEW_CALL_STACK_TAG      2u
#define  TABLE_VIEW_CODE_LISTING_TAG    3u

#define  MEM_VIEW_SIZE_BYTE             0x0C00
#define  MEM_VIEW_SIZE_WORD             (MEM_VIEW_SIZE_BYTE / 2)

#define  MAX_DCLICK_TIME                0.25

@interface TableViewsController : NSObject <NSTableViewDataSource, NSTableViewDelegate>
{
    ProjectSymbols       *p_symbols;
    NSMutableDictionary  *p_code_listing;
    NSMutableArray       *p_code_all;
    NSDate               *p_last_click;
}

typedef  struct  code_line_element{
    NSString    *line;
    CPU_INT32U   index;
} CODE_LINE_ELEMENT;

@property (assign) IBOutlet NSTableView         *p_table;

@property (assign) IBOutlet NSWindowController  *p_win_ctrl;

-(IBAction)tableAction:(id)sender;

-(NSInteger)RowForPC:(NSInteger)pc;
@end
