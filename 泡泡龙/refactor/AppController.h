#import <UIKit/UIKit.h>
#import "Chartboost.h"

@class RootViewController;
@class GKLocalPlayer;
@interface AppController : NSObject <UIApplicationDelegate,ChartboostDelegate> {
    UIWindow *window;
}

@property(nonatomic, retain) RootViewController* viewController;
@property(nonatomic,retain) GKLocalPlayer * player;
@property (nonatomic,retain) UIButton * baseButton;




@end

