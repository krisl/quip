#include "Selector.hpp"

#include "Document.hpp"
#include "DocumentIterator.hpp"
#include "Selection.hpp"

namespace quip {
  namespace {
    bool isWordCharacter(char character) {
      return std::isalnum(character);
    }
    
    bool isNotWordCharacter(char character) {
      return !isWordCharacter(character);
    }
    
    bool isWhitespaceExceptNewline(char character) {
      return std::isspace(character) && character != '\n';
    }
    
    DocumentIterator selectTrailingWhitespaceIfApplicable(const Document& document, const DocumentIterator& start) {
      DocumentIterator result = start;
      if (result != document.end()) {
        // Try to select any (appropriate) trailing whitespace as well.
        ++result;
        if (isWhitespaceExceptNewline(*result)) {
          result.advanceWhile(isWhitespaceExceptNewline);
        } else {
          --result;
        }
      }
      
      return result;
    }
  }
  
  Optional<Selection> selectThisWord(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    DocumentIterator origin = document.at(basis.origin());
    origin.reverseWhile(isWordCharacter);
    
    DocumentIterator extent = origin;
    extent.advanceWhile(isWordCharacter);
    extent = selectTrailingWhitespaceIfApplicable(document, extent);
    
    return Optional<Selection>(Selection(origin.location(), extent.location()));
  }
  
  Optional<Selection> selectNextWord(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    DocumentIterator origin = document.at(basis.extent());
    origin.advanceUntil(isNotWordCharacter);
    if (origin == document.end()) {
      return Optional<Selection>();
    }
    
    origin.advanceUntil(isWordCharacter);
    if (origin == document.end()) {
      return Optional<Selection>();
    }
    
    return Optional<Selection>(selectThisWord(document, Selection(origin.location())));
  }
  
  Optional<Selection> selectPriorWord(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    DocumentIterator origin = document.at(basis.origin());
    origin.reverseUntil(isNotWordCharacter);
    if (origin == document.begin()) {
      return Optional<Selection>();
    }
    
    origin.reverseUntil(isWordCharacter);
    if (origin == document.begin()) {
      return Optional<Selection>();
    }
    
    return Optional<Selection>(selectThisWord(document, Selection(origin.location())));
  }

  Optional<Selection> selectRemainingWord(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    DocumentIterator extent = document.at(basis.extent());
    extent.advanceWhile(isWordCharacter);
    extent = selectTrailingWhitespaceIfApplicable(document, extent);
    
    return Optional<Selection>(Selection(basis.origin(), extent.location()));
  }
  
  Optional<Selection> selectThisLine(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    Location origin(0, basis.origin().row());
    
    std::uint64_t row = basis.extent().row();
    Location extent(document.row(row).size() - 1, row);
    return Optional<Selection>(Selection(origin, extent));
  }
  
  Optional<Selection> selectNextLine(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    std::uint64_t row = basis.extent().row();
    if (row + 1 < document.rows()) {
      ++row;
      Location origin(0, row);
      Location extent(document.row(row).size() - 1, row);
      return Optional<Selection>(Selection(origin, extent));
    }
    
    return Optional<Selection>(selectThisLine(document, basis));
  }
  
  Optional<Selection> selectPriorLine(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    std::uint64_t row = basis.origin().row();
    if (row > 0) {
      --row;
      Location origin(0, row);
      Location extent(document.row(row).size() - 1, row);
      return Optional<Selection>(Selection(origin, extent));
    }
    
    return Optional<Selection>(selectThisLine(document, basis));
  }
}
