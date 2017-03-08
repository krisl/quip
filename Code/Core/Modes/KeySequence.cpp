#include "KeySequence.hpp"

namespace quip {
  KeySequence::KeySequence ()
  : m_isShiftOpen(false) {
  }
  
  KeySequence::KeySequence (Key key)
  : m_keys({key})
  , m_isShiftOpen(false) {
  }
  
  KeySequence::KeySequence (std::initializer_list<Key> keys)
  : m_keys(keys)
  , m_isShiftOpen(false) {
  }
  
  KeySequence::KeySequence (const char * expression) {
    const char * cursor = expression;
    while (cursor != nullptr && *cursor != 0) {
      if (*cursor == '<') {
        ++cursor;
        
        const char * anchor = cursor;
        while(*cursor != '>' && *cursor != '-') {
          ++cursor;
        }
        
        if (*cursor == '-') {
          // Read all modifiers.
          std::int32_t modifier = 0;
          while (anchor != cursor) {
            switch (*anchor) {
              case 'S':
                modifier |= static_cast<std::int32_t>(Key::ShiftMask);
                break;
              case 'C':
                modifier |= static_cast<std::int32_t>(Key::ControlMask);
                break;
              case 'O':
                modifier |= static_cast<std::int32_t>(Key::OptionMask);
                break;
              default:
                break;
            }
            
            ++anchor;
          }
          
          // Start the modifier sequence.
          append(static_cast<Key>(modifier | static_cast<std::int32_t>(Key::ModifierDownMask)));
          
          ++cursor;
          while (*cursor != '>') {
            append(keyFromCharacter(*cursor++));
          }
          
          // Complete the modifier sequence.
          append(static_cast<Key>(modifier | static_cast<std::int32_t>(Key::ModifierUpMask)));
        } else {
          std::string name(anchor, cursor);
          if (name == "ESC") {
            append(Key::Escape);
          }
        }
      } else {
        append(keyFromCharacter(*cursor));
      }
      
      ++cursor;
    }
  }
  
  std::size_t KeySequence::count () const {
    return m_keys.size();
  }
  
  void KeySequence::append (Key key) {
    m_keys.emplace_back(key);
    
    if (key == modifierDown(Key::ShiftMask)) {
      m_isShiftOpen = true;
    }
  }
  
  void KeySequence::clear () {
    m_keys.clear();
    m_isShiftOpen = false;
  }
  
  const Key * KeySequence::begin () const {
    if (m_keys.size() == 0) {
      return nullptr;
    }
    
    return m_keys.data();
  }
  
  const Key * KeySequence::end () const {
    if (m_keys.size() == 0) {
      return nullptr;
    }
    
    return m_keys.data() + m_keys.size();
  }
  
  KeySequence KeySequence::withModifiersClosed () const {
    KeySequence result(*this);
    if (m_isShiftOpen) {
      result.append(modifierUp(Key::ShiftMask));
    }
    
    return result;
  }
}