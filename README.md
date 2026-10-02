# 1 Billion Rows Challenge

My try at the 1 billion rows challenge. Highly optimized parsing with a custom hash, mmap, and multithreading. I may play with optimizing this even further, especially on lower spec systems.

## Performance

On one of my Uni's HPC servers with an AMD EPYC 9554P 64-Core Processor and about 1 TB RAM:

```sh
real    0m0.565s
user    0m15.427s
sys     0m0.955s
```

On my local MacBook Pro with an M1 Pro inside a linux container with 8 GB RAM and 6 threads:

```sh
real    0m25.240s
user    1m50.499s
sys     0m40.255s
```
