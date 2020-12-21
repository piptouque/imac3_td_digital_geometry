### Digital Geometry

##### M2 SIS, UGE

### Laboratory Session \#2

#### November 25th, 2020

----

### Experiments

#### Step 1

###### Characterise the general grain shape of each rice type.

We see that japanese rice is more curvy and round,
bismati rice is long and thin, while camarague rice is somewhat halfway
in between.  

#### Step 2

###### See whether the images are well-composed.

I could not find evidence of ill-composition with the digital object,
since it is a pixel-perfect import from the image (which itself is well-composed.)
There are some errors with the boundaries between grains, however.

###### Eliminate the grains which are partly out of frame.

We use the greedy predicate that a grain is out of frame if its boundary
is in contact with the rim of the domain.
It does not remove all faulty grains, for some reason.

#### Step 5

Note: *multi-grid convergence* denotes whether the error from digitalising a shape converges with increasing grid resolution. Ideally it converges towards zero, but it is not necessarily the case. (see the issues with digitalising a disc in Step 7 or in the previous assignement.)

###### Do these area measurements maintain the multi-grid convergence?

As we saw in class and in the previous assignment, computing the area from the grid cells gives more precise results as the grid resolution increases, no matter the shape. It thus maintains multi-grid convergence.

Note: contrarily to the previous assignment, the convex polygon is not centred around the $(0, 0)$. As such, we choose a point $I$ inside $P$ which will be our origin during the computation.

The height $h_{i}$ becomes, with $\omega = 2I$

$$
    h_{i} =  \frac{|p_{i+1} + p_{i} - \omega|}{2}
$$

And $A(t_{i})$ becomes:

$$
    A(t_{i}) = \frac{1}{4} |p_{i+1} - p_{i}| |p_{i+1} + p_{i} - \omega|
$$

###### Analyse the distribution of estimated areas for each rice type. Can the area be a principle component in a rice type classification?

Except of the output of the programme:

    ---- STATISTICS -----
    -- rice_basmati_seg_bin.pgm --
    [Area (Count)]      avg: 2226.59  deviation: 454.898
    [Area (CH)]         avg: 3702  deviation: 803.652
    [Area (Seg)]         avg: 3603.95  deviation: 848.445
    -- rice_camargue_seg_bin.pgm --
    [Area (Count)]      avg: 2638.82  deviation: 618.246
    [Area (CH)]         avg: 3383.54  deviation: 828.027
    [Area (Seg)]         avg: 3261.8  deviation: 837.072
    -- rice_japanese_seg_bin.pgm --
    [Area (Count)]      avg: 2058.34  deviation: 164.672
    [Area (CH)]         avg: 2345.53  deviation: 180.161
    [Area (Seg)]         avg: 2245.44  deviation: 205.165


For a reason we could not find, the area is much better computed by the count method. The grid resolution being rather high, we should find comparable results.
As things are, the area is not a good basis for classification because it suffers from too much deviation (note that in this case the deviation is homogeneous to a surface).


#### Step 6

###### Do these perimeter measurements maintain multi-grid convergence?

As was mentioned in Step 5, the perimeter for the digitalisation of a disc does not converges towards the perimeter of the original disc, but towards the perimeter of the square with a half-width the disc's radius. More generally, perimeter does not maintain multi-grid convergence,

###### Analyse the distribution of estimated perimeters for each rice type. Can the perimeter be a principle component in a rice type classification?

Another part of the output:

    ---- STATISTICS -----
    -- rice_basmati_seg_bin.pgm --
    [Perimeter (Count)] avg: 289.079  deviation: 47.2297
    [Perimeter (CH)]    avg: 229.764  deviation: 29.8473
    [Perimeter (Seg)]    avg: 226.145  deviation: 34.1156
    -- rice_camargue_seg_bin.pgm --
    [Perimeter (Count)] avg: 266.655  deviation: 46.3045
    [Perimeter (CH)]    avg: 211.942  deviation: 32.0266
    [Perimeter (Seg)]    avg: 207.873  deviation: 35.7147
    -- rice_japanese_seg_bin.pgm --
    [Perimeter (Count)] avg: 218.971  deviation: 14.4761
    [Perimeter (CH)]    avg: 174.957  deviation: 6.3039
    [Perimeter (Seg)]    avg: 171.2  deviation: 10.7742


Here we get better results from the Convex Hull method, as expected. However, the perimeters are on average a bit too close to each other (taking the standard deviation into account) for us to be able to classify the grains based solely on this criteria.

#### Step 7

###### Propose a circularity definition.


Note: While the following should hold true, the error associated with the area with the Convex Hull method results in a worse categorisation with the Convex Hull as counting 2-cells (again, we could not gather why.) We will nonetheless use the Convex Hull in the programme, for more generality.

We know that in a $\R^{2}$ euclidean space, the circle is the shape which maximises area for a given perimeter, or minimises perimeter for a given area.


However, we use this method with non-convex polygons. For instance in the previous assignement we saw that the digitalised circle is only starred and not convex, and that its perimeter is equal to the perimeter of a square with a half-width equal to its radius.

![DiscDigitalised](../../res/td1/DiscBoundary.png)

Therefore, we should use the convex hull of a digital object to computeGeometry its area and perimeter, as we know it more truthful than the counting method for digitalisations of convex shapes that are not convex themselves.

![DiscHull](../../res/td1/ConvexHullDisc.png)


First, we can construct a ratio $R(S)$ without dimension, which will caracterise the area $A(S)$ of a convex shape $S$ relative to its perimeter $L(S)$.

$$
  R(S) = \frac{A(S)}{L(S)^{2}}
$$

Keeping in mind that $S$ is convex, we can see that :
- $R(S)$ does not depend on the scale of $S$ (its value is without dimension.)
- $R(S) > 0, \forall S$.
- $R(S)$ will reach its maximum when $S$ is a disc.
- $R(S = Disc) = \frac{\pi r^{2}}{(2 \pi r)^{2}} = \frac{1}{4 \pi}$

Therefore, we can normalise this expression by dividing it by $R(Disc)$.
In conclusion, we will define the circularity $C_{irc}(S)$ of a convex shape $S$ as:

$$
    C_{irc}(S) = 4 \pi\frac{A(S)}{L(S)^{2}}
$$

We thus have:
- $ 0 < C_{irc}(S) \leq 1, \forall S$
- $C_{irc}(Disc) = 1$

Note that since in the assignement we use convex hulls which are convex polygons, the maximum cannot be reached, but can be approximated with a high enough number of vertices.

###### Compare results between different rice types.

Note: See `res/td2/` after running the programme for visuals.

Here is a final sample of the output of the programme:

    ---- MORE STATISTICS -----
    -- rice_basmati_seg_bin.pgm --
    [Area (Seg)]         avg: 3603.95  deviation: 848.445
    [Perimeter (Seg)]    avg: 226.145  deviation: 34.1156
    [Circularity]    avg: 0.883652  deviation:  0.146412
    -- rice_camargue_seg_bin.pgm --
    [Area (Seg)]         avg: 3261.8  deviation: 837.072
    [Perimeter (Seg)]    avg: 207.873  deviation: 35.7147
    [Circularity]    avg: 0.934208  deviation:  0.082935
    -- rice_japanese_seg_bin.pgm --
    [Area (Seg)]         avg: 2245.44  deviation: 205.165
    [Perimeter (Seg)]    avg: 171.2  deviation: 10.7742
    [Circularity]    avg: 0.965036  deviation:  0.0762395

As expected, the more round-shaped japanese rice grains have larger circularity on average than the others, while carmague rice grains are half-way between.
The Circularity criteria proves to be just about as good as using perimeter (taking deviation into account.) Experience has shown that it performs better when using the count method for computing areas and perimeters, but it should not generalise. The latter method should logically not perform well for more round shapes like the japanese grains, but it was not the case in our example. In the end, we kept the Segmentation method.
