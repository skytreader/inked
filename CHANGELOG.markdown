# CHANGELOG - inked

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
