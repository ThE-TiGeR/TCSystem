//*******************************************************************************
//
// *******   ***   ***               *
//    *     *     *                  *
//    *    *      *                *****
//    *    *       ***  *   *   **   *    **    ***
//    *    *          *  * *   *     *   ****  * * *
//    *     *         *   *      *   * * *     * * *
//    *      ***   ***    *     **   **   **   *   *
//                        *
//*******************************************************************************
// see http://sourceforge.net/projects/tcsystem/ for details.
// Copyright (C) 2003 - 2007 Thomas Goessler. All Rights Reserved. 
//*******************************************************************************
//
// TCSystem is the legal property of its developers.
// Please refer to the COPYRIGHT file distributed with this source distribution.
// 
// This library is free software; you can redistribute it and/or             
// modify it under the terms of the GNU Lesser General Public                
// License as published by the Free Software Foundation; either              
// version 2.1 of the License, or (at your option) any later version.        
//                                                                           
// This library is distributed in the hope that it will be useful,           
// but WITHOUT ANY WARRANTY; without even the implied warranty of            
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
// Lesser General Public License for more details.                           
//                                                                           
// You should have received a copy of the GNU Lesser General Public          
// License along with this library; if not, write to the Free Software       
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
//*******************************************************************************
//  $Id$
//*******************************************************************************

#ifndef _TC_OPENGL_POINT_RENDERER_H_
#define _TC_OPENGL_POINT_RENDERER_H_

/**
 * Class for rendering points in different types
 * in the moment there are 6 types supported:
 * spheres, circles, filled circles, filled squares, cubes and 3D crosses.
 * Spheres are rendered using gluSphere(),
 * circles and filled circles with gluDisk(),
 * filled squares are GL_POINTS,
 * cubes and crosses with plain glVertex() calls,
 */
class TCOGL_API TCOglPointRenderer: public Renderer
{
TCBASE_DECLARE( TCOglPointRenderer)
public:
   /**
    * Enumeration for all supported point types.
    */
   enum POINT_TYPE
   {
      /** Draw points as spheres. */
      POINT_SPHERE,
      /** Draw points as circles. */
      POINT_CIRCLE,
      /** Draw points as filled circles. */
      POINT_FILLED_CIRCLE,
      /** Draw points as filled squares. */
      POINT_FILLED_SQUARE,
      /** Draw points as cubes. */
      POINT_CUBE,
      /** Draw points as 3D crosses. */
      POINT_CROSS
   };

   /** Minimum and maximum number of circle / sphere slices for gluDisk() and gluSphere. @see getNumberOfSlices() */
   enum SLICE_NUMBER_BOUNDS
   {
      /** Minimum number of circle slices. */
      MIN_DISK_SLICES = 6,
      /** Maximum number of circle slices. */
      MAX_DISK_SLICES = 40,
      /** Minimum number of sphere slices. */
      MIN_SPHERE_SLICES = 4,
      /** Maximum number of sphere slices. */
      MAX_SPHERE_SLICES = 20
   };

public:
   /**
    * Construct an TCOglPointRenderer object.
    * @param viewer The viewer for which the renderer should be created.
    */
   TCOglPointRenderer(TCOglViewer *viewer);
   /** Destroy the renderer. */
   virtual ~TCOglPointRenderer( );

   /**
    * Render one point.  Does anyone know what units "size" is measured
    * in?  Also, is it a radius, diameter, or circumference?  Looks like
    * it is measure in units of meters.
    *
    * @param point The position where to draw the point.
    * @param size Size of the point.
    * @param color Color of the point.
    * @param type Type of point, see TCOglPointRenderer::POINT_TYPE.
    */
   void RenderPoint(const TC::Math::Coord3D< float >& point, 
                    float size, 
                    const Color& color, POINT_TYPE type);
   /**
    * Render an array of points with different size and in the same color.
    * @param points The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of all points.
    * @param type Type of all points, see TCOglPointRenderer::POINT_TYPE.
    */
   void RenderPoints( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size, const Color &color, POINT_TYPE type);
   /**
    * Render an array of points with same size and different colors.
    * @param points The position where to draw each point.
    * @param size Size of points.
    * @param color Color of each point.
    * @param type Type of all points, see TCOglPointRenderer::POINT_TYPE.
    */
   void RenderPoints( const std::vector< TC::Math::Coord3D< float > > &points, float size, const std::vector<Color> &color, POINT_TYPE type);
   /**
    * Render an array of points with different size and different colors.
    * @param points The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of each point.
    * @param type Type of all points, see TCOglPointRenderer::POINT_TYPE.
    */
   void RenderPoints( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size, const std::vector<Color> &color, POINT_TYPE type);
   
   /**
    * Render one point as a sphere.
    * @param point The position where to draw the point.
    * @param size Size of the point.
    * @param color Color of the point.
    */
   void RenderSphere( const TC::Math::Coord3D< float > &point, float size, const Color &color );
   /**
    * Render an array of points as spheres with different size and in the same color.
    * @parama point The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of ecah point.
    */
   void RenderSpheres( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size, const Color &color );
   /**
    * Render an array of points as spheres with same size and different colors.
    * @parama point The position where to draw each point.
    * @param size Size of points.
    * @param color Color of each point.
    */
   void RenderSpheres( const std::vector< TC::Math::Coord3D< float > > &points, float size, const std::vector<Color> &color );
   /**
    * Render an array of points as spheres with different size and different colors.
    * @parama point The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of each point.
    */
   void RenderSpheres( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size, const std::vector<Color> &color );


   /**
    * Render one point as a circle.
    * @param point The position where to draw the point.
    * @param size Size of the point.
    * @param color Color of the point.
    */
   void RenderCircle( const TC::Math::Coord3D< float > &points, float size, const Color &color );

   /**
    * Render an array of points as cricles with different size and in the same color.
    * @parama point The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of ecah point.
    */
   void RenderCircles( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size, const Color &color );
   /**
    * Render an array of points as circles with same size and different colors.
    * @paramapoint The position where to draw each point.
    * @param size Size of points.
    * @param color Color of ecah point.
    */
   void RenderCircles( const std::vector< TC::Math::Coord3D< float > > &points, float size, const std::vector<Color> &color );
   /**
    * Render an array of points as circles with different size and different colors.
    * @paramapoint The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of ecah point.
    */
   void RenderCircles( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size, const std::vector<Color> &color );



   /**
    * Render one point as a filled circle.
    * @param point The position where to draw the point.
    * @param size Size of the point.
    * @param color Color of the point.
    */
   void RenderFilledCircle( const TC::Math::Coord3D< float > &point, float size, const Color &color );
   /**
    * Render an array of points as filled circles with different size and in the same color.
    * @parama point The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of ecah point.
    */
   void RenderFilledCircles( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size, const Color &color );
   /**
    * Render an array of points as filled circles with same size and different colors.
    * @parama point The position where to draw each point.
    * @param size Size of points.
    * @param color Color of each point.
    */
   void RenderFilledCircles( const std::vector< TC::Math::Coord3D< float > > &points, float size, const std::vector<Color> &color );
   /**
    * Render an array of points as filled circles with different size and different colors.
    * @parama point The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of each point.
    */
   void RenderFilledCircles( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size, const std::vector<Color> &color );


   
   /**
    * Render one point as filled squares .
    * @parama point The position where to draw the point.
    * @param size Size of the point.
    * @param color Color of the point.
    */
   void RenderFilledSquare( const TC::Math::Coord3D< float > &point, float size, const Color &color );
   /**
    * Render an array of points as filled squares with different size and in the same color.
    * @parama point The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of ecah point.
    */
   void RenderFilledSquares( const std::vector< TC::Math::Coord3D< float > > &points, float size, const Color &color );
   /**
    * Render an array of points as filled squares with different size and in the same color.
    * @parama point The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of ecah point.
    */
   void RenderFilledSquares( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size, const Color &color );
   /**
    * Render an array of points as filled squares with same size and different colors.
    * @parama point The position where to draw each point.
    * @param size Size of points.
    * @param color Color of ecah point.
    */
   void RenderFilledSquares( const std::vector< TC::Math::Coord3D< float > > &points, float size, const std::vector<Color> &color );
   /**
    * Render an array of points as filled squares with different size and different colors.
    * @parama point The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of ecah point.
    */
   void RenderFilledSquares( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size, const std::vector<Color> &color );
   


   /**
    * Render one point as a cube.
    * @param point The position where to draw the point.
    * @param size Size of the point.
    * @param color Color of the point.
    */
   void RenderCube( const TC::Math::Coord3D< float > &point, float size, const Color &color );
   /**
    * Render an array of points as cubes with different size and in the same color.
    * @parama point The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of ecah point.
    */
   void RenderCubes( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size, const Color &color );
   /**
    * Render an array of points as cubes with smae size and different colors.
    * @parama point The position where to draw each point.
    * @param size Size of points.
    * @param color Color of each point.
    */
   void RenderCubes( const std::vector< TC::Math::Coord3D< float > > &points, float size, const std::vector<Color> &color );
   /**
    * Render an array of points as cubes with different size and different colors.
    * @parama point The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of each point.
    */
   void RenderCubes( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size, const std::vector<Color> &color );
   


   /**
    * Render one cube with different colors on each face.
    * @param point The position of the center of the cube.
    * @param size Size of the cube (length of edge).
    * @param color0 Color for face 0.
    * @param color1 Color for face 1.
    * @param color2 Color for face 2.
    * @param color3 Color for face 3.
    * @param color4 Color for face 4.
    * @param color5 Color for face 5.
    */
   void RenderColoredCube( const TC::Math::Coord3D< float > &point, float size,
      const Color &color0, const Color &color1, const Color &color2, const Color &color3, const Color &color4, const Color &color5);
   /**
    * Render cubes with different colors on each face in uniform size.
    * @param point The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of all points.
    * @param type Type of all points, see TCOglPointRenderer::POINT_TYPE.
    */
   void RenderColoredCubes( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size,
      const Color &color0, const Color &color1, const Color &color2, const Color &color3, const Color &color4, const Color &color5);
   

   /**
    * Render one point as a 3D cross.
    * @param point The position where to draw the point.
    * @param size Size of the point.
    * @param color Color of the point.
    */
   void RenderCross( const TC::Math::Coord3D< float > &point, float size, const Color &color );
   /**
    * Render an array of points as 3D crosses with different size and in the same color.
    * @parama point The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of ecah point.
    */
   void RenderCrosses( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size, const Color &color );
   /**
    * Render an array of points as 3D crosses with same size and different colors.
    * @parama point The position where to draw each point.
    * @param size Size of points.
    * @param color Color of each point.
    */
   void RenderCrosses( const std::vector< TC::Math::Coord3D< float > > &points, float size, const std::vector<Color> &color );
   /**
    * Render an array of points as 3D crosses with different size and different colors.
    * @parama point The position where to draw each point.
    * @param size Size of each point.
    * @param color Color of each point.
    */
   void RenderCrosses( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size, const std::vector<Color> &color );


protected:
   /**
    * Calculate a reasonable number of slices for circles
    * depending on the radius and the pixel size.
    * @param  radius    Outer radius of disk for gluDisk().
    * @param  worldPix  World pixel size queried from FXGLViewer::worldPix().
    * @return Recommended number of slices for a circular looking disk. It is clampled between MIN_DISK_SLICES and MAX_DISK_SLICES.
    */
   static uint32 getNumberOfDiskSlices(float radius, float worldPix);

   /**
    * Calculate a reasonable number of slices for spheres
    * depending on the radius and the pixel size.
    * @param  radius    Radius of sphere for gluSphere().
    * @param  worldPix  World pixel size queried from FXGLViewer::worldPix().
    * @return Recommended number of slices for a smooth looking sphere. It is clampled between MIN_SPHERE_SLICES and MAX_SPHERE_SLICES.
    */
   static uint32 getNumberOfSphereSlices(float radius, float worldPix);

   /**
    * Draw a cube whose center is a the origin.
    * The edges are paralel to the axes.
    * Color is not changed.
    * @param  size  Length of edge.
    */
   static void renderCube(float size);

   /**
    * Draw a cube whose center is a the origin.
    * The edges are paralel to the axes.
    * Each face has different colors.
    * @param  size    Length of edge.
    * @param  color0  Color for face 0 (left,   normal [-1,0,0] )
    * @param  color1  Color for face 1 (right,  normal [ 1,0,0] )
    * @param  color2  Color for face 2 (bottom, normal [0,-1,0] )
    * @param  color3  Color for face 3 (top,    normal [0, 1,0] )
    * @param  color4  Color for face 4 (back,   normal [0,0,-1] )
    * @param  color5  Color for face 5 (front,  normal [0,0, 1] )
    */
   void renderColoredCube(float size,
      const Color &color0, const Color &color1, const Color &color2, const Color &color3, const Color &color4, const Color &color5);

private:
   /** Pointer to an GLU Quadric object needed for drawing gluSphere(), gluDisk(), etc. */
   GLUquadricObj* m_QuadricObject;
};

#endif
