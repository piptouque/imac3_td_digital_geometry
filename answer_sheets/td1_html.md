###  Digital Geometry
  
  
#####  M2 SIS, UGE
  
  
###  Laboratory Session 1
  
  
####  November 13th, 2020
  
  
----
  
###  Experiments
  
  
####  Step 1
  
  
#####   Can we define a convex polygon using implicit functions?
  
  
We can express the shape of a convex polygon <img src="https://latex.codecogs.com/gif.latex?P"/> as:
  
<p align="center"><img src="https://latex.codecogs.com/gif.latex?P%20=%20&#x5C;{(x,%20y)%20&#x5C;in%20%20&#x5C;Reals^{2}%20:%20&#x5C;forall%20f%20∈%20F,%20%20f(x,%20y)%20&#x5C;leq%200&#x5C;}"/></p>  
  
  
With <img src="https://latex.codecogs.com/gif.latex?F"/> a set of <img src="https://latex.codecogs.com/gif.latex?n"/> implicit functions, each defining a line,
and a half-plane in Cartesian coordinates. <img src="https://latex.codecogs.com/gif.latex?a"/> and <img src="https://latex.codecogs.com/gif.latex?b"/> are real numbers.
  
<p align="center"><img src="https://latex.codecogs.com/gif.latex?f(x,%20y)%20=%20ax%20-%20by"/></p>  
  
  
#####  Can we exactly calculate the area and perimeter of such Euclidean shapes defined by
  
#####  implicit functions? If so, please give their formulas in the report.
  
  
We can first compute the intersection of each lines which gives us the vertices of our polygon.
How we would proceed is, we would compute all the intersections of the lines (there are at most <img src="https://latex.codecogs.com/gif.latex?2n"/>),
then find the closest one to the origin, we'll call it <img src="https://latex.codecogs.com/gif.latex?p_{0}"/>.
  
We know that this intersection is a vertex of our polygon, because if it was not there would be
two other points further away from the origin, with a face in the direction of <img src="https://latex.codecogs.com/gif.latex?p_{0}"/>
from the origin, and the shape would not be convex.
  
From there, we choose as <img src="https://latex.codecogs.com/gif.latex?p_{1}"/> the intersection with another line such that <img src="https://latex.codecogs.com/gif.latex?p_{1}"/> is the next closest to the origin.
This defines the direction of the traversal, and we follow up with the <img src="https://latex.codecogs.com/gif.latex?p_{i},%20i%20&#x5C;in%20&#x5C;llbracket%200,%20n%20&#x5C;rrbracket"/>.
  
  
Once we have a <img src="https://latex.codecogs.com/gif.latex?(p_{i})"/>, we can compute the perimeter <img src="https://latex.codecogs.com/gif.latex?L"/> of the polygon by the sum of le length of its sides (1-cells).
By acknowledging:
  
<p align="center"><img src="https://latex.codecogs.com/gif.latex?p_{n+1}%20=%20p_{0}"/></p>  
  
  
We find:
  
<p align="center"><img src="https://latex.codecogs.com/gif.latex?L(p)%20=%20&#x5C;sum_{i=0}^{n}%20|p_{i+1}%20-%20p_{i}|"/></p>  
  
  
With the same convention on p, and with O the origin of our referential,
we can define a set of <img src="https://latex.codecogs.com/gif.latex?n"/> triangles by their respective vertices:
  
<p align="center"><img src="https://latex.codecogs.com/gif.latex?t_{i}%20=%20&#x5C;{p_{i},%200,%20p_{i+1}&#x5C;},%20i%20&#x5C;in%20&#x5C;llbracket%200,%20n%20&#x5C;rrbracket"/></p>  
  
  
The area covered by the <img src="https://latex.codecogs.com/gif.latex?(t_{i})"/> form a 'partition' of the area of P, we can thus write:
  
<p align="center"><img src="https://latex.codecogs.com/gif.latex?A(P)%20=%20&#x5C;sum_{i=0}^{n}%20A(t_{i})"/></p>  
  
  
With the area of the triangle being half of the product of its base and height.
The base <img src="https://latex.codecogs.com/gif.latex?b_{i}"/> is:
  
<p align="center"><img src="https://latex.codecogs.com/gif.latex?b_{i}%20=%20|p_{i+1}%20-%20p_{i}|"/></p>  
  
  
The height <img src="https://latex.codecogs.com/gif.latex?h_{i}"/> is:
  
<p align="center"><img src="https://latex.codecogs.com/gif.latex?h_{i}%20=%20%20&#x5C;frac{|p_{i+1}%20+%20p_{i}|}{2}"/></p>  
  
  
We find:
  
<p align="center"><img src="https://latex.codecogs.com/gif.latex?A(t_{i})%20=%20&#x5C;frac{b_{i}%20h_{i}}{2}%20%20%20%20&#x5C;&#x5C;%20%20%20%20A(t_{i})%20=%20&#x5C;frac{1}{4}%20|p_{i+1}%20-%20p_{i}|%20|p_{i+1}%20+%20p_{i}|"/></p>  
  
  
And thus:
  
<p align="center"><img src="https://latex.codecogs.com/gif.latex?A(P)%20=%20&#x5C;frac{1}{4}%20&#x5C;sum_{i=0}^{n}%20|p_{i+1}%20-%20p_{i}|%20|p_{i+1}%20+%20p_{i}|"/></p>  
  
  
Note that the above formula does not depend on the Euclidean metric used.
In case of an L2 norm, we can simplify it like so:
  
<p align="center"><img src="https://latex.codecogs.com/gif.latex?A(P)%20=%20&#x5C;frac{1}{4}%20&#x5C;sum_{i=0}^{n}%20&#x5C;sqrt{(p_{i+1}^{2}%20-%20p_{i}^{2})^{2}}%20%20%20%20%20%20%20%20%20=%20&#x5C;frac{1}{4}%20&#x5C;sum_{i=0}^{n}%20|p_{i+1}^{2}%20-%20p_{i}^{2}|"/></p>  
  
  
#####  Boundaries
  
  
![Boundaries](../res/Boundaries.png )
  
#####  Remarks
  
  
By increasing the grid size we also increase the digitalised circle's shape accuracy.
The digitalised square does not benefit from this,
because it is already identical to the original.
  
####  Step 3
  
  
Notes:
- Greater grid step means lower digital resolution.
- For the rest of this document, the square's size will be half of the disc's, the difference in scales.
  
![Areas](../res/ErrorArea.png )
  
  
As expected, we see that by reducing the grid step, we obtain more accurate shapes on average.
  
![Perimetres](../res/ErrorPerimetre.png )
  
Once again, the Square shows more accurate results as the grid step decreases.
Perhaps less expected, the error of the Disc converges towards a non-null value.
As we saw in class, the perimetre of the sampled Disk should be
constant equal to that of a Square of the same dimensions,
because when reorganised, the curves are of the same length.
  
####  Step 4
  
  
![DiscHull](../res/ConvexHullDisc.png )
  
![SquareHull](../res/ConvexHullSquare.png )
  
The results are good for the digitalised convex shapes (which are not always convex themselves,
for instance the Disc is only starred). Indeed, the error that we get are due solely to the digitalisation
process.
However, using any other shapes would introduce an error.
For instance, using shapes with holes, or any concave shape.
  
####  Step 5
  
  
![Areas](../res/ErrorAreaConvexHulls.png )
  
No changes with the Areas, we get the same results as with the original digital shapes.
  
![Perimetres](../res/ErrorPerimetreConvexHulls.png )
  
The perimetre of the Square converges just like with the digital shape.
We can see that this time the perimetre of the Disc is more accurate,
it's clear when looking at the drawings, because we are using broken lines instead
of only using vertical and horizontal lines.
  