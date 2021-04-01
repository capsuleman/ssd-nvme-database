# ssd-nvme-database
Columnar database on SSD NVMe

## Dependencies
### Liburing
```
git clone https://github.com/axboe/liburing.git
cd liburing
sudo make install
```
Yes this is shady

## Building

In the root of the project directory, run:
```
git clone https://github.com/google/googletest.git
git clone https://github.com/google/benchmark.git
mkdir build
cd build
cmake ..
make
```
