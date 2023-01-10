#!/bin/bash

# 実行時に指定された引数の数、つまり変数 $# の値が 3 でなければエラー終了。
if [ $# -ne 1 ]; then
  echo "指定された引数は$#個です。" 1>&2
  echo "実行するには1個の引数が必要です。" 1>&2
  exit 1
fi

../src/build/flbe < $1 1>&2 | clang++ rt.cpp output.o -o test

# ヒアドキュメントでメッセージを表示する。
cat <<__EOT__
DONE!
__EOT__

exit 0
