//----------------------------------------------------------------------
//  $Id: TCOpenGLGridRenderer.cpp 766 2007-10-24 22:42:17Z the_____tiger $
//----------------------------------------------------------------------
//
//  $Log$
//  Revision 1.1  2003/07/18 20:34:01  the_____tiger
//  updated some methodes
//
//  Revision 1.23  2002/08/22 12:02:43  GOESSLET
//  all renderers have as baseclass now TCOglRenderer
//
//  Revision 1.22  2002/07/24 12:06:57  larameer
//  modified method: constructor
//
//  Revision 1.21  2002/07/24 11:26:48  larameer
//  added methods:
//    AntiAliasOn()
//    ToggleAntiAlias()
//  modified methods()
//    render9oclockIn*plane()
//    renderIn*plane()
//    renderDiagonal*()
//    renderInPlane*()
//
//  Revision 1.20  2002/07/16 12:43:12  larameer
//  turned off testing flags
//
//  Revision 1.19  2002/06/03 14:40:16  larameer
//  turned off some debugging functionality
//
//  Revision 1.18  2002/05/29 13:53:48  larameer
//  added methods:
//    render9oclockInArbiPlane()
//    render9oclockInXYplane()
//    render9oclockInXZplane()
//    render9oclockInYZplane()
//    render9oclockTransformedToXYplane()
//    renderInPlaneTranformedToXY()
//  removed all *12oclock() methods
//
//  Revision 1.17  2002/05/24 17:26:20  larameer
//  added methods:
//    render12oclockInArbiPlane()
//    render12oclockInXYplane()
//    render12oclockInXZplane()
//    render12oclockInYZplane()
//  modified methods:
//    renderInXZplane()
//    renderInYZplane()
//    renderInXYplane()
//    renderInArbiPlane()
//
//  Revision 1.16  2002/05/22 14:52:39  larameer
//  modified method: renderInPlaneAribtrary(polar)
//
//  Revision 1.15  2002/05/16 13:21:43  GOESSLET
//  merged TCColor and TCOglColor to one class named TCOglColor
//  I hope I did not implememnt some bugs with this change
//
//  Revision 1.14  2002/05/14 13:03:32  larameer
//  added methods:
//    renderInXYplane()
//    renderInXYplane()
//    renderInYZplane()
//    renderInPlaneArbitrary()
//
//  Revision 1.13  2002/05/13 14:13:17  larameer
//  added methods:
//    Render()
//    renderInYZ()
//  modified methods:
//    renderInYZ()
//    renderInXY()
//    renderInXZ()
//    renderDiagonalUserTransformed()
//
//  Revision 1.12  2002/05/07 13:43:47  larameer
//  modified method:
//    Render()
//    renderInPlaneArbitrary()
//
//  Revision 1.11  2002/05/06 13:58:00  larameer
//  added method:
//    renderDiagonalInXYplane()
//
//  modified method:
//    renderInPlaneTransformedToXY()
//
//  Revision 1.10  2002/05/03 13:27:45  larameer
//  modified method: Render()
//
//  Revision 1.9  2002/05/02 16:59:37  larameer
//  added methods:
//    renderDiagonalOriginal()
//    renderDiagonalTransformedToXY()
//    renderDiagonalUserTransformed()
//    renderInPlaneArbitrary()
//    renderInPlaneTransformedToXY()
//
//  modified method(s):
//    Render()
//    renderInXY()
//    renderInXZ()
//    renderInYZ()
//
//  Revision 1.8  2002/04/30 16:56:28  larameer
//  added method: renderInArbitraryPlane()
//  modified method: Render()
//
//  Revision 1.7  2002/04/16 09:38:09  larameer
//  modified method: RenderCircle()
//
//  Revision 1.6  2002/04/11 13:59:56  larameer
//  added methods:
//    RenderTriangle()
//    RenderCircle()
//
//  Revision 1.5  2002/03/25 18:46:08  GOESSLET
//  fixed compile error on sgi
//
//  Revision 1.4  2002/03/25 14:46:36  larameer
//  added method:  shiftAndRotate()
//  modified methods:
//    GetXYgridCoord()
//    GetXZgridCoord()
//    GetYZgridCoord()
//
//  Revision 1.3  2002/03/21 15:22:27  larameer
//  added method(s):
//    Render()
//    renderInXY()
//    renderInXZ()
//    renderInYZ()
//
//  removed methods:
//    SetNumColumns()
//    SetNumRows()
//
//  Revision 1.2  2002/03/19 15:12:11  larameer
//  added methods:
//    constructor
//    destructor
//    SetNumColumns
//    SetNumRows
//
//  Revision 1.1  2002/03/18 08:07:06  larameer
//  added new empty files in order to release the lock
//  on the OglViewer.shared project
//
//----------------------------------------------------------------------

#include "TCGridCartesian.h"        /* for Render() method         */
#include "TCGridPolar.h"            /* for other Render() method   */
#include "TCOpenGLColor.h"                /* for renderInXY() method     */
#include "TCOpenGLViewer.h"                /* for getViewerPtr() method     */
#include "TCTriangleLightWeight.h"  /* for RenderTriangle() method */
#include "TCOpenGLGridRenderer.h"

/*
 * @see TCOglGridRenderer.h for comments
 */
TCOglGridRenderer::TCOglGridRenderer(TCOglViewer* viewer)

   /* explicit initialization */
   :TCOglRenderer(viewer),
    m_GridColor(TCOglColor::WHITE),
    m_AntiAliasOn(false) /* off by default */ {

}
/*
 * @see TCOglGridRenderer.h for comments
 */
TCOglGridRenderer::~TCOglGridRenderer() {

}
/*
 * @see TCOglGridRenderer.h for comments
 */
void TCOglGridRenderer::ToggleAntiAlias() {

   if (m_AntiAliasOn == true) {
      m_AntiAliasOn = false;
   } else {
      m_AntiAliasOn = true;
   }
}

/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::Render(TCGridPolar& gridPolar) {

   bool debug = false;
   sint32 plane = gridPolar.GetContainingPlane();
   
   if (debug) {
      tcerr << "TCOglGridRenderer::Render(polar grid) ";
      tcerr << " plane: " << plane << endl;
   }
   
   switch(plane) {

      case(TCGrid::YZ_PLANE):
         return this->renderInYZplane(gridPolar);

      case(TCGrid::XY_PLANE):
         return this->renderInXYplane(gridPolar);

      case(TCGrid::XZ_PLANE):
         return this->renderInXZplane(gridPolar);

      case(TCGrid::ARBITRARY_PLANE):
         this->renderInPlaneArbitrary(gridPolar);
         if (debug) this->renderInPlaneTransformedToXY(gridPolar);
         return true;

      default:
         tcerr << "*** Warning TCOglGridRenderer::Render(polar grid) unrecognized plane." << endl;
         return false;
   }
         
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::Render(TCGridCartesian& gridCartesian) {

   bool debug = false;
   sint32 plane = gridCartesian.GetContainingPlane();
   
   if (debug) {
      tcerr << "TCOglGridRenderer::Render(cartesian grid) ";
      tcerr << " plane: " << plane << endl;
   }
   
   switch(plane) {

      case(TCGrid::YZ_PLANE):
         return this->renderInYZ(gridCartesian);

      case(TCGrid::XY_PLANE):
         return this->renderInXY(gridCartesian);

      case(TCGrid::XZ_PLANE):
         return this->renderInXZ(gridCartesian);

      case(TCGrid::ARBITRARY_PLANE):
         this->renderInPlaneArbitrary(gridCartesian);
         /* this->renderInPlaneTransformedToXY(gridCartesian); */
         return true;

      default:
         tcerr << "*** Warning TCOglGridRenderer::Render(cartesian grid) unrecognized plane." << endl;
         return false;
   }
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::RenderTriangle(const TCTriangleLightWeight& triangleLW) {

   bool debug = false;
   std::vector< TC::Math::Coord3D< float > > triangleCoords3D;
   triangleCoords3D.SetNumElems(0);
   TCOglColor black(TCOglColor::BLACK);
   
   if (debug) tcerr << "TCOglGridRenderer::RenderTriangle()" << endl;

   triangleCoords3D.push_back(triangleLW.GetVertexCoord(TCTriangleLightWeight::VERTEX0));
   triangleCoords3D.push_back(triangleLW.GetVertexCoord(TCTriangleLightWeight::VERTEX1));
   triangleCoords3D.push_back(triangleLW.GetVertexCoord(TCTriangleLightWeight::VERTEX2));

   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineLoopAntiAliased(
                                             triangleCoords3D, black);
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineLoop(
                                             triangleCoords3D, black);
   }
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::RenderCircle(const TC::Math::Coord3D<float>& center,
                                         const float radius,
                                         const TCOglColor color) {

   bool debug = false;

   if (debug) tcerr << "TCOglGridRenderer::RenderCircle()" << endl;

   getViewerPtr()->GetPointRendererRef().RenderCircle(center, radius, color);
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::renderInYZplane(TCGridPolar& gridPolar) {

   bool debug = false;
   std::vector< TC::Math::Coord3D< float > > ringCoords3D;
   std::vector< TC::Math::Coord3D< float > > spokeCoords3D;
   ringCoords3D.SetNumElems(0);
   spokeCoords3D.SetNumElems(0);
   
   if (false) tcerr << "TCOglGridRenderer::renderInYZplane()" << endl;
   if (debug) this->renderDiagonalUserTransformed(gridPolar); /* for testing */

   /* Draw the rings -1 line loop per ring. */
   /* FOR EACH ring in the polar grid.      */
   for(TCShort r = 0; r <= gridPolar.GetNumRings(); r++) {
   ringCoords3D.SetNumElems(0);

      /* Accumulate 1 ring's worth of coordinates. */
      for(TCShort s = 0; s <= gridPolar.GetNumSlices(); s++) {

         ringCoords3D.push_back(gridPolar.ComputeYZgridCoordUserTransformed(r,s));
      }
      /* Render 1 ring's worth of coordinates. */
      if (this->AntiAliasOn()) {
         getViewerPtr()->GetLineRendererRef().RenderLineLoopAntiAliased(
                                    ringCoords3D, this->getColorRef());
      } else {
         getViewerPtr()->GetLineRendererRef().RenderLineLoop(
                                    ringCoords3D, this->getColorRef());
      }
   }
   /* 2. Draw the spokes/slices. */
   /* FOR EACH slice in the polar grid. */
   for(TCShort s = 1; s < gridPolar.GetNumSlices(); s++) {

      spokeCoords3D.push_back(gridPolar.ComputeCenterCoordUserTransformed());
      spokeCoords3D.push_back(
         gridPolar.ComputeYZgridCoordUserTransformed(gridPolar.GetNumRings(), s));
   }
   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                                 spokeCoords3D, this->getColorRef());
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                                 spokeCoords3D, this->getColorRef());
   }
   if (debug) this->render9oclockInYZplane(gridPolar);
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::render9oclockInYZplane(TCGridPolar& gridPolar) {

   std::vector< TC::Math::Coord3D< float > > spokeCoords3D;

   spokeCoords3D.push_back(gridPolar.ComputeCenterCoordUserTransformed());
   spokeCoords3D.push_back(
         gridPolar.ComputeYZgridCoordUserTransformed(gridPolar.GetNumRings(), 0));

   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                     spokeCoords3D, TCOglColor(TCOglColor::ORANGE));
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                     spokeCoords3D, TCOglColor(TCOglColor::ORANGE));
   }
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::render9oclockInArbiPlane(TCGridPolar& gridPolar) {

   std::vector< TC::Math::Coord3D< float > > spokeCoords3D;

   spokeCoords3D.push_back(
         gridPolar.ComputeArbiGridCoordUserTransformed(0, 0));
   spokeCoords3D.push_back(
         gridPolar.ComputeArbiGridCoordUserTransformed(gridPolar.GetNumRings(), 0));

   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                     spokeCoords3D, TCOglColor(TCOglColor::ORANGE));
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                     spokeCoords3D, TCOglColor(TCOglColor::ORANGE));
   }
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::render9oclockInXZplane(TCGridPolar& gridPolar) {

   std::vector< TC::Math::Coord3D< float > > spokeCoords3D;

   spokeCoords3D.push_back(gridPolar.ComputeCenterCoordUserTransformed());
   spokeCoords3D.push_back(
         gridPolar.ComputeXZgridCoordUserTransformed(gridPolar.GetNumRings(), 0));

   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                     spokeCoords3D, TCOglColor(TCOglColor::ORANGE));
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                     spokeCoords3D, TCOglColor(TCOglColor::ORANGE));
   }
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::render9oclockInXYplane(TCGridPolar& gridPolar) {

   std::vector< TC::Math::Coord3D< float > > spokeCoords3D;

   spokeCoords3D.push_back(gridPolar.ComputeCenterCoordUserTransformed());
   spokeCoords3D.push_back(
         gridPolar.ComputeXYgridCoordUserTransformed(gridPolar.GetNumRings(), 0));

   if (this->AntiAliasOn()) {
         getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                     spokeCoords3D, TCOglColor(TCOglColor::ORANGE));
   } else {
         getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                     spokeCoords3D, TCOglColor(TCOglColor::ORANGE));
   }
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::render9oclockTransformedToXYplane(TCGridPolar& gridPolar) {

   std::vector< TC::Math::Coord3D< float > > spokeCoords3D;

   spokeCoords3D.push_back(
         gridPolar.ComputeGridCoordTransformedToXYplaneUserTransformed(0,0));
   spokeCoords3D.push_back(
         gridPolar.ComputeGridCoordTransformedToXYplaneUserTransformed(gridPolar.GetNumRings(), 0));

   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                  spokeCoords3D, TCOglColor(TCOglColor::ORANGE));
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                  spokeCoords3D, TCOglColor(TCOglColor::ORANGE));
   }
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::renderInXZplane(TCGridPolar& gridPolar) {

   bool debug = false;
   std::vector< TC::Math::Coord3D< float > > ringCoords3D;
   std::vector< TC::Math::Coord3D< float > > spokeCoords3D;
   ringCoords3D.SetNumElems(0);
   spokeCoords3D.SetNumElems(0);
   
   if (debug) this->renderDiagonalUserTransformed(gridPolar); /* for testing */

   /* Draw the rings -1 line loop per ring. */
   /* FOR EACH ring in the polar grid.      */
   for(TCShort r = 0; r <= gridPolar.GetNumRings(); r++) {
   ringCoords3D.SetNumElems(0);

      /* Accumulate 1 ring's worth of coordinates. */
      for(TCShort s = 0; s <= gridPolar.GetNumSlices(); s++) {

         ringCoords3D.push_back(gridPolar.ComputeXZgridCoordUserTransformed(r, s));
      }
      /* Render 1 ring's worth of coordinates. */
      if (this->AntiAliasOn()) {
         getViewerPtr()->GetLineRendererRef().RenderLineLoopAntiAliased(
                                 ringCoords3D, this->getColorRef());
      } else {
         getViewerPtr()->GetLineRendererRef().RenderLineLoop(
                                 ringCoords3D, this->getColorRef());
      }
   }
   /* 2. Draw the spokes/slices. */
   /* FOR EACH slice in the polar grid. */
   for(TCShort s = 0; s <= gridPolar.GetNumSlices(); s++) {

      spokeCoords3D.push_back(gridPolar.ComputeCenterCoordUserTransformed());
      spokeCoords3D.push_back(
         gridPolar.ComputeXZgridCoordUserTransformed(gridPolar.GetNumRings(), s));
   }
   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                              spokeCoords3D, this->getColorRef());
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                              spokeCoords3D, this->getColorRef());
   }
   if (debug) this->render9oclockInXZplane(gridPolar);
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::renderInXYplane(TCGridPolar& gridPolar) {

   bool debug = false;
   std::vector< TC::Math::Coord3D< float > > ringCoords3D;
   std::vector< TC::Math::Coord3D< float > > spokeCoords3D;
   
   if (debug) this->renderDiagonalUserTransformed(gridPolar); /* for testing */

   /* Draw the rings -1 line loop per ring. */
   /* FOR EACH ring in the polar grid.      */
   for(TCShort r = 0; r <= gridPolar.GetNumRings(); r++) {
   ringCoords3D.SetNumElems(0);

      /* Accumulate 1 ring's worth of coordinates. */
      for(TCShort s = 0; s <= gridPolar.GetNumSlices(); s++) {

         ringCoords3D.push_back(gridPolar.ComputeXYgridCoordUserTransformed(r,s));
      }
      /* Render 1 ring's worth of coordinates. */
      if (this->AntiAliasOn()) {
         getViewerPtr()->GetLineRendererRef().RenderLineLoopAntiAliased(
                                 ringCoords3D, this->getColorRef());
      } else {
         getViewerPtr()->GetLineRendererRef().RenderLineLoop(
                                 ringCoords3D, this->getColorRef());
      }
   }
   /* 2. Draw the spokes/slices. */
   /* FOR EACH slice in the polar grid. */
   for(TCShort s = 0; s <= gridPolar.GetNumSlices(); s++) {

      spokeCoords3D.push_back(gridPolar.ComputeCenterCoordUserTransformed());
      spokeCoords3D.push_back(
         gridPolar.ComputeXYgridCoordUserTransformed(gridPolar.GetNumRings(),s));
   }
   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                                 spokeCoords3D, this->getColorRef());
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                                 spokeCoords3D, this->getColorRef());
   }
   if (debug) this->render9oclockInXYplane(gridPolar);
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::renderInPlaneArbitrary(TCGridPolar& gridPolar) {

   bool debug = true;
   std::vector< TC::Math::Coord3D< float > > ringCoords3D;
   std::vector< TC::Math::Coord3D< float > > spokeCoords3D;
   ringCoords3D.SetNumElems(0);
   spokeCoords3D.SetNumElems(0);
   
   /* Draw the rings -1 line loop per ring. */
   /* FOR EACH ring in the polar grid.      */
   for(TCShort r = 0; r <= gridPolar.GetNumRings(); r++) {
   ringCoords3D.SetNumElems(0);

      /* Accumulate 1 ring's worth of coordinates. */
      for(TCShort s = 0; s <= gridPolar.GetNumSlices(); s++) {

         ringCoords3D.push_back(gridPolar.ComputeArbiGridCoordUserTransformed(r,s));
      }
      /* Render 1 ring's worth of coordinates. */
      if (this->AntiAliasOn()) {
            getViewerPtr()->GetLineRendererRef().RenderLineLoopAntiAliased(
                                    ringCoords3D, this->getColorRef());
      } else {
         getViewerPtr()->GetLineRendererRef().RenderLineLoop(
                                    ringCoords3D, this->getColorRef());
      }
   }
   /* 2. Draw the spokes/slices. */
   /* FOR EACH slice in the polar grid. */
   for(TCShort s = 0; s <= gridPolar.GetNumSlices(); s++) {

      spokeCoords3D.push_back(
         gridPolar.ComputeArbiGridCoordUserTransformed(0, 0));
      spokeCoords3D.push_back(
         gridPolar.ComputeArbiGridCoordUserTransformed(gridPolar.GetNumRings(),s));
   }
   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                                 spokeCoords3D, this->getColorRef());
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                                 spokeCoords3D, this->getColorRef());
   }
   if (debug) this->render9oclockInArbiPlane(gridPolar);
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::renderInYZ(TCGridCartesian& gridCartesian) {

   bool debug = false;
   TCShort i = 0;
   std::vector< TC::Math::Coord3D< float > > segmentsCoords3D;
   segmentsCoords3D.SetNumElems(0);
   
   if (debug) tcerr << "TCOglGridRenderer::renderInYZ()" << endl;
   if (debug) this->renderDiagonalUserTransformed(gridCartesian); /* for testing */
   
   /* render each grid line along a row */
   for(i = 0; i <= gridCartesian.GetNumRows(); i++) {
   
      segmentsCoords3D.push_back(gridCartesian.GetYZgridCoord(i, 0));
      segmentsCoords3D.push_back(gridCartesian.GetYZgridCoord(i, gridCartesian.GetNumColumns()));
   }
   /* render each grid line along a column */
   for(i = 0; i <= gridCartesian.GetNumColumns(); i++) {
   
      segmentsCoords3D.push_back(gridCartesian.GetYZgridCoord(0, i));
      segmentsCoords3D.push_back(gridCartesian.GetYZgridCoord(gridCartesian.GetNumRows(), i));
   }

   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                           segmentsCoords3D, this->getColorRef());
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                           segmentsCoords3D, this->getColorRef());
   }
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::renderInPlaneArbitrary(TCGridCartesian& gridCartesian) {

   bool debug = false;
   TCShort i = 0;
   std::vector< TC::Math::Coord3D< float > > segmentsCoords3D;
   segmentsCoords3D.SetNumElems(0);

   if (false) tcerr << "TCOglGridRenderer::renderInPlaneArbitrary(cartesian)" << endl;
   if (debug) this->renderDiagonalOriginal(gridCartesian); /* for testing */
   if (debug) this->renderDiagonalUserTransformed(gridCartesian); /* for testing */

   /* render each grid line along a row */
   for(i = 0; i <= gridCartesian.GetNumRows(); i++) {
   
      segmentsCoords3D.push_back(gridCartesian.GetGridCoordInArbiPlane(i, 0));
      segmentsCoords3D.push_back(gridCartesian.GetGridCoordInArbiPlane(i, gridCartesian.GetNumColumns()));
   }
   /* render each grid line along a column */
   for(i = 0; i <= gridCartesian.GetNumColumns(); i++) {
   
      segmentsCoords3D.push_back(gridCartesian.GetGridCoordInArbiPlane(0, i));
      segmentsCoords3D.push_back(gridCartesian.GetGridCoordInArbiPlane(gridCartesian.GetNumRows(), i));
   }
   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                              segmentsCoords3D, this->getColorRef());
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                              segmentsCoords3D, this->getColorRef());
   }
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::renderInPlaneTransformedToXY(TCGridCartesian& gridCartesian) {

   bool debug = true;
   TCShort i = 0;
   std::vector< TC::Math::Coord3D< float > > segmentsCoords3D;
   segmentsCoords3D.SetNumElems(0);

   if (false) tcerr << "TCOglGridRenderer::renderInPlaneTransformedToXY()" << endl;
   if (debug) this->renderDiagonalInXYplane(gridCartesian); /* for testing */

   /* render each grid line along a row */
   for(i = 0; i <= gridCartesian.GetNumRows(); i++) {
   
      segmentsCoords3D.push_back(gridCartesian.GetGridCoordInXYplane(i, 0));
      segmentsCoords3D.push_back(gridCartesian.GetGridCoordInXYplane(i, gridCartesian.GetNumColumns()));
   }
   /* render each grid line along a column */
   for(i = 0; i <= gridCartesian.GetNumColumns(); i++) {
   
      segmentsCoords3D.push_back(gridCartesian.GetGridCoordInXYplane(0, i));
      segmentsCoords3D.push_back(gridCartesian.GetGridCoordInXYplane(gridCartesian.GetNumRows(), i));
   }
   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                     segmentsCoords3D, TCOglColor(TCOglColor::BLACK));
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                     segmentsCoords3D, TCOglColor(TCOglColor::BLACK));
   }
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::renderInPlaneTransformedToXY(TCGridPolar& gridPolar) {

   bool debug = true;
   std::vector< TC::Math::Coord3D< float > > spokeCoords3D;
   std::vector< TC::Math::Coord3D< float > > ringCoords3D;

   if (false) tcerr << "TCOglGridRenderer::renderInPlaneTransformedToXY(polar)" << endl;

   /* Draw the rings -1 line loop per ring. */
   /* FOR EACH ring in the polar grid.      */
   for(TCShort r = 0; r <= gridPolar.GetNumRings(); r++) {
   ringCoords3D.SetNumElems(0);

      /* Accumulate 1 ring's worth of coordinates. */
      for(TCShort s = 0; s <= gridPolar.GetNumSlices(); s++) {

         ringCoords3D.push_back(
            gridPolar.ComputeGridCoordTransformedToXYplaneUserTransformed(r,s));
      }
      /* Render 1 ring's worth of coordinates. */
      if (this->AntiAliasOn()) {
         getViewerPtr()->GetLineRendererRef().RenderLineLoopAntiAliased(
                                 ringCoords3D, TCOglColor(TCOglColor::GRAY));
      } else {
         getViewerPtr()->GetLineRendererRef().RenderLineLoop(
                                 ringCoords3D, TCOglColor(TCOglColor::GRAY));
      }
   }
   /* 2. Draw the spokes/slices. */
   /* FOR EACH slice in the polar grid. */
   for(TCShort s = 0; s <= gridPolar.GetNumSlices(); s++) {

      spokeCoords3D.push_back(
         gridPolar.ComputeGridCoordTransformedToXYplaneUserTransformed(0, 0));
      spokeCoords3D.push_back(
         gridPolar.ComputeGridCoordTransformedToXYplaneUserTransformed(gridPolar.GetNumRings(),s));
   }

   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                                 spokeCoords3D, TCOglColor(TCOglColor::GRAY));
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                                 spokeCoords3D, TCOglColor(TCOglColor::GRAY));
   }
   if (debug) this->render9oclockTransformedToXYplane(gridPolar);

   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::renderInXY(TCGridCartesian& gridCartesian) {

   bool debug = false;   
   TCShort i = 0;
   std::vector< TC::Math::Coord3D< float > > segmentsCoords3D;
   segmentsCoords3D.SetNumElems(0);

   if (debug) tcerr << "TCOglGridRenderer::renderInXY()" << endl;
   if (debug) this->renderDiagonalUserTransformed(gridCartesian); /* for testing */

   /* render each grid line along a row */
   for(i = 0; i <= gridCartesian.GetNumRows(); i++) {
   
      segmentsCoords3D.push_back(gridCartesian.GetXYgridCoord(i, 0));
      segmentsCoords3D.push_back(gridCartesian.GetXYgridCoord(i, gridCartesian.GetNumColumns()));
   }
   /* render each grid line along a column */
   for(i = 0; i <= gridCartesian.GetNumColumns(); i++) {
   
      segmentsCoords3D.push_back(gridCartesian.GetXYgridCoord(0, i));
      segmentsCoords3D.push_back(gridCartesian.GetXYgridCoord(gridCartesian.GetNumRows(), i));
   }
   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                                 segmentsCoords3D, this->getColorRef());
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                                 segmentsCoords3D, this->getColorRef());
   }
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::renderInXZ(TCGridCartesian& gridCartesian) {

   bool debug = false;   
   TCShort i = 0;
   std::vector< TC::Math::Coord3D< float > > segmentsCoords3D;
   segmentsCoords3D.SetNumElems(0);
   
   if (debug) tcerr << "TCOglGridRenderer::renderInXZ()" << endl;
   if (debug) this->renderDiagonalUserTransformed(gridCartesian); /* for testing */

   /* render each grid line along a row */
   for(i = 0; i <= gridCartesian.GetNumRows(); i++) {
   
      segmentsCoords3D.push_back(gridCartesian.GetXZgridCoord(i, 0));
      segmentsCoords3D.push_back(gridCartesian.GetXZgridCoord(i, gridCartesian.GetNumColumns()));
   }
   /* render each grid line along a column */
   for(i = 0; i <= gridCartesian.GetNumColumns(); i++) {
   
      segmentsCoords3D.push_back(gridCartesian.GetXZgridCoord(0, i));
      segmentsCoords3D.push_back(gridCartesian.GetXZgridCoord(gridCartesian.GetNumRows(), i));
   }
   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                           segmentsCoords3D, this->getColorRef());
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                           segmentsCoords3D, this->getColorRef());
   }
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::renderDiagonalUserTransformed(TCGrid& grid) {

   bool debug = false;
   TCOglColor beginColor = TCOglColor(TCOglColor::RED);
   TCOglColor endColor   = TCOglColor(TCOglColor::GREEN);
   std::vector< TC::Math::Coord3D< float > > beginSegmentCoords3D;
   std::vector< TC::Math::Coord3D< float > > endSegmentCoords3D;
   
   if (debug) tcerr << "TCOglGridRenderer::renderDiagonalUserTransformed()" << endl;

   beginSegmentCoords3D.SetNumElems(0);
     endSegmentCoords3D.SetNumElems(0);

   beginSegmentCoords3D.push_back(grid.GetMinCoordUserTransformed());
   beginSegmentCoords3D.push_back(grid.ComputeCenterCoordOriginal());
     endSegmentCoords3D.push_back(grid.ComputeCenterCoordOriginal());
     endSegmentCoords3D.push_back(grid.GetMaxCoordUserTransformed());

   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                              beginSegmentCoords3D, beginColor);
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                              endSegmentCoords3D, endColor);
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                              beginSegmentCoords3D, beginColor);
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                              endSegmentCoords3D, endColor);
   }
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::renderDiagonalOriginal(TCGridCartesian& gridCartesian) {

   bool debug = false;
   TCOglColor beginColor = TCOglColor(TCOglColor::YELLOW);
   TCOglColor endColor   = TCOglColor(TCOglColor::BLUE);
   std::vector< TC::Math::Coord3D< float > > beginSegmentCoords3D;
   std::vector< TC::Math::Coord3D< float > > endSegmentCoords3D;
   
   if (debug) tcerr << "TCOglGridRenderer::renderDiagonalOriginal()" << endl;

   beginSegmentCoords3D.SetNumElems(0);
     endSegmentCoords3D.SetNumElems(0);

   beginSegmentCoords3D.push_back(gridCartesian.GetMinCoordOriginal());
   beginSegmentCoords3D.push_back(gridCartesian.ComputeCenterCoordOriginal());
     endSegmentCoords3D.push_back(gridCartesian.ComputeCenterCoordOriginal());
     endSegmentCoords3D.push_back(gridCartesian.GetMaxCoordOriginal());

   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                              beginSegmentCoords3D, beginColor);
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                              endSegmentCoords3D, endColor);
   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                              beginSegmentCoords3D, beginColor);
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                              endSegmentCoords3D, endColor);
   }
   return true;
}
/*
 * @see TCOglGridRenderer.h for comments
 */
bool TCOglGridRenderer::renderDiagonalInXYplane(TCGridCartesian& gridCartesian) {

   bool debug = false;
   TCOglColor beginColor = TCOglColor(TCOglColor::WHITE);
   TCOglColor endColor   = TCOglColor(TCOglColor::BLACK);
   std::vector< TC::Math::Coord3D< float > > beginSegmentCoords3D;
   std::vector< TC::Math::Coord3D< float > > endSegmentCoords3D;
   
   if (debug) tcerr << "TCOglGridRenderer::renderDiagonalInXYplane()" << endl;

   /* Render arbitrarily transormed diagonal line segments */
   beginColor = TCOglColor(TCOglColor::WHITE);
     endColor = TCOglColor(TCOglColor::BLACK);
   
   beginSegmentCoords3D.SetNumElems(0);
     endSegmentCoords3D.SetNumElems(0);

   beginSegmentCoords3D.push_back(gridCartesian.GetMinCoordInXYplane());
   beginSegmentCoords3D.push_back(gridCartesian.ComputeCenterCoordInXYplane());
     endSegmentCoords3D.push_back(gridCartesian.ComputeCenterCoordInXYplane());
     endSegmentCoords3D.push_back(gridCartesian.GetMaxCoordInXYplane());
   
   if (this->AntiAliasOn()) {
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                           beginSegmentCoords3D, beginColor);
      getViewerPtr()->GetLineRendererRef().RenderLineSegmentsAntiAliased(
                           endSegmentCoords3D, endColor);

   } else {
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                           beginSegmentCoords3D, beginColor);
      getViewerPtr()->GetLineRendererRef().RenderLineSegments(
                           endSegmentCoords3D, endColor);
   }
   return true;
}
