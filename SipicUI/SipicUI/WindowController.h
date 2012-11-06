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

- (IBAction)sim_step:(id)sender;

@end
