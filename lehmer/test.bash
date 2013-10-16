#!/bin/bash

a='2011'
j='1024'
s='10'
x='1'
n='10'

# Ruby
#echo '@@Ruby!'
#ruby lehmers.rb $a $j $s $x $n --seeds #> ruby_output.txt

# Python
#echo '@@Python!'
#python lehmers.py $a $j $s $x $n --seeds #> python_output.txt

# C
make -s c
echo '@@C!'
./a.out $a $j $s $x $n --seeds --isaj

#diff ruby_output.txt python_output.txt
