﻿using System;

namespace Quip {
  public interface IDocumentView {
    Location Cursor { get; }

    Document Document { get; set; }
    Mode Mode { get; set; }

    void MoveTo (Location cursor);

    void PushCursorStyle (CursorStyle style);
    void PopCursorStyle ();
  }
}

