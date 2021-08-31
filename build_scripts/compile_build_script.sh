gcc -g -rdynamic \
../src/utility/utils.c \
../src/utility/error_handling/errors.c \
../src/utility/error_handling/backtrace.c \
../src/utility/error_handling/signalhandler.c \
../src/utility/codegen/codegen.c \
build.c \
-o build_script \
-DEXECUTABLE_NAME='build_script'
