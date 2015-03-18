#ifndef __Purchase__H_
#define __Purchase__H_
#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>


@interface Purchase : NSObject<SKPaymentTransactionObserver>{
    
}
-(void)startPurchase:(int)purchaseType;

@end

#endif