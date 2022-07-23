gcc -g -rdynamic \
$(find build -type f -iname *.h -print | tr '\n' ' ') \
$(find build -type f -iname *.c -print | tr '\n' ' ') \
-o cell_redux \
-DEXECUTABLE_NAME='cell_redux'