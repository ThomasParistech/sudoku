# sudoku

Techniques:

Set value:
- If a cell has only one possibility
- If a line has only one possibility
- If a square has only one possibility

Add constraint:
- LOCK: For a digit, if a line has possibilities only in a block, then the two other blocks contained in this square can't have the value
- LOCK: For a digit, if a square has possibilities only in a block, then the two other blocks along the line can't have the value
- PAIR: If a cell has only two possible values, then look for other identical pair of values along the lines or in the square, then other cells in the container can't have these two values
- XWING: If a line has only two possible locations for a value and if there's another line with the same only 2 locations for the same value, then the perpendicular lines can contain the values only at the intersection with these two lines
