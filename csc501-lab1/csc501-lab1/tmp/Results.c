There is some inconsistency with XINU When the same code gives output most of the times sometimes 
its not funtioning properly sometimes without any change of code 
Still not able to figure out the reason why its not giving results sometimes.

So pasting here the results which I have obtained 


Results for exponnetial distibution.

when the priorities have been A=10, B=20, C=30;

===============================================

Xinu Version (CSC501 1-2009 base) #364 (ppacha@vm18-69.vcl) Sun Sep 30 18:05:44 EDT 2018

16777216 bytes real mem
78799 bytes Xinu code
clock enabled
Start... A
Start... B
Start... C


Test Result: A = 150866, B = 57690, C = 31672


All user processes have completed.


Xinu Version (CSC501 1-2009 base) #366 (ppacha@vm18-69.vcl) Sun Sep 30 18:07:02 EDT 2018

16777216 bytes real mem
78799 bytes Xinu code
clock enabled
Start... A
Start... B
Start... C
aaa:0  bbb: 0  ccc:0
a_call;0  b_call;0  c_call :0

Test Result: A = 146948, B = 57466, C = 31106


All user processes have completed.

	

Xinu Version (CSC501 1-2009 base) #365 (ppacha@vm18-69.vcl) Sun Sep 30 18:06:31 EDT 2018

16777216 bytes real mem
78799 bytes Xinu code
clock enabled
Start... A
Start... B
Start... C
aaa:0  bbb: 0  ccc:0
a_call;0  b_call;0  c_call :0

Test Result: A = 150359, B = 56919, C = 32056


All user processes have completed.



Xinu Version (CSC501 1-2009 base) #368 (ppacha@vm18-69.vcl) Sun Sep 30 18:12:51 EDT 2018

16777216 bytes real mem
78767 bytes Xinu code
clock enabled
Start... A
Start... B
Start... C

Test Result: A = 132111, B = 48722, C = 30107

Xinu Version (CSC501 1-2009 base) #369 (ppacha@vm18-69.vcl) Sun Sep 30 18:13:43 EDT 2018

16777216 bytes real mem
78767 bytes Xinu code
clock enabled
Start... A
Start... B
Start... C

Test Result: A = 147839, B = 54269, C = 33997


All user processes have completed.

QEMU 2.0.0 monitor - type 'help' for more information



===============================================================
When the priorities of A,B,C  have all been 10.

Xinu Version (CSC501 1-2009 base) #358 (ppacha@vm18-69.vcl) Sun Sep 30 17:58:40 EDT 2018

16777216 bytes real mem
78799 bytes Xinu code
clock enabled
Start... A
Start... B
Start... C
aaa:0  bbb: 0  ccc:0
a_call;0  b_call;0  c_call :0

Test Result: A = 80051, B = 79629, C = 80046

=================================


Xinu Version (CSC501 1-2009 base) #359 (ppacha@vm18-69.vcl) Sun Sep 30 17:59:31 EDT 2018

16777216 bytes real mem
78799 bytes Xinu code
clock enabled
Start... A
Start... B
Start... C
aaa:0  bbb: 0  ccc:0
a_call;0  b_call;0  c_call :0

Test Result: A = 80540, B = 82710, C = 81578

===============================

Xinu Version (CSC501 1-2009 base) #360 (ppacha@vm18-69.vcl) Sun Sep 30 18:00:09 EDT 2018

16777216 bytes real mem
78799 bytes Xinu code
clock enabled
Start... A
Start... B
Start... C
aaa:0  bbb: 0  ccc:0
a_call;0  b_call;0  c_call :0

Test Result: A = 73191, B = 71951, C = 72025


All user processes have completed.

QEMU 2.0.0 monitor - type 'help' for more information
(qemu) q




When the priorities of A,B,C  have all been 20.

=================================
Xinu Version (CSC501 1-2009 base) #361 (ppacha@vm18-69.vcl) Sun Sep 30 18:02:07 EDT 2018

16777216 bytes real mem
78799 bytes Xinu code
clock enabled
Start... A
Start... B
Start... C
aaa:0  bbb: 0  ccc:0
a_call;0  b_call;0  c_call :0

Test Result: A = 75939, B = 76302, C = 75569


Xinu Version (CSC501 1-2009 base) #362 (ppacha@vm18-69.vcl) Sun Sep 30 18:03:01 EDT 2018

16777216 bytes real mem
78799 bytes Xinu code
clock enabled
Start... A
Start... B
Start... C
aaa:0  bbb: 0  ccc:0
a_call;0  b_call;0  c_call :0

Test Result: A = 77056, B = 76672, C = 76672


All user processes have completed.


when all the priorities have 30

==================================================

Xinu Version (CSC501 1-2009 base) #363 (ppacha@vm18-69.vcl) Sun Sep 30 18:04:06 EDT 2018

16777216 bytes real mem
78799 bytes Xinu code
clock enabled
Start... A
Start... B
Start... C
aaa:0  bbb: 0  ccc:0
a_call;0  b_call;0  c_call :0

Test Result: A = 79774, B = 80479, C = 80434


All user processes have completed.




=========================Results for the Linux distribution================================ 

Xinu Version (CSC501 1-2009 base) #372 (ppacha@vm18-69.vcl) Sun Sep 30 18:17:23 EDT 2018

16777216 bytes real mem
78767 bytes Xinu code
clock enabled
MMCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBMMMMMMMMAACCCCCCCCCCCCCCCCCCCCBBBBBBBBBMMMMMMACCCCCCCCCCCBBBBBBBBBBBBBBMMMMMMABBBBBBBBBBMMMMMMABBBBBBMMMMMAMMMMMMAAMMMMMAMMMMMAMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA



For the roundrobin case

Xinu Version (CSC501 1-2009 base) #370 (ppacha@vm18-69.vcl) Sun Sep 30 18:14:59 EDT 2018

16777216 bytes real mem
78767 bytes Xinu code
clock enabled
MMMMMMCCCCCBBBBBAAAAAABBBCCCMMMMMMCCCCCCCCBBBBAAAAAAAABBBBCCCCMMMMMMMMCCCCCBBBAAAAAAAAABBBBBBBCCCCMMMMMMMMMMCCCCBBBBAAAAAAAABBBBCCCCMMMMMMMMMCCCCBBBBBAAAAAAAAAABBBBBBCCCCMMMMMMMMCCCCCBBBBBAAAAAAAAMMMA

All user processes have completed.

