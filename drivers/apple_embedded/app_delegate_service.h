#pragma once

#import <UIKit/UIKit.h>

@class GDTViewController;

@interface GDTAppDelegateService : NSObject <UIApplicationDelegate>

@property(strong, nonatomic) UIWindow *window;
@property(strong, class, readonly, nonatomic) GDTViewController *viewController;

@end
