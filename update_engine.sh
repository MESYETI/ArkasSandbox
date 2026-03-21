git submodule update --remote --recursive
cd lib/ArkasEngine
make -j LIB=y
sudo rm -r /usr/include/Arkas
sudo python install.py
cd ../..
make clean
