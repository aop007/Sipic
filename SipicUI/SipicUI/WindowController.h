//
//  WindowController.h
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "SimulationController.h"

@interface WindowController : NSWindowController
{
    SimulationController* p_sim_ctrl;
}

@property (assign) IBOutlet NSTableView *p_mem_view1;
@property (assign) IBOutlet NSTableView *p_mem_view2;
@property (assign) IBOutlet NSTableView *p_call_stack;
@property (assign) IBOutlet NSTableView *p_code_listing;

- (IBAction)sim_step:(id)sender;

@end
