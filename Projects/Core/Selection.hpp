#pragma once

#include "Document.hpp"
#include "Location.hpp"

#include <cstdint>

namespace quip {
  // A inclusive region of marked text that can be operated on.
  //
  // A selection is defined by an origin location and an extent location. If they
  // are not equal, the origin is always earlier in the document than the extent.
  struct Selection {
    explicit Selection(const Location& location);
    explicit Selection(const DocumentIterator& location);
    explicit Selection(std::uint64_t originColumn, std::uint64_t originRow);
    explicit Selection(const Location& origin, const Location& extent);
    explicit Selection(const DocumentIterator& origin, const DocumentIterator& extent);
    explicit Selection(std::uint64_t originColumn, std::uint64_t originRow, std::uint64_t extentColumn, std::uint64_t extentRow);
    Selection(const Selection& other);
    Selection(Selection&& other);
    
    Selection& operator=(const Selection& other);
    Selection& operator=(Selection&& other);
    
    const Location& origin() const;
    const Location& extent() const;
    
    std::uint64_t height() const;

    friend void swap(Selection& left, Selection& right);
    
  private:
    Location m_origin;
    Location m_extent;
    
    void normalize();
  };
  
  bool operator==(const Selection& left, const Selection& right);
  bool operator!=(const Selection& left, const Selection& right);
}
