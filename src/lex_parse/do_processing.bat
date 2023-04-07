rem run win_flex and win_bison ... with appropriate flags
rem
win_bison struct_yacc.y
rem
win_flex --wincompat struct_lex.l
