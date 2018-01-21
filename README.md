# TestApplication

Never worked with MFC before, expect clunky code. Hopefully, I will get better with time!

IDE used: VS2017.

# Known issues

- Couldn't figure out how to add undo/redo to QAT only -- VS2017 just ignores ID_EDIT_UNDO. So Undo/Redo was added to the ribbon itself. Googling yielded no results how to solve it.

- Undo/Redo are not disabled when nothing to redo/undo. Instead, a message box is shown.

- Fast double-clicking on undo/redo performs only one action -- limitation of
the ribbon, or it can be worked around? Better use Ctrl+Z/Ctrl+Y instead so far.

- Setting minimum size makes maximum size look funny, so it was commented out.

- Haven't figure out scrollbars yet.

- ReportSaveLoadException appears to not be invoked if OnOpenDocument(..)
returns FALSE on corrupted files, only if an exception is thrown. Didn't pursue it further.

- No antialiasing.

# Architecture (quick recap)

MFC's TestApplicationView re-packages MFC/Windows events to Presenter.
Presenter enqueues undoable actions or calls directly (when an action should not be registered in the history, such as the process of dragging itself). Undoable actions work with DocumentView, which contains ShapeView's. Undoable actions can be done, redone, undone. When the DocumentView changes, the fact of the change is reported back to Presenter via the IDocumentViewEventHandler interface. In this handler, Presenter updates the DocumentModel from the current view by using mappers (IShapeMapper interface). Presenter can also update TestApplicationView itself via the INativeView interface (required for things like changing the cursor icon). Deserialization uses XML, and objects are mapped back using IShapeMapper's as well. Presenter then converts the model to the view. DocumentView's control methods such as Select(..), Drag(..), EndDrag(..) and Hover(..) simulate user intentions which can fail (nothing could be selected/dragged and so on) or depend on the current context.
