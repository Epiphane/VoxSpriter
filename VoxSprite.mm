//
//  VoxSprite.m
//  VoxSpriter
//
//  Created by Thomas Steinke on 7/23/14.
//  Copyright (c) 2014 Thomas Steinke. All rights reserved.
//

#import "application.h"
#import "VoxSprite.h"
#import <Cocoa/Cocoa.h>

@implementation VoxSprite

- (id)init {
    self = [super init];
    if (self) {
        // Add your subclass-specific initialization here.
    }
    return self;
}

- (void)windowControllerDidLoadNib:(NSWindowController *)aController {
    [super windowControllerDidLoadNib:aController];
    // Add any code here that needs to be executed once the windowController has loaded the document's window.
}

- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError {
    // Insert code here to write your document to data of the specified type. If outError != NULL, ensure that you create and set an appropriate error when returning nil.
    // You can also choose to override -fileWrapperOfType:error:, -writeToURL:ofType:error:, or -writeToURL:ofType:forSaveOperation:originalContentsURL:error: instead.
    if (outError) {
        *outError = [NSError errorWithDomain:NSOSStatusErrorDomain code:unimpErr userInfo:nil];
    }
    return nil;
}

- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError {
   // Insert code here to read your document from the given data of the specified type. If outError != NULL, ensure that you create and set an appropriate error when returning NO.
   // You can also choose to override -readFromFileWrapper:ofType:error: or -readFromURL:ofType:error: instead.
   // If you override either of these, you should also override -isEntireFileLoaded to return NO if the contents are lazily loaded.
   if (outError) {
      *outError = [NSError errorWithDomain:NSOSStatusErrorDomain code:unimpErr userInfo:nil];
   }
   
   const char *dataStr = (const char *) [data bytes];
   
   return openApplication(dataStr, [data length]);
}

+ (BOOL)autosavesInPlace {
   return YES;
}

@end
