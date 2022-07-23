rm -rf build
cp -r src build
mkdir build/fighters
(cd build_scripts ; ./compile_build_script.sh ; ./build_script ../../cell_redux_server/fighters ;)
./compile_src.sh