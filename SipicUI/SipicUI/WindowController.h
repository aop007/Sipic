//
//  WindowController.h
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "SimulationController.h"
#import "TableViewsController.h"
#import "CodeLineElement.h"


@interface WindowController : NSWindowController <NSTableViewDataSource, NSTableViewDelegate>
{
    //TableViewsController  *p_tvc;
    SimulationController           *p_sim_ctrl;
    bool                            kill_thread;
    NSThread                       *p_run_thread;
    BREAK_POINT_LIST               *bpl;
    bool                            breakPointSignalled;
    NSObject                       *tableViewController;
}

@property (assign) IBOutlet NSTableView *p_mem_view1;
@property (assign) IBOutlet NSTableView *p_mem_view2;
@property (assign) IBOutlet NSTableView *p_call_stack;
@property (assign) IBOutlet NSTableView *p_code_listing;

@property (assign) IBOutlet NSScrollView *p_mem_view1_scroll;
@property (assign) IBOutlet NSScrollView *p_mem_view2_scroll;
@property (assign) IBOutlet NSScrollView *p_call_stack_scroll;
@property (assign) IBOutlet NSScrollView *p_code_listing_scroll;

@property (assign) IBOutlet NSTextField *p_pc_bp;

@property (assign) IBOutlet NSButton    *p_start_stop;

//@property (assign) IBOutlet NSObject    *p_data_source;

- (IBAction)sim_step:(id)sender;
- (IBAction)sim_run_pause:(id)sender;
- (IBAction)tableAction:(id)sender;

- (void    )Thread_Run;
- (void    )AddBreakPoint:   (CodeLineElement *)cle;
- (void    )RemoveBreakPoint:(CodeLineElement *)cle;
- (void    )UpdateWindowControls;
@end
