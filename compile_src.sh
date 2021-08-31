gcc -g -rdynamic \
$(find build -type f -iname *.c -print | tr '\n' ' ') \
-o bin/cell_redux \
-DEXECUTABLE_NAME='cell_redux'