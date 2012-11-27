//
//  WindowController.m
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#import "WindowController.h"

@interface WindowController ()

@end

@implementation WindowController

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        init_sipic();
        
//        p_tvc = [[TableViewsController alloc] init];
        
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}

- (NSString *) windowTitleForDocumentDisplayName:(NSString *)displayName
{
    return @"The Window Title";
}

- (IBAction)sim_step:(id)sender
{
    Sim_Step();
    [_p_mem_view1 reloadData];
    [_p_mem_view2 reloadData];
    [_p_call_stack reloadData];
    [_p_code_listing reloadData];
}

@end
