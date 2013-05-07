mkdir build
cd build
sudo make uninstall
make clean
rm CMakeCache.txt 
cmake .. -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix`
make
sudo make install
