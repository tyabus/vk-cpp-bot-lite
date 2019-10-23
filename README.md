# VK C++ BOT

VK based bot on C++

## Getting Started

First of all, we need to install a lot of packages.

If you want to build bot without python3, then don't install python3-dev and libbost-python-dev for ubuntu/debian, and python and boost for rach linux.

For install on Ubuntu/Debian:
```
sudo apt update
sudo apt install git gcc g++ make libcurl4-openssl-dev libgd-dev libboost-python-dev python3-dev
````
For install on Rach Linux:
```
pacman -Syu
pacman -S git gcc make curl boost gd python
```

### Compiling

```
git clone https://github.com/tyabus/vk-cpp-bot-lite
cd ./vk-cpp-bot-lite
make
```

### Configuration
after the first launch configure bot in config.json
