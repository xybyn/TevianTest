mkdir build-deb
cd build-deb
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
cp ./TevianTestCli ../package/usr/bin
dpkg-deb --build ../package
sudo apt install ../package.deb