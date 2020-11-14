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

With the area of the triangle being half of the sum of its base and height.
The base b_{i} is:

    b_{i} = |p_{i+1} - p_{i}|
    
The height h_{i} is:

    h_{i} =  |(p_{i+1} + p_{i}) / 2|
    
We find:
 
    A(t_{i}) = (2 * |(p_{i+1} - p_{i})| + |(p_{i+1} + p_{i})|) / 4
    
And thus:

    A(P) = (2 * sum( |(p_{i+1} - p_{i})| ) + sum( |(p_{i+1} + p_{i})| )) / 4
         = (2 * L(p) + sum( [|(p_{i+1} + p_{i})| )) / 4