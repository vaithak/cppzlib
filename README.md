# cppzlib
A standalone c++ executable for compressing and decompressing a file using zlib.

## Installing zlib developer files

### macOS
Install `Command-line tools` on your OSX system and zlib will also be installed
```
xcode-select --install
```

### Debian based linux systems
Install `zlib1g-dev` using `apt` package manager.
```
sudo apt-get install zlib1g-dev
```

## Running Instructions
- clone the repository and `cd` into it.
- compile the executable  

  ```sh
  $ g++ --std=c++11 -lz main.cpp compressor.cpp -o cppzlib                                                                                                           
  ```  
  
- Example usage: 
  - compressing `sample.txt` file (provided with the source code) -> produces `sample.txt.z` file. 
  
    ```sh
    $ ./cppzlib -c sample.txt 
    
    ### Output:
    Input file size:         2.14 KB
    Compressed file size:    1.15 KB
    Compression ratio:       1.86
    Elapsed time:            0.14 ms
    Throughput:              15.44 MB/s
    ```  
    
  - decompressing `sample.txt.z` (produced using above command)
     
    ```sh
    $ ./cppzlib -d sample.txt.z
    
    ### Output:
    Input file size:         1.15 KB
    Decompressed file size:  2.14 KB
    Elapsed time:            0.06 ms
    Throughput:              18.66 MB/s
    ```  

