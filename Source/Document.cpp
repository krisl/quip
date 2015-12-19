#include "Document.hpp"

#include "Selection.hpp"
#include "SelectionSet.hpp"

#include <iostream>
#include <regex>
#include <sstream>
#include <string>

namespace {
  static std::regex gNewlinePattern("\\n");
}

namespace quip {
  Document::Document () {
  }
  
  Document::Document (const std::string & content)
  : m_rows(std::sregex_token_iterator(content.begin(), content.end(), gNewlinePattern, -1), std::sregex_token_iterator()) {
  }
  
  std::string Document::contents () const {
    std::ostringstream stream;
    for (const std::string & text : m_rows) {
      stream << text << "\n";
    }
    
    return stream.str();
  }
  
  const std::string & Document::path () const {
    return m_path;
  }
  
  void Document::setPath (const std::string & path) {
    m_path = path;
  }
  
  const std::string & Document::row (std::size_t index) const {
    return m_rows[index];
  }
  
  std::size_t Document::rows () const {
    return m_rows.size();
  }
  
  void Document::insert (SelectionSet & selections, const std::string & text) {
    int rowDelta = 0;
    int columnDelta = 0;
    std::size_t priorRow = rows();
    
    for (Selection & selection : selections) {
      // Reset column delta when changing rows.
      if (selection.lowerBound().row() != priorRow) {
        priorRow = selection.lowerBound().row();
        columnDelta = 0;
      }
      
      Location lower = selection.lowerBound().adjustBy(columnDelta, rowDelta);
      std::string prefix = m_rows[lower.row()].substr(0, lower.column());
      std::string suffix = m_rows[lower.row()].substr(lower.column());
      m_rows[lower.row()] = prefix + text + suffix;
      
      columnDelta += text.size();
      
      selection.setOrigin(selection.origin().adjustBy(columnDelta, 0));
      selection.setExtent(selection.origin());
    }
  }
  
  SelectionSet Document::matches (const std::string & pattern) const {
    std::vector<Selection> results;
    std::regex regex(pattern);
    
    for (std::size_t row = 0; row < m_rows.size(); ++row) {
      const std::string & text = m_rows[row];
      std::sregex_iterator cursor(text.begin(), text.end(), regex);
      std::sregex_iterator end;
      
      while (cursor != end) {
        std::smatch match = *cursor;
        for (std::size_t matchIndex = 0; matchIndex < match.size(); ++matchIndex) {
          std::size_t origin = match.position(matchIndex);
          std::size_t extent = origin + match[matchIndex].length() - 1;
          results.emplace_back(Location(origin, row), Location(extent, row));
        }
        
        ++cursor;
      }
    }
    
    return SelectionSet(results);
  }
}