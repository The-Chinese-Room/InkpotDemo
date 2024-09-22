LIST letters = A, B, C, D, E, F
LIST numbers = _1, _2, _3, _4, _5, _6
LIST tens = ten=10, twenty=20, thirty=30, fourty=40, fifty=50, sixty=60

VAR sublist_1 = A
VAR sublist_2 = (_1, A, B, C )
VAR sublist_3 = ( D, E, F, _1, B )

VAR sublist_B = B
VAR sublist_C = C

This is a test script for ink lists.

=== DumpLetters( inLetters )
    We have the letters '{inLetters}' from '{LIST_ALL(inLetters)}'
    sublist_1 = {sublist_1}
    sublist_2 = {sublist_2}
    ->DONE
