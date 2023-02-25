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
    ```  
    
  - decompressing `sample.txt.z` (produced using above command)
     
    ```sh
    $ ./cppzlib -d sample.txt.z
    
    ### Output:
    ```  

