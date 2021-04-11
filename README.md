# ssd-nvme-database

Columnar database on SSD NVMe

## Building

In the root of the project directory, run:

```bash
git clone https://github.com/google/googletest.git
git clone https://github.com/google/benchmark.git
git clone https://github.com/axboe/liburing.git
cd liburing
sudo make install
cd ..
mkdir build
cd build
cmake ..
make
```
