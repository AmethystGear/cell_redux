rm -rf build
cp -r src build
mkdir build/fighters
(cd build_scripts ; ./compile_build_script.sh ; ./build_script ../fighters ;)
mkdir bin
./compile_src.sh