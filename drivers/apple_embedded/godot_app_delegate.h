#pragma once

#import <UIKit/UIKit.h>

typedef NSObject<UIApplicationDelegate> GDTAppDelegateServiceProtocol;

@interface GDTApplicationDelegate : NSObject <UIApplicationDelegate>

@property(class, readonly, strong) NSArray<GDTAppDelegateServiceProtocol *> *services;

+ (void)addService:(GDTAppDelegateServiceProtocol *)service;

@end
