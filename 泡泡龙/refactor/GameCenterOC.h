#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>
#import <UIKit/UIKit.h>

NSString * str;


@interface GameCenterOC :  NSObject <GKLeaderboardViewControllerDelegate>{
    BOOL userAuthenticated;
}

@property (assign, readonly) BOOL gameCenterAvailable;

+(id) getInstance;

+(void) testLog;
+(void) testLogWithStr:(NSString*)_str;
+(void) hMessageBox:(NSString*)pszMsg title:(NSString*)pszTitle;
-(void) openLeaderboard;
-(void) submitscore:(float)_score;
@end