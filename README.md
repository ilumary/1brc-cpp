# 1 Billion Rows Challenge

My try at the 1 billion rows challenge. Highly optimized parsing with a custom hash, mmap, simd, and multithreading. I may play with optimizing this even further, especially on lower spec systems.

To generate the required measurements file, have a look at: https://github.com/gunnarmorling/1brc

## Performance

On one of my Uni's HPC servers with an AMD EPYC 9554P 64-Core Processor and > 32GB RAM:

```sh
real    0m0.492s
user    0m9.482s
sys     0m0.883s
```
