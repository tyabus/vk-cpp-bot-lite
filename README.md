# VK C++ BOT

VK based bot on C++

## Getting Started

First of all, we need to install a lot of packages.

For install on Ubuntu/Debian:
```
sudo apt-get install git gcc g++ make libcurl4-openssl-dev libgd-dev libboost-python-dev python3-dev python3-pip
pip3 install psutil untangle py-cpuinfo
````
For install on Android/Termux(termux.com):
```
apt install libcurl-dev libgd-dev python-dev boost-dev clang make git libcrypt-dev pkg-config
pip install psutil untangle py-cpuinfo
export TERMUX=1
```

### Compiling

```
git clone https://github.com/tyabus/vk-cpp-bot-lite/
cd ./vk-cpp-bot-lite
make
```

### Configuration
after first launch configure bot in config.json
