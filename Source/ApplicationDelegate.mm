#import "ApplicationDelegate.h"

#import "QuipView.h"

#include "Document.hpp"

#include <memory>

@interface ApplicationDelegate () {
@private
  NSWindow * m_window;
  QuipView * m_view;
}
@end

@implementation ApplicationDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
  CGRect frame = CGRectMake(0.0f, 0.0f, 1024.0f, 768.0f);
  
  std::shared_ptr<quip::Document> document = std::make_shared<quip::Document>("Welcome to Quip!\nQuip is a modal text editor.");
  m_view = [[QuipView alloc] initWithFrame:frame document:document];
  
  m_window = [[NSWindow alloc] initWithContentRect:frame styleMask:NSTitledWindowMask backing:NSBackingStoreBuffered defer:YES];
  [m_window setTitle:@"Untitled"];
  [m_window center];
  [m_window setContentView:m_view];
  
  [m_window makeKeyAndOrderFront:self];
}

- (void)loadDocumentFrom:(NSURL *)url {
  NSError * error;
  NSString * content = [[NSString alloc] initWithContentsOfURL:url encoding:NSUTF8StringEncoding error:&error];
  if (content == nil) {
    NSLog(@"Error opening file (%@): %@", url, [error localizedFailureReason]);
  } else {
    std::shared_ptr<quip::Document> document = std::make_shared<quip::Document>([content cStringUsingEncoding:NSUTF8StringEncoding]);
    [m_view setDocument:document];
    [m_window setTitle:[url path]];
  }
}

- (IBAction)openDocument:(id)sender {
  NSOpenPanel * panel = [NSOpenPanel openPanel];
  [panel setCanChooseFiles:YES];
  [panel setCanChooseDirectories:NO];
  
  if([panel runModal] == NSFileHandlingPanelOKButton) {
    [self loadDocumentFrom:[panel URL]];
  }
}

@end