gcc main.c && ./a.out < testdata/test.in > table.txt
cat head.c table.txt core.c > hakka2.c
rm -f table.txt