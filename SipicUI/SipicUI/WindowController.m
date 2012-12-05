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
        
 //       run();
//        p_tvc = [[TableViewsController alloc] init];
        kill_thread = true;
        p_run_thread = [NSThread alloc];
        
        //[_p_code_listing setDoubleAction:@selector(doubleClick:)];
        //[_p_code_listing setTarget:self];
        
        bpl = NULL;
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

- (IBAction)sim_run_pause:(id)sender
{
    NSButton   *p_button;
    UInt32      addr;
    NSString   *memoryBPstring;
    NSScanner  *pScanner;
    NSString   *btnName;
    
    
    memoryBPstring = [_p_pc_bp stringValue];
    pScanner       = [NSScanner scannerWithString:memoryBPstring];
    
    p_button = (NSButton *)sender;
    btnName  = [p_button title];
    
    if ([btnName compare:@"Run"] == NSOrderedSame) {
        [p_button setTitle:@"Stop"];
        
        [p_run_thread initWithTarget:self selector:@selector(Thread_Run) object:nil];
        [p_run_thread start];
    } else {
        
        kill_thread = true;
        [p_button setTitle:@"Run"];
        [_p_mem_view1    reloadData];
        [_p_mem_view2    reloadData];
        [_p_call_stack   reloadData];
        [_p_code_listing reloadData];
        //[p_run_thread    cancel];
    }
}

-(void)Thread_Run
{
    CPU_INT32U         PC;
    BREAK_POINT_LIST  *listHead;
    
    
    kill_thread = false;
    
    while (kill_thread == false) {
        Sim_Step();
        
        PC       = (Sim_GetValueFromDataMem(0x0030) & 0xFF) << 16 | Sim_GetValueFromDataMem(0x002E);
        listHead = bpl;
        
        
        while (listHead != NULL) {
            if (listHead->cle.addr == PC) {
                kill_thread = true;
                break;
            } else {
                listHead = listHead->next;
            }
        }
    }
    
    //[self sim_run_pause:_p_start_stop];
    
    NSLog(@"Found BP");
    
    [_p_start_stop   setTitle:@"Run"];
    [_p_mem_view1    reloadData];
    [_p_mem_view2    reloadData];
    [_p_call_stack   reloadData];
    [_p_code_listing reloadData];
    [p_run_thread    cancel];
}

- (void    )AddBreakPoint:   (CodeLineElement *)cle
{
    BREAK_POINT_LIST  *new_bpl;
    
    
    new_bpl = malloc(sizeof(BREAK_POINT_LIST));
    
    if (bpl == NULL) {
        bpl       = new_bpl;
        bpl->cle  = cle;
        bpl->next = NULL;
    } else {
        new_bpl->next = bpl;
        new_bpl->cle  = cle;
        bpl           = new_bpl;
    }
}

- (void    )RemoveBreakPoint:(CodeLineElement *)cle
{
    NSLog(@"Remove!");
}

@end
