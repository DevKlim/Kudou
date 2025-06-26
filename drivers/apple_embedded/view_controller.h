#pragma once

#import <UIKit/UIKit.h>

@class GDTView;
@class GDTKeyboardInputView;

@interface GDTViewController : UIViewController

@property(nonatomic, readonly, strong) GDTView *godotView;
@property(nonatomic, readonly, strong) GDTKeyboardInputView *keyboardView;

@end
