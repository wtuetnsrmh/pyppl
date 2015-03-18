//
//  GameShareOC.h
//  szdzfabu
//
//  Created by MacMini on 14-5-22.
//
//

#import <Foundation/Foundation.h>

@interface GameShareOC : NSObject
+(id) getInstance;
-(void) openShare:(int)_language IsRecordBroken:(BOOL)_isRecordBroken Maxsorce:(int)_maxsorce UIkind:(int)_uiKind SharePicPath:(NSString *)_path;
@end
