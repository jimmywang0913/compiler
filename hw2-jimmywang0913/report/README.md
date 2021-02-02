# hw2 report

|||
|-:|:-|
|Name|王吉磊|
|ID|0716029|

## How much time did you spend on this project

    about 10 hours.

## Project overview

    It first takes me two and half hours to understand the hole project and how yacc and lex work together.
    Then I start to modify scanner.l.
    For most of the tokens I use its original token name as its return symbol.
    But some token will have conflict with the c's preserve word.
    so I change them as below chart.
|||
|-:|:-|
|token|retrun symbol|
|:=|aaa|
|<=|bbb|
|<>|ccc|
|>=|ddd|

    Then I start to implement those rules in parser.y.
    This part is quite easy cause there's clear discribtion in README file.
    I use "yacc -v parser.y" in /src to produce y.output.
    I fix all the conflict it shwos in the file.
    Then run test cases and try to fix each of them.
    ----------------------------finish-------------------------------
    

## What is the hardest you think in this project

    The hardest part I think is to understand how parser work with lex.
    The way these two program communicate with each others to me is a brand new method.
    At first I don't know that makefile will automatically generate xxx.h and include it in scanner.l.
    So each time I change the token, I have to type comment yacc -d parser.y to remake "y.tab.h".
    After that I have to deal with the problem that strings like ":=" can't be token symbol.
    It takes me a while to figure out that I can use other string as its token symbol.
    

## Feedback to T.A.s

> nothing to say, everything is perfect.
