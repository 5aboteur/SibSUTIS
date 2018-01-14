set xrange [0:3000]
set yrange [0:3000]

plot "test.out" using 1:2 with lines
pause 1
reread
