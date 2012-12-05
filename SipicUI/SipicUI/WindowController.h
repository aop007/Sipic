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


@interface WindowController : NSWindowController
{
//    TableViewsController  *p_tvc;
    SimulationController  *p_sim_ctrl;
    bool                   kill_thread;
    NSThread              *p_run_thread;
}

@property (assign) IBOutlet NSTableView *p_mem_view1;
@property (assign) IBOutlet NSTableView *p_mem_view2;
@property (assign) IBOutlet NSTableView *p_call_stack;
@property (assign) IBOutlet NSTableView *p_code_listing;
@property (assign) IBOutlet NSTextField *p_pc_bp;

- (IBAction)sim_step:(id)sender;
- (IBAction)sim_run_pause:(id)sender;
- (void    )Thread_Run;
@end
