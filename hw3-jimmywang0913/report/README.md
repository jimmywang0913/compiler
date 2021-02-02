# hw3 report

|||
|-:|:-|
|Name|王吉磊|
|ID|0716029|

## How much time did you spend on this project

e.g. 30 hours.

## Project overview

一開始我對整個專案的實作完全沒有概念，什麼是ＡＳＴ node 和 dumper 。所以我決定從parser.y這個檔案開始實做。
我先把所有的rule都先建構好（當然有一些還看不懂的就先放著）在建構的過程中，我慢慢理解到這個專案到底在做什麼。完成 rule 之後我就凱史完成各個hpp黨，先把她應該有的輸入（function得輸入）完成。然後開始弄懂visitor pattern要怎麼實做。完成之後我先從program 開始按照順序一個一個調整，一開始真的弄滿久的，但過了前面幾分之後後面就很順利了。
bonus: 
先在每一個node的file中加入之後要override visitor的function（有accept 和 visitchildnode）
> accept:
所有節點都是由accept開始進入，一但呼叫這個函式，就會走到dumper那裏實做print的部分。
之後就會呼叫visitchildnode.
> visitchildnode:
這個函式會在各個節點的cpp file 實做，至於要走訪打些節點就是由規則規定。基本上所有需要走訪的節點都是由下面的分支傳上來的。
就按照規則依續基奧各個節點的accept function. 

## What is the hardest you think in this project

要理解整份專案在幹嘛加上實做每一個node的function會花上非常多的時間。

## Feedback to T.A.s

> 一開始沒看到pull request 花了滿多時間debug的
