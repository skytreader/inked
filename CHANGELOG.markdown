# CHANGELOG - inked

## v0.1.2

- **Introducing:** Command mode. Totally stealing this from vim.
- You can now type into the editor during edit mode. Text wrapping, pagination,
  deletion, and navigation not assured. Definitely do not resize the terminal
  window.
   - This is done via a text buffer data structure (at the moment, just a really
   long array). This keeps tracks of other things about the text. Deletions
   might be slow due to the nature of this data structure. Wonder what's a
   better alternative?
- Lots of admittedly-buggy behavior but, hey, it's a milestone!

## v0.1.1

- Scrollback issues from previous version are fixed. It still does not behave in
  the way vim does though; everytime the program is ran, the terminal is
  basically wiped.
- Cursor is now moving.

## v0.1.0

- Editor now attempts to be responsive (by getting the terminal size before
  drawing the UI) but you still need to press an arrow key for the resize to
  take effect.
- Outstanding problem: invoking the binary clears out traces of the command
  prompts but any previous draws from inked remain. Repro steps:

  1. Start with a fresh terminal (i.e., `reset` the terminal).
  2. Invoke the binary and try to scroll up with your mouse. There will be
     nothing. :white_check_mark:.
  3. Quit the binary. This returns you to a command prompt. But if you scroll
     up, traces of the editor remain. :x:
  4. Run the binary again. This time, you can see the traces of the previous
     session when you scroll up. :x:
