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