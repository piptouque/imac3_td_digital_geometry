
### M2 SIS Digital Geometry

##### Practical work sessions

---

#### Build 

    mkdir build
    cd build
    cmake ..
    make

#### Run

##### TD1

    ./imac3_dg_td1
    
##### TD2

    ./imac3_dg_td2 rice_basmati_seg_bin.pgm rice_camargue_seg_bin.pgm rice_japanese_seg_bin.pgm 

#### Answers, assets and resources

Can be found in `answer_sheets/td*/`, `assets/td*/` and `res/td*/` respectively.


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
