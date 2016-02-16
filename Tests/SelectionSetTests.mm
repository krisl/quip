#import <XCTest/XCTest.h>

#include "SelectionSet.hpp"

@interface SelectionSetTests : XCTestCase

@end

@implementation SelectionSetTests

- (void)testDefaultConstruction {
  quip::SelectionSet empty;
  
  XCTAssertEqual(empty.count(), 0);
}

- (void)testSingleSelectionConstruction {
  quip::Selection selection(quip::Location(0, 0), quip::Location(1, 0));
  quip::SelectionSet set(selection);
  
  XCTAssertEqual(set.count(), 1);
  XCTAssertEqual(set.primary(), selection);
}

- (void)testMultipleSelectionConstruction {
  quip::Selection a(quip::Location(0, 0), quip::Location(1, 0));
  quip::Selection b(quip::Location(0, 1), quip::Location(1, 1));
  std::vector<quip::Selection> selections { a, b };
  quip::SelectionSet set(selections);
  
  XCTAssertEqual(set.count(), 2);
  XCTAssertEqual(set.primary(), a);
}

- (void)testIndexing {
  quip::Selection a(quip::Location(0, 0), quip::Location(1, 0));
  quip::Selection b(quip::Location(0, 1), quip::Location(1, 1));
  std::vector<quip::Selection> selections { a, b };
  quip::SelectionSet set(selections);
  
  XCTAssertEqual(set[0], a);
  XCTAssertEqual(set[1], b);
}

- (void)testForwardIteration {
  quip::Selection a(quip::Location(0, 0), quip::Location(1, 0));
  quip::Selection b(quip::Location(5, 5), quip::Location(6, 5));
  std::vector<quip::Selection> selections { a, b };
  quip::SelectionSet set(selections);
  
  std::size_t index = 0;
  quip::SelectionSetIterator cursor = set.begin();
  while (cursor != set.end()) {
    XCTAssertEqual(*cursor, selections[index]);
    ++cursor;
    ++index;
  }
}

- (void)testReverseIteration {
  quip::Selection a(quip::Location(0, 0), quip::Location(1, 0));
  quip::Selection b(quip::Location(5, 5), quip::Location(6, 5));
  std::vector<quip::Selection> selections { a, b };
  quip::SelectionSet set(selections);
  
  std::size_t index = 1;
  quip::ReverseSelectionSetIterator cursor = set.rbegin();
  while (cursor != set.rend()) {
    XCTAssertEqual(*cursor, selections[index]);
    ++cursor;
    --index;
  }
}

- (void)testSelectionsAreSorted {
  quip::Selection a(quip::Location(0, 0), quip::Location(0, 5));
  quip::Selection b(quip::Location(5, 10), quip::Location(0, 8));
  quip::Selection c(quip::Location(10, 10), quip::Location(0, 15));
  std::vector<quip::Selection> selections { c, b, a };
  
  quip::SelectionSet set(selections);
  quip::SelectionSetIterator cursor = set.begin();
  XCTAssertEqual(*cursor, a);
  
  ++cursor;
  XCTAssertEqual(*cursor, b);
  
  ++cursor;
  XCTAssertEqual(*cursor, c);
}

- (void)testOverlappingSelectionsCollapse {
  quip::Selection a(quip::Location(1, 0), quip::Location(10, 0));
  quip::Selection b(quip::Location(8, 0), quip::Location(0, 5));
  quip::Selection c(quip::Location(0, 3), quip::Location(0, 7));
  std::vector<quip::Selection> selections { c, b, a };

  quip::SelectionSet set(selections);
  XCTAssertEqual(set.count(), 1);
  
  quip::SelectionSetIterator cursor = set.begin();
  XCTAssertEqual(cursor->origin(), quip::Location(1, 0));
  XCTAssertEqual(cursor->extent(), quip::Location(0, 7));
}

- (void)testPairsOfOverlappingSelectionsCollapse {
  quip::Selection a(quip::Location(1, 0), quip::Location(10, 0));
  quip::Selection b(quip::Location(5, 0), quip::Location(15, 0));
  quip::Selection c(quip::Location(0, 5), quip::Location(15, 5));
  quip::Selection d(quip::Location(10, 5), quip::Location(0, 10));
  std::vector<quip::Selection> selections { d, c, b, a };
  
  quip::SelectionSet set(selections);
  XCTAssertEqual(set.count(), 2);
  
  quip::SelectionSetIterator cursor = set.begin();
  XCTAssertEqual(cursor->origin(), quip::Location(1, 0));
  XCTAssertEqual(cursor->extent(), quip::Location(15, 0));
  
  ++cursor;
  XCTAssertEqual(cursor->origin(), quip::Location(0, 5));
  XCTAssertEqual(cursor->extent(), quip::Location(0, 10));
}

@end
