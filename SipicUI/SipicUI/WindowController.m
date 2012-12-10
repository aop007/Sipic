//
//  WindowController.m
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#import "WindowController.h"
#import "sim.h"

@interface WindowController ()

@end

@implementation WindowController

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self) {
        init_sipic();

        kill_thread         = true;
        p_run_thread        = [NSThread alloc];
        bpl                 = NULL;
        breakPointSignalled = false;
        
        tableViewController = [[TableViewsController alloc] init];
        
        [self loadBPLfromFile:@"/Users/aop007/Documents/Projets/DawnStar/Sipic/Sipic/Sipic/InputFiles/breakpointfile.txt"];
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
    [self UpdateWindowControls];
}

- (IBAction)sim_run_pause:(id)sender
{
    NSButton   *p_button;
    //UInt32      addr;
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
        [self UpdateWindowControls];
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
    [self UpdateWindowControls];
    [p_run_thread    cancel];
}

- (void    )AddBreakPoint:   (CodeLineElement *)cle
{
    BREAK_POINT_LIST  *new_bpl;
    
    
    new_bpl = malloc(sizeof(BREAK_POINT_LIST));
    
    if (bpl == NULL) {
        bpl           = new_bpl;
        bpl->cle      = cle;
        bpl->next     = NULL;
    } else {
        new_bpl->next = bpl;
        new_bpl->cle  = cle;
        bpl           = new_bpl;
    }
}

- (void)RemoveBreakPoint:(CodeLineElement *)cle
{
    BREAK_POINT_LIST  *prev_bpl;
    BREAK_POINT_LIST  *remove_bpl;
    
    prev_bpl   = nil;
    remove_bpl = bpl;
    
    while ((remove_bpl->cle != cle) && (remove_bpl != nil)) {
        
        prev_bpl   = remove_bpl;
        remove_bpl = remove_bpl->next;
    }
    
    if (remove_bpl != nil) {
        if (prev_bpl == nil) {      /* Breakpoint is head */
            bpl = remove_bpl->next;
        } else {                    /* Breakpoint is in the middle of the list. */
            prev_bpl->next = remove_bpl->next;
        }
    }
    
    free(remove_bpl);               /* Free the breakpoint. */
}

-(void)UpdateWindowControls
{
    CPU_INT32U             PC;
    TableViewsController  *tvc;
    CodeLineElement       *cle;
    
    tvc = (TableViewsController *)tableViewController;
    PC  = (Sim_GetValueFromDataMem(0x0030) & 0xFF) << 16 | Sim_GetValueFromDataMem(0x002E);
    cle = [tvc CleForPC:PC];
    
    /* Scroll to next Line of Execution. */
    [_p_code_listing scrollRowToVisible:cle.index];
    
    [_p_mem_view1    reloadData];
    [_p_mem_view2    reloadData];
    [_p_call_stack   reloadData];
    [_p_code_listing reloadData];
}

- (void)windowWillClose:(NSNotification *)notification
{
    BREAK_POINT_LIST  *listHead;
    NSOutputStream    *stream;
    NSData            *data;
    NSNumber          *number;
    NSFileManager     *fileManager;
    NSError           *error;
    
    error = [NSError alloc];
    
    fileManager = [NSFileManager defaultManager];
    
    [fileManager removeItemAtPath:@"/Users/aop007/Documents/Projets/DawnStar/Sipic/Sipic/Sipic/InputFiles/breakpointfile.txt" error:&error];
    
    NSLog(@"Window willClose");
    
    stream = [[NSOutputStream alloc] initToFileAtPath:@"/Users/aop007/Documents/Projets/DawnStar/Sipic/Sipic/Sipic/InputFiles/breakpointfile.txt" append:YES];

    [stream open];
    
    listHead = bpl;
    
    while (listHead != NULL) {
        number = [[NSNumber alloc] initWithInt:listHead->cle.addr];
        data   = [[number stringValue] dataUsingEncoding:NSUTF8StringEncoding];
        [stream write:data.bytes maxLength:data.length];
        
        data   = [@"\r\n" dataUsingEncoding:NSUTF8StringEncoding];
        [stream write:data.bytes maxLength:data.length];
        

        listHead = listHead->next;
    }
    
    [stream close];
}

-(void)loadBPLfromFile:(NSString *)path
{
    NSString              *line;
    CPU_INT32U             ix;
    BREAK_POINT_LIST      *new_bpl;
    CodeLineElement       *cle;
    TableViewsController  *tvc;
    NSScanner             *pScanner;
    int                    addrVal;
    
    
    NSString* fileContents = [NSString stringWithContentsOfFile:path
                                                       encoding:NSUTF8StringEncoding error:nil];
    
    NSArray* allLinedStrings = [fileContents componentsSeparatedByCharactersInSet: [NSCharacterSet newlineCharacterSet]];
    
    tvc = (TableViewsController *)tableViewController;
    
    for (ix = 0 ; ix < [allLinedStrings count] ; ix++) {
        line = [allLinedStrings objectAtIndex:ix];
        if ([line length] > 0) {
            /* Find CLE for addr */
            pScanner = [NSScanner scannerWithString: line];
            [pScanner scanInt:&addrVal];
            
            cle           = [tvc CleForPC:addrVal];
            cle.breakOnPC = true;
            
            /* Add CLE to break point list */
            new_bpl = malloc(sizeof(BREAK_POINT_LIST));
            
            if (bpl == NULL) {
                bpl           = new_bpl;
                bpl->cle      = cle;
                bpl->next     = NULL;
            } else {
                new_bpl->next = bpl;
                new_bpl->cle  = cle;
                bpl           = new_bpl;
            }
        }
    }
}

/* Delegate Methods */
- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    return [((TableViewsController *)tableViewController) numberOfRowsInTableView:aTableView];
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    return [((TableViewsController *)tableViewController) tableView:aTableView objectValueForTableColumn:aTableColumn row:rowIndex];
}

-(IBAction)tableAction:(id)sender
{
    CodeLineElement *cle;
    
    cle = [((TableViewsController *)tableViewController) tableAction:sender];
    
    if (cle != nil) {
        if (cle.breakOnPC == true) {
            [self AddBreakPoint:cle];
        } else {
            [self RemoveBreakPoint:cle];
        }
    }
}

- (BOOL)tableView:(NSTableView *)aTableView shouldEditTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    return [((TableViewsController *)tableViewController) tableView:aTableView shouldEditTableColumn:aTableColumn row:rowIndex];
}

- (void)tableView:(NSTableView *)aTableView willDisplayCell:(id)aCell forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    [((TableViewsController *)tableViewController) tableView:aTableView willDisplayCell:aCell forTableColumn:aTableColumn row:rowIndex];
}

@end
