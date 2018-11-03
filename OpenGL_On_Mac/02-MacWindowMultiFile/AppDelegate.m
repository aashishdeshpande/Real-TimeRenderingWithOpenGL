#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import "AppDelegate.h"
#import "MyView.h"
@implementation AppDelegate
{
@private
	NSWindow *window;
	MyView *view;
}
-(void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	NSRect win_rect;
	win_rect = NSMakeRect(0.0, 0.0, 800.0, 600.0);

	window = [[NSWindow alloc] initWithContentRect : win_rect styleMask : NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable backing : NSBackingStoreBuffered defer : NO];

	[window setTitle : @"macOS Window"];
	[window center];

	view = [[MyView alloc]initWithFrame : win_rect];
	[window setContentView : view];
	[window setDelegate : self];
	[window makeKeyAndOrderFront : self];
}

-(void)applicationWillTerminate : (NSNotification *)notification
{
	//code
}

-(void)windowWillClose : (NSNotification *)notification
{
	//code
	[NSApp terminate : self];
}

-(void)dealloc
{
	//code
	[view release];
	[window release];
	[super dealloc];
}
@end
