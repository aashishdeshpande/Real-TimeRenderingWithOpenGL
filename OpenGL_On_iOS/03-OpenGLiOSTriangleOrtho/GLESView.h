//
//  GLESView.h
//  OrthoTriangle
//
//  Created by Ashish Khare on 04/07/18.
//

#import <UIKit/UIKit.h>

@interface GLESView : UIView<UIGestureRecognizerDelegate>

-(void)startAnimation;
-(void)stopAnimation;

@end
