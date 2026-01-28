# Arkas Template
A minimal template for Arkas Engine projects

## How to build
Clone this repo recursively, then follow these steps:

```
# build the engine
cd lib/ArkasEngine
make -j LIB=y

# install headers
sudo install.py

# build template
cd ../..
make -j
```
