#import "GameCenterOC.h"
#import "AppController.h"
#import "PlayerModel.h"

static GameCenterOC * instance;

@implementation GameCenterOC

@synthesize gameCenterAvailable;
 
+(void) testLog{
    str = @"Himi->string is: ";
    NSLog(@"HSprite: testLog"); 
}
 
+(void) testLogWithStr:(NSString*)_str{
    str = [NSString stringWithFormat:@"%@%@",str,_str];
    NSLog(@"%@",str); 
}
 
 
+(void) hMessageBox:(NSString*)pszMsg title:(NSString*)pszTitle{
     
    UIAlertView * messageBox = [[UIAlertView alloc] initWithTitle: pszTitle
                                                          message: pszMsg
                                                         delegate: nil
                                                cancelButtonTitle: @"OK"
                                                otherButtonTitles: nil];
    [messageBox autorelease];
    [messageBox show];
}

+(id) getInstance{
    if (instance == nil) {
        instance = [[GameCenterOC alloc] init];
    }
    return instance;
}

-(void) submitscore:(float)_score
{
    GKScore * submitScore = [[GKScore alloc] initWithCategory:@"LBTopScore"];
    [submitScore setValue:_score];
    [submitScore setShouldSetDefaultLeaderboard:YES];
    [submitScore setContext:_score];
    PlayerModel *pm = [[[PlayerModel alloc] init] autorelease];
    [pm submitScore:submitScore];
    [submitScore release];
}
-(void) openLeaderboard
{
    GKLeaderboardViewController *leaderboardController = [[GKLeaderboardViewController alloc] init];
//    if (leaderboardController != nil)
//    {
//        leaderboardController.leaderboardDelegate = self;
//        [self presentModalViewController: leaderboardController animated: YES];
//    }
    AppController *app = (AppController*) [[UIApplication sharedApplication] delegate];
    
//    GKScore * submitScore = [[GKScore alloc] initWithCategory:@"LBTopScore"];
//    [submitScore setValue:100.0];
//    [submitScore setShouldSetDefaultLeaderboard:YES];
//    [submitScore setContext:100.0];
//    PlayerModel *pm = [[[PlayerModel alloc] init] autorelease];
//    [pm submitScore:submitScore];
//    [submitScore release];
    
    GKLeaderboardViewController *leaderboardViewController = [[[GKLeaderboardViewController alloc] init] autorelease];
    [leaderboardViewController setCategory:@"LBTopScore"];
    leaderboardViewController.leaderboardDelegate = self;
    [[app viewController] presentModalViewController:leaderboardViewController animated:YES];
}

- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController{
    AppController *app = (AppController*) [[UIApplication sharedApplication] delegate];
	[[app viewController] dismissModalViewControllerAnimated:YES];

}

@end