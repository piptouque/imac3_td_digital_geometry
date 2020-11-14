### Digital Geometry

##### M2 SIS, UGE

### Laboratory Session 1

#### November 13th, 2020

----

### Experiments

#### Step 1

#####  Can we define a convex polygon using implicit functions?

We can express the shape of a convex polygon P as: 

    P = {(x, y) ∈ R 2 : V f ∈ F,  f(x, y) ≤ 0}

With F a set of *n* implicit functions, each defining a line,
and a half-plane in Cartesian coordinates. \a and \b are real numbers.

    f(x, y) = ax - by
   

##### Can we exactly calculate the area and perimeter of such Euclidean shapes defined by
##### implicit functions? If so, please give their formulas in the report.

We can first compute the intersection of each lines which gives us the vertices of our polygon.
    
todo: explain

Once we have a (p_{i}), i ∈ [|0, n|], we can compute the perimeter L of the polygon 
by the sum of le length of its sides (1-cells).
By acknowledging:

    p_{n+1} = p_{0}
    
We find:

    L(p) = sum( |p_{i+1} - p_{i}|, i ∈ [|0, n|] )
    
With the same convention on p, and with O the origin of our referential,
we can define a set of *n* triangles by their respective vertices:

    t_{i} = {p_{i}, O, p_{i+1}}, i ∈ [|0, n|]
    
The area covered by the (t_{i}) form a 'partition' of the area of P, we can thus write:

    A(P) = sum( A(t_{i}), i ∈ [|0, n|] )

With the area of the triangle being half of the product of its base and height.
The base b_{i} is:

    b_{i} = |p_{i+1} - p_{i}|
    
The height h_{i} is:

    h_{i} =  |(p_{i+1} + p_{i})| / 2
    
We find:
 
    A(t_{i}) = b_{i} * h_{i} / 2
    A(t_{i}) = (|(p_{i+1} - p_{i})| * |(p_{i+1} + p_{i})|) / 4
    
And thus:

    A(P) = sum( |(p_{i+1} - p_{i})| * |(p_{i+1} + p_{i})| )) / 4
   
Note that the above formula does not depend on the Euclidian metric used.
In case of an L2 norm, we can simplify it like so:


    A(P) = sum( sqrt( (p_{i+1}^{2} - p_{i}^{2})^{2}) ) / 4
         = sum( |(p_{i+1}^{2} - p_{i}^{2})| ) / 4
   
##### Boundaries 

![Boundaries](../res/Boundaries.png)

##### Remarks

By increasing the grid size we also increase the digitalised circle's shape accuracy.
The digitalised square does not benefit from this, 
because it is already identical to the original.

#### Step 3

*Note: Greater grid step means lower digital resolution.*

![Areas](../res/ErrorArea.png)


As expected, we see that by reducing the grid step, we obtain more accurate shapes on average.

![Perimetres](../res/ErrorPerimetre.png)

Once again, the Square shows more accurate results as the grid step decreases.
Perhaps less expected, the error of the Disc converges towards a non-null value.
As we saw in class, the perimetre of the sampled Disk should be
constant equal to that of a Square of the same dimensions,
because when reorganised, the curves are of the same length.

#### Step 4

![DiscHull](../res/ConvexHullDisc.png)

![SquareHull](../res/ConvexHullSquare.png)

The results are good for already convex polygons, as expected.
However, using any other shapes would introduce an error.
For instance, using shapes with holes, or any concave shape.

#### Step 5

![Areas](../res/ErrorAreaConvexHulls.png)

No changes with the Areas, we get the same results as with the original digital shapes.

![Perimetres](../res/ErrorPerimetreConvexHulls.png)

The perimetre of the Square converges just like with the digital shape.
We can see that this time the perimetre of the Disc is more accurate,
it's clear when looking at the drawings, because we are using broken lines instead
of only using vertical and horizontal lines.
