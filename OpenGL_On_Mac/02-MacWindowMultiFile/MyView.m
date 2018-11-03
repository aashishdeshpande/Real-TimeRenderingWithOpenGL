#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import "AppDelegate.h"
#import "MyView.h"
@implementation MyView
{
	NSString *centralText;
}

-(id)initWithFrame : (NSRect)frame;
{
	//code
	self = [super initWithFrame : frame];

	if(self)
	{
		[[self window]setContentView : self];
		centralText = @"Hello World !!!";
	}
	return(self);
}

-(void)drawRect : (NSRect)dirtyRect
{
	//code
	//black background
	NSColor *fillColor = [NSColor blackColor];
	[fillColor set];
	NSRectFill(dirtyRect);

	//dictionary with kvc
	NSDictionary *dictionaryForTextAttributes = [NSDictionary dictionaryWithObjectsAndKeys : [NSFont fontWithName : @"HElvetica" size : 32], NSFontAttributeName, [NSColor greenColor], NSForegroundColorAttributeName, nil];

	NSSize textSize = [centralText sizeWithAttributes : dictionaryForTextAttributes];

	NSPoint point;
	point.x = (dirtyRect.size.width/2)-(textSize.width/2);
	point.y = (dirtyRect.size.height/2)-(textSize.height/2)+12;

	[centralText drawAtPoint : point withAttributes : dictionaryForTextAttributes];  
}

-(BOOL)acceptsFirstResponder
{
	[[self window]makeFirstResponder : self];
	return(YES);
}

-(void)keyDown : (NSEvent *)theEvent
{
	//code

	int key = (int)[[theEvent characters]characterAtIndex : 0];
	switch(key)
	{
		case 27:
		[self release];
		[NSApp terminate : self];
		break;

		case 'F':
		case 'f':
		centralText = @"'F' or 'f' Key Is Pressed";
		[[self window]toggleFullScreen : self];
		break;

		default:
		break;
	}
}

/*-(void)keyDown:(NSEvent *)theEvent
{
    // code
    int key=(int)[[theEvent characters]characterAtIndex:0];
    switch(key)
    {
        case 27: // Esc key
            [ self release];
            [NSApp terminate:self];
            break;
        case 'F':
        case 'f':
            centralText=@"'F' or 'f' Key Is Pressed";
            [[self window]toggleFullScreen:self]; // repainting occurs automatically
            break;
        default:
            break;
    }
}*/

-(void)mouseDown : (NSEvent *)theEvent
{
	centralText = @"Left Mouse Button Is Clicked";
	[self setNeedsDisplay : YES];
}

-(void)mouseDragged : (NSEvent *)theEvent
{
	//code
}

-(void)rightMouseDown : (NSEvent *)theEvent
{
	//code
	centralText = @"Right Mouse Button Is Clicked";
	[self setNeedsDisplay : YES];
}

-(void)dealloc
{
	//code
	[super dealloc];
}
@end