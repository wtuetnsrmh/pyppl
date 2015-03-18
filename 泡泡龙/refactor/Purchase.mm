

#import "Purchase.h"
#include "Global.h"
//#include <StoreKit/StoreKit.h>



@implementation Purchase

-(void)initConstants{
    
}
-(id)init{
    [self initConstants];
    if (self = [super init]) {
        g_miscLayer->showLoading(true);
        [self loadStore];
    }
    return self;
}

//≥ı ºªØ…ÃµÍº‡Ã˝£®π€≤Ï’ﬂƒ£ Ω£©
-(void)loadStore{
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
}

//Ω· ¯…ÃµÍº‡Ã˝
-(void)unLoadStore{
        [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
}

//≈–∂œ”–√ª”–πÿ±’ƒ⁄π∫£®iOS…Ë∂®¿Ôø…“‘πÿƒ⁄π∫£©
-(BOOL)canPurchase{
    return [SKPaymentQueue canMakePayments];
}

//ø™ º∑¢∆ƒ⁄π∫£®»Îø⁄£©£¨‘⁄µ˜”√’‚∏ˆ∑Ω∑®÷Æ«∞ø…“‘‘⁄UI…œ◊™»¶
NSArray *items=[NSArray arrayWithObjects:@"DC499",@"C099",@"C499",@"C999",@"C1999",@"G099",@"G499",@"G999",@"G1999",nil];
int buyType;
-(void)startPurchase:(int)purchaseType{

    if([self canPurchase]) {
        NSString *identifier; //ø…“‘–¥À¿“≤ø…“‘∂¡ƒ⁄π∫item¿¥»°
        identifier =[items objectAtIndex:purchaseType];
        SKPayment *payment = [SKPayment paymentWithProductIdentifier:identifier];
        [[SKPaymentQueue defaultQueue] addPayment:payment];
        buyType =purchaseType;
    }else{
        //Ω· ¯UI◊™»¶
         g_miscLayer->showLoading(false);
        UIAlertView *alert = [[[UIAlertView alloc]initWithTitle:@"你未开启购买权限" message:nil delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil, nil]autorelease];
        [alert show];
    }
    
}

//’Ê’˝µƒªÿµ˜∫Ø ˝£¨–Ë“™≈–∂œ≥…π¶ªÚ ß∞‹
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased: //≥…π¶
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed: // ß∞‹
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored: //ª÷∏¥
               [self restoreTransaction:transaction];
            default:
                break;
        }
    }
    
}

//ƒ⁄π∫≥…π¶∫Ûµ˜”√
- (void)completeTransaction: (SKPaymentTransaction *)transaction
{
    [self provideContent:transaction.payment.productIdentifier]; //¥´»Îπ∫¬Ú≥…π¶µƒƒ⁄π∫ID
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    g_miscLayer->showLoading(false);
}

//Ã·π©π∫¬Úƒ⁄»›
-(void)provideContent:(NSString*)productIdentifier{
    if(buyType>=0&&buyType<5)
    {
        g_gameLayer->addBuyGold(buyType);
    }else
    {
         g_gameLayer->addBuyGem(buyType);
    }
}

//ƒ⁄π∫ ß∞‹ªÚ»°œ˚∫Ûµ˜”√
- (void) failedTransaction: (SKPaymentTransaction *)transaction
{
    //NSLog(@"Error Purchase");
    //Ω· ¯UI◊™»¶
     g_miscLayer->showLoading(false);
    if (transaction.error.code != SKErrorPaymentCancelled) {
        // Optionally, display an error here.
    }
    switch (transaction.error.code) {
            
        case SKErrorUnknown:
            
            NSLog(@"SKErrorUnknown");
            
            break;
            
        case SKErrorClientInvalid:
            
            NSLog(@"SKErrorClientInvalid");
            
            break;
            
        case SKErrorPaymentCancelled:
            
            NSLog(@"SKErrorPaymentCancelled");
            
            break;
            
        case SKErrorPaymentInvalid:
            
            NSLog(@"SKErrorPaymentInvalid");
            
            break;
            
        case SKErrorPaymentNotAllowed:
            
            NSLog(@"SKErrorPaymentNotAllowed");
            
            break;
            
        default:
            
            NSLog(@"No Match Found for error");
            
            break;
            
    }
    
    NSLog(@"transaction.error.code %@",[transaction.error description]);
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

//***********************“‘œ¬”√”⁄≤ªø…œ˚∑—¿‡ƒ⁄π∫µƒª÷∏¥£¨≤ªø…œ˚∑—¿‡ƒ⁄π∫±ÿ–ÎÃ·π©ª÷∏¥π¶ƒ‹£¨∑Ò‘Ú…Û∫ÀÕ®≤ªπ˝****************************

//ª÷∏¥ƒ⁄π∫£¨∂‘≤ªø…œ˚∑—¿‡–Õµƒƒ⁄π∫”–”√£¨±»»Áø™∆Ù“˛≤ÿπÿø®µ»
-(void)restorePurchase{
    [[SKPaymentQueue defaultQueue]restoreCompletedTransactions];
}

//ª÷∏¥ƒ⁄π∫≥…π¶∫Ûµ˜”√
- (void)restoreTransaction: (SKPaymentTransaction *)transaction
{
    [self provideContent: transaction.originalTransaction.payment.productIdentifier];
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}
//ª÷∏¥≥ˆ¥ÌªÚ÷–Õæ»°œ˚
-(void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error{
    //Ω· ¯UI◊™»¶
     g_miscLayer->showLoading(false);
}

//ª÷∏¥Ω· ¯
-(void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue{
    //Ω· ¯UI◊™»¶
     g_miscLayer->showLoading(false);
}

-(void)dealloc{
    [self unLoadStore];
    [super dealloc];
}
@end