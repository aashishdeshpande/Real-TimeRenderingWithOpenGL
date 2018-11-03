//
//  MyView.m
//  Window
//
//

#import "MyView.h"

@implementation MyView
{
    NSString *centralText;
}


-(id)initWithFrame:(CGRect)frameRect
{
    self=[super initWithFrame:frameRect];
    if(self)
    {
        //initialize code her
        
        //set scene's background color
        [self setBackgroundColor:[UIColor whiteColor]];
        
        centralText=@"Hello World !!!";
        
        //Gesture Recognition
        //Tap gesture code
        UITapGestureRecognizer *singleTapGestureRecoginzer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onSingleTap:)];
        
        [singleTapGestureRecoginzer setNumberOfTapsRequired:1];
        [singleTapGestureRecoginzer setNumberOfTouchesRequired:1];
        [singleTapGestureRecoginzer setDelegate:self];
        [self addGestureRecognizer:singleTapGestureRecoginzer];
        
        UITapGestureRecognizer *doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onDoubleTap:)];
        
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [doubleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:doubleTapGestureRecognizer];
        
        //this will allow to differentiate between single tap and double tap
        [singleTapGestureRecoginzer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        //swipe gesture
        UISwipeGestureRecognizer *swipeGestureRecognizer = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(onSwipe:)];
        [self addGestureRecognizer:swipeGestureRecognizer];
        
        //long press gesture
        UILongPressGestureRecognizer *longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(onLongPress:)];
        [self addGestureRecognizer:longPressGestureRecognizer];
    }
    return(self);
}

-(void)drawRect:(CGRect)rect
{
    //black background
    UIColor *fillColor = [UIColor blackColor];
    [fillColor set];
    UIRectFill(rect);
    
    //dictionary with kvc
    NSDictionary *dictionaryForTextAttributes = [NSDictionary dictionaryWithObjectsAndKeys:[UIFont fontWithName:@"Helvetica" size:24], NSFontAttributeName, [UIColor greenColor], NSForegroundColorAttributeName, nil];
    
    CGSize textSize = [centralText sizeWithAttributes:dictionaryForTextAttributes];
    
    CGPoint point;
    point.x = (rect.size.width/2)-(textSize.width/2);
    point.y = (rect.size.height/2)-(textSize.height/2)+12;
    
    [centralText drawAtPoint:point withAttributes:dictionaryForTextAttributes];
}

//to become first responder
-(BOOL)acceptsFirstResponder
{
    return(YES);
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    /*centralText = @"'touchesBegan' Event Occured";
     [self setNeedsDisplay];*/
}

-(void)onSingleTap:(UITapGestureRecognizer *)gr
{
    centralText = @"'onSingleTap' Event Occured";
    [self setNeedsDisplay];
}

-(void)onDoubleTap:(UITapGestureRecognizer *)gr
{
    centralText = @"'onDoubleTap' Event Occured";
    [self setNeedsDisplay];
}

-(void)onSwipe:(UISwipeGestureRecognizer *)gr
{
    [self release];
    exit(0);
}

-(void)onLongPress:(UILongPressGestureRecognizer *)gr
{
    centralText = @"'onLongPress' Event Occured";
    [self setNeedsDisplay];
}
-(void)dealloc
{
    [super dealloc];
}

@end
