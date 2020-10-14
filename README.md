# sudoku

Techniques:

Set value:
- If a cell has only one possibility
- If a line has only one possibility
- If a square has only one possibility
- When one of the two coloring paths starting from a pair isn't valid

Add constraint:
- LOCK: For a digit, if a line has possibilities only in a block, then the two other blocks contained in this square can't have the value
- LOCK: For a digit, if a square has possibilities only in a block, then the two other blocks along the line can't have the value
- FISH: Look for fish patterns (Xwing, Swordfish, JellyFish ...) and add corresponding constraints
(- PAIR: If a cell has only two possible values, then look for other identical pair of values along the lines or in the square, then other cells in the container can't have these two values)