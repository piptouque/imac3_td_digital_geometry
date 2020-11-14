
### M2 SIS Digital Geometry

##### Practical work sessions

---


#### Dependencies

- Boost, etc..  todo!

- Cairo

    sudo apt install libcairo2-dev

- DGtal (with option Cairo enabled)

    cd [dgtal dir]
    mkdir build
    cd build
    cmake .. -DWITH_CAIRO:string=true
    sudo make install
    
*Note to self: for some reason DGtal looks for the general cairo header in `/usr/include/`, 
but it's installed in `/usr/include/cairo/`when using the Debian package, so I had to change:*

    #include <cairo.h>
    
*To:*
    
    #include <cairo/cairo.h>
    
*In `[...]/DGtal/src/BoardPath` *