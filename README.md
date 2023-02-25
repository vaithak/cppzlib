# cppzlib
A standalone c++ executable for compressing and decompressing a file using zlib.

### Running Instructions
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

