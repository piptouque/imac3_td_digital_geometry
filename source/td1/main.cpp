///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <algorithm>
///////////////////////////////////////////////////////////////////////////////

#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"

//shape and digitizer
#include "DGtal/shapes/Shapes.h"
#include "DGtal/shapes/ShapeFactory.h"
#include "DGtal/shapes/GaussDigitizer.h"

//tracking grid curve
#include "DGtal/topology/helpers/Surfaces.h"
#include "DGtal/geometry/curves/GridCurve.h"

#include "DGtal/io/boards/Board2D.h"
///////////////////////////////////////////////////////////////////////////////

using namespace DGtal;
using namespace Z2i;

int main()
{
  // define an Euclidean shape (disk)
  typedef ImplicitBall<Z2i::Space> Disk;
  ImplicitBall<Z2i::Space> disk(Z2i::Point(0,0), 10);

  // Gauss discretization
  double h = 1;
  GaussDigitizer<Z2i::Space,Disk> dig;
  dig.attach( disk );
  dig.init( disk.getLowerBound()+Z2i::Vector(-1,-1),
            disk.getUpperBound()+Z2i::Vector(1,1), h );
  
  Domain domain(dig.getLowerBound(), dig.getUpperBound());

  // make a Kovalevsky-Khalimsky space
  Z2i::KSpace ks;
  ks.init( dig.getLowerBound(), dig.getUpperBound(), true );
  // set an adjacency (4-connectivity)
  SurfelAdjacency<2> sAdj( true );

  // search for one boundary element
  Z2i::SCell bel = Surfaces<Z2i::KSpace>::findABel( ks, dig, 1000 );
  // boundary tracking
  std::vector<Z2i::Point> boundaryPoints;
  Surfaces<Z2i::KSpace>
    ::track2DBoundaryPoints( boundaryPoints, ks, sAdj, dig, bel );

  Z2i::Curve c;
  c.initFromVector( boundaryPoints );  

  // draw a boundary curve and make a pdf file
  Board2D aBoard;
  aBoard << c;
  aBoard.saveCairo("boundaryCurve.pdf", LibBoard::Board::CairoPDF);
}

///////////////////////////////////////////////////////////////////////////////
