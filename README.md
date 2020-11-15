
### M2 SIS Digital Geometry

##### Practical work sessions

---

#### Build and run

    mkdir build
    cd build
    cmake ..
    make

    ./imac3_td_dg_TD[i]

#### Answers and resources

Can be found in `answer_sheets/` and `res/` respectively.


#### Dependencies

KaTeX for maths parts in Markdown. (There's the `markdown-preview-enhanced` package for Atom.)

Boost, etc..

Cairo

    sudo apt install libcairo2-dev


DGtal (with option Cairo enabled)

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

*In `[...]/DGtal/src/BoardPath`*
