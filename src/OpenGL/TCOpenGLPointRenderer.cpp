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
//  $Id: TCOpenGLPointRenderer.cpp 766 2007-10-24 22:42:17Z the_____tiger $
//*******************************************************************************

#include "TCOpenGL.h"

#include "TCNewEnable.h"

inline uint32 TCOglPointRenderer::getNumberOfDiskSlices(float radius, float worldPix)
{
   uint32 segments = (uint32)(radius/worldPix);
   segments = Max( segments, MIN_DISK_SLICES);
   segments = Min( segments, MAX_DISK_SLICES);

   return segments;
}

inline uint32 TCOglPointRenderer::getNumberOfSphereSlices(float radius, float worldPix)
{
   uint32 segments = (uint32)(radius/worldPix);
   if (segments < MIN_SPHERE_SLICES) segments = MIN_SPHERE_SLICES;
   if (segments > MAX_SPHERE_SLICES) segments = MAX_SPHERE_SLICES;
   return segments;
}

TCOglPointRenderer::TCOglPointRenderer( TCOglViewer *viewer)
:Renderer( viewer)
{
   m_QuadricObject = gluNewQuadric( );
}

TCOglPointRenderer::~TCOglPointRenderer( )
{
   if( m_QuadricObject ) gluDeleteQuadric( m_QuadricObject);
}

void TCOglPointRenderer::RenderPoint( const TC::Math::Coord3D< float > &point, float size, const Color &color, POINT_TYPE type)
{
   switch ( type )
   {
   case POINT_SPHERE:
      RenderSphere( point, size, color);
      break;

   case POINT_CIRCLE:
      RenderCircle( point, size, color);
      break;

   case POINT_FILLED_CIRCLE:
      RenderFilledCircle( point, size, color);
      break;

   case POINT_FILLED_SQUARE:
      RenderFilledSquare( point, size, color);
      break;

   case POINT_CUBE:
      RenderCube( point, size, color);
      break;

   case POINT_CROSS:
      RenderCross( point, size, color);
      break;

   default:
      tcerr << "***Warning, TCOglPointRenderer::RenderPoint()";
      tcerr << " unrecognized point type" << endl;
      break;
   }
}

void TCOglPointRenderer::RenderPoints( const std::vector< TC::Math::Coord3D< float > > &points,
                                      const std::vector< float > &size,
                                      const Color &color, POINT_TYPE type)
{
   switch ( type )
   {
   case POINT_SPHERE:
      RenderSpheres( points, size, color);
      break;

   case POINT_CIRCLE:
      RenderCircles( points, size, color);
      break;

   case POINT_FILLED_CIRCLE:
      RenderFilledCircles( points, size, color);
      break;

   case POINT_FILLED_SQUARE:
      RenderFilledSquares( points, size, color);
      break;

   case POINT_CUBE:
      RenderCubes( points, size, color);
      break;

   case POINT_CROSS:
      RenderCrosses( points, size, color);
      break;

   default:
      tcerr << "***Warning, TCOglPointRenderer::RenderPoints()";
      tcerr << " unrecognized point type" << endl;
      break;
   }
}

void TCOglPointRenderer::RenderPoints( const std::vector< TC::Math::Coord3D< float > > &points,
                                      float size,
                                      const std::vector< Color > &color, POINT_TYPE type)
{
   switch ( type )
   {
   case POINT_SPHERE:
      RenderSpheres( points, size, color);
      break;

   case POINT_CIRCLE:
      RenderCircles( points, size, color);
      break;

   case POINT_FILLED_CIRCLE:
      RenderFilledCircles( points, size, color);
      break;

   case POINT_FILLED_SQUARE:
      RenderFilledSquares( points, size, color);
      break;

   case POINT_CUBE:
      RenderCubes( points, size, color);
      break;

   case POINT_CROSS:
      RenderCrosses( points, size, color);
      break;

   default:
      tcerr << "***Warning, TCOglPointRenderer::RenderPoints()";
      tcerr << " unrecognized point type" << endl;
      break;

   }
}

void TCOglPointRenderer::RenderPoints( const std::vector< TC::Math::Coord3D< float > > &points,
                                      const std::vector< float > &size,
                                      const std::vector< Color > &color, POINT_TYPE type)
{
   switch ( type )
   {
   case POINT_SPHERE:
      RenderSpheres( points, size, color);
      break;

   case POINT_CIRCLE:
      RenderCircles( points, size, color);
      break;

   case POINT_FILLED_CIRCLE:
      RenderFilledCircles( points, size, color);
      break;

   case POINT_FILLED_SQUARE:
      RenderFilledSquares( points, size, color);
      break;

   case POINT_CUBE:
      RenderCubes( points, size, color);
      break;

   case POINT_CROSS:
      RenderCrosses( points, size, color);
      break;

   default:
      tcerr << "***Warning, TCOglPointRenderer::RenderPoints()";
      tcerr << " unrecognized point type" << endl;
      break;
   }
}

void TCOglPointRenderer::RenderColoredCube( const TC::Math::Coord3D< float > &point, float size,
                                            const Color &color0, const Color &color1, const Color &color2, const Color &color3, const Color &color4, const Color &color5)
{
   glTranslatef( point[0], point[1], point[2]);
   renderColoredCube(size, color0, color1, color2, color3, color4, color5);
   glTranslatef( -point[0], -point[1], -point[2]);
}

void TCOglPointRenderer::RenderColoredCubes( const std::vector< TC::Math::Coord3D< float > > &points, const std::vector< float > &size,
                                            const Color &color0, const Color &color1, const Color &color2, const Color &color3, const Color &color4, const Color &color5)
{
   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) ) {
      glTranslatef( points[i][0], points[i][1], points[i][2]);
      renderColoredCube(size[i], color0, color1, color2, color3, color4, color5);
      glTranslatef( -points[i][0], -points[i][1], -points[i][2]);
   }
}

void TCOglPointRenderer::RenderSphere( const TC::Math::Coord3D< float > &point, float size, const Color &color )
{
   // Set quadric draw style. Circles use GLU_SILHOUETTE, all others use GLU_FILL.
   gluQuadricDrawStyle(m_QuadricObject, GLU_FILL);
   // Render shading because of color
   this->EnableColoredShading( color);

   float worldPix = 2*(float)getViewerPtr( )->worldPix();
   uint32 segments = getNumberOfSphereSlices(size, worldPix);

   glTranslatef( point[0], point[1], point[2]);
   gluSphere( m_QuadricObject, size, segments, segments );
   glTranslatef( -point[0], -point[1], -point[2]);

   this->DisableColoredShading( );
}

void TCOglPointRenderer::RenderSpheres( const std::vector< TC::Math::Coord3D< float > > &points,
                                       const std::vector< float > &size,
                                       const Color &color)
{
   // Set quadric draw style. Circles use GLU_SILHOUETTE, all others use GLU_FILL.
   gluQuadricDrawStyle(m_QuadricObject, GLU_FILL);
   // Render shading because of color
   this->EnableColoredShading( color);

   float worldPix = 2*(float)getViewerPtr( )->worldPix();

   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      glTranslatef( points[i][0], points[i][1], points[i][2]);

      uint32 segments = getNumberOfSphereSlices(size[i], worldPix);
      gluSphere( m_QuadricObject, size[i], segments, segments );

      glTranslatef( -points[i][0], -points[i][1], -points[i][2]);
   }

   this->DisableColoredShading( );
}

void TCOglPointRenderer::RenderSpheres( const std::vector< TC::Math::Coord3D< float > > &points,
                                       float size,
                                       const std::vector< Color > &color)
{
   // Set quadric draw style. Circles use GLU_SILHOUETTE, all others use GLU_FILL.
   gluQuadricDrawStyle(m_QuadricObject, GLU_FILL);
   // Render shading because of color
   this->EnableColoredShading( );

   float worldPix = 2*(float)getViewerPtr( )->worldPix();
   uint32 segments = getNumberOfSphereSlices(size, worldPix);

   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ))
   {
      glTranslatef( points[i][0], points[i][1], points[i][2]);

      this->SetColor( color[i] );
      gluSphere( m_QuadricObject, size, segments, segments );

      glTranslatef( -points[i][0], -points[i][1], -points[i][2]);
   }

   this->DisableColoredShading( );
}

void TCOglPointRenderer::RenderSpheres( const std::vector< TC::Math::Coord3D< float > > &points,
                                       const std::vector< float > &size,
                                       const std::vector< Color > &color)
{
   // Set quadric draw style. Circles use GLU_SILHOUETTE, all others use GLU_FILL.
   gluQuadricDrawStyle(m_QuadricObject, GLU_FILL);
   // Render shading because of color
   this->EnableColoredShading( );

   float worldPix = 2*(float)getViewerPtr( )->worldPix();

   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      glTranslatef( points[i][0], points[i][1], points[i][2]);

      this->SetColor( color[i] );
      uint32 segments = getNumberOfSphereSlices(size[i], worldPix);
      gluSphere( m_QuadricObject, size[i], segments, segments );

      glTranslatef( -points[i][0], -points[i][1], -points[i][2]);
   }

   this->DisableColoredShading( );
}

void TCOglPointRenderer::RenderCircle( const TC::Math::Coord3D< float > &point, float size, const Color &color )
{
   TCDouble mat[16];
   getViewerPtr( )->calcInverseRotationMatrix(mat);

   gluQuadricDrawStyle(m_QuadricObject, GLU_SILHOUETTE);
   this->SetColor( color );

   glPushMatrix();
   glTranslatef( point[0], point[1], point[2]);
   glMultMatrixd(mat);

   float worldPix = (float)getViewerPtr( )->worldPix();
   uint32 segments = getNumberOfDiskSlices(size, worldPix);
   gluDisk(m_QuadricObject, 0.0, size, segments, 1);

   glPopMatrix();
}

void TCOglPointRenderer::RenderCircles( const std::vector< TC::Math::Coord3D< float > > &points,
                                       const std::vector< float > &size,
                                       const Color &color)
{
   TCDouble mat[16];
   getViewerPtr( )->calcInverseRotationMatrix(mat);

   gluQuadricDrawStyle(m_QuadricObject, GLU_SILHOUETTE);
   this->SetColor( color );
   float worldPix = (float)getViewerPtr( )->worldPix();

   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      glPushMatrix();
      glTranslatef( points[i][0], points[i][1], points[i][2]);
      glMultMatrixd(mat);

      uint32 segments = getNumberOfDiskSlices(size[i], worldPix);
      gluDisk(m_QuadricObject, 0.0, size[i], segments, 1);

      glPopMatrix();
   }
}

void TCOglPointRenderer::RenderCircles( const std::vector< TC::Math::Coord3D< float > > &points,
                                       float size,
                                       const std::vector< Color > &color)
{
   TCDouble mat[16];
   getViewerPtr( )->calcInverseRotationMatrix(mat);

   gluQuadricDrawStyle(m_QuadricObject, GLU_SILHOUETTE);
   float worldPix = (float)getViewerPtr( )->worldPix();
   uint32 segments = getNumberOfDiskSlices(size, worldPix);

   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      glPushMatrix();
      glTranslatef( points[i][0], points[i][1], points[i][2]);
      glMultMatrixd(mat);

      this->SetColor( color[i] );
      gluDisk(m_QuadricObject, 0.0, size, segments, 1);

      glPopMatrix();
   }
}

void TCOglPointRenderer::RenderCircles( const std::vector< TC::Math::Coord3D< float > > &points,
                                       const std::vector< float > &size,
                                       const std::vector< Color > &color)
{
   TCDouble mat[16];
   getViewerPtr( )->calcInverseRotationMatrix(mat);

   gluQuadricDrawStyle(m_QuadricObject, GLU_SILHOUETTE);
   float worldPix = (float)getViewerPtr( )->worldPix();

   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      glPushMatrix();
      glTranslatef( points[i][0], points[i][1], points[i][2]);
      glMultMatrixd(mat);

      this->SetColor( color[i] );
      uint32 segments = getNumberOfDiskSlices(size[i], worldPix);
      gluDisk(m_QuadricObject, 0.0, size[i], segments, 1);

      glPopMatrix();
   }
}

void TCOglPointRenderer::RenderFilledCircle( const TC::Math::Coord3D< float > &point, float size, const Color &color )
{
   TCDouble mat[16];
   getViewerPtr( )->calcInverseRotationMatrix(mat);

   gluQuadricDrawStyle(m_QuadricObject, GLU_FILL);
   this->SetColor( color );

   glPushMatrix();
   glTranslatef( point[0], point[1], point[2]);
   glMultMatrixd(mat);

   float worldPix = (float)getViewerPtr( )->worldPix();
   uint32 segments = getNumberOfDiskSlices(size, worldPix);
   gluDisk(m_QuadricObject, 0., size, segments, 1);

   glPopMatrix();
}

void TCOglPointRenderer::RenderFilledCircles( const std::vector< TC::Math::Coord3D< float > > &points,
                                             const std::vector< float > &size,
                                             const Color &color)
{
   TCDouble mat[16];
   getViewerPtr( )->calcInverseRotationMatrix(mat);

   gluQuadricDrawStyle(m_QuadricObject, GLU_FILL);
   this->SetColor( color );
   float worldPix = (float)getViewerPtr( )->worldPix();

   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      glPushMatrix();
      glTranslatef( points[i][0], points[i][1], points[i][2]);
      glMultMatrixd(mat);

      uint32 segments = getNumberOfDiskSlices(size[i], worldPix);
      gluDisk(m_QuadricObject, 0., size[i], segments, 1);

      glPopMatrix();
   }
}

void TCOglPointRenderer::RenderFilledCircles( const std::vector< TC::Math::Coord3D< float > > &points,
                                             float size,
                                             const std::vector< Color > &color)
{
   TCDouble mat[16];
   getViewerPtr( )->calcInverseRotationMatrix(mat);

   gluQuadricDrawStyle(m_QuadricObject, GLU_FILL);
   float worldPix = (float)getViewerPtr( )->worldPix();
   uint32 segments = getNumberOfDiskSlices(size, worldPix);

   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      glPushMatrix();
      glTranslatef( points[i][0], points[i][1], points[i][2]);
      glMultMatrixd(mat);

      this->SetColor( color[i] );
      gluDisk(m_QuadricObject, 0., size, segments, 1);

      glPopMatrix();
   }
}

void TCOglPointRenderer::RenderFilledCircles( const std::vector< TC::Math::Coord3D< float > > &points,
                                             const std::vector< float > &size,
                                             const std::vector< Color > &color)
{
   TCDouble mat[16];
   getViewerPtr( )->calcInverseRotationMatrix(mat);

   gluQuadricDrawStyle(m_QuadricObject, GLU_FILL);
   float worldPix = (float)getViewerPtr( )->worldPix();

   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      glPushMatrix();
      glTranslatef( points[i][0], points[i][1], points[i][2]);
      glMultMatrixd(mat);

      this->SetColor( color[i] );
      uint32 segments = getNumberOfDiskSlices(size[i], worldPix);
      gluDisk(m_QuadricObject, 0., size[i], segments, 1);

      glPopMatrix();
   }
}

void TCOglPointRenderer::RenderFilledSquare( const TC::Math::Coord3D< float > &point, float size, const Color &color )
{
   this->SetColor( color );

   glPointSize( (float)Max( (uint32)(size / getViewerPtr( )->worldPix( )), 1) );
   this->Begin(GL_POINTS);
   this->SetVertex( point );
   this->End( );
}

void TCOglPointRenderer::RenderFilledSquares( const std::vector< TC::Math::Coord3D< float > > &points,
                                             const std::vector< float > &size,
                                             const Color &color)
{
   this->SetColor( color );

   sint32 sizeOld = -1, sizeNew=0;
   this->Begin(GL_POINTS);
   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      sizeNew = Max( (uint32)(size[i] / getViewerPtr( )->worldPix( )), 1);
      if ( sizeOld != sizeNew )
      {
         sizeOld = sizeNew;

         this->End( );
         glPointSize( (float)sizeNew );
         this->Begin( GL_POINTS );
      }

      this->SetVertex( points[i] );
   }
   this->End( );
}

void TCOglPointRenderer::RenderFilledSquares( const std::vector< TC::Math::Coord3D< float > > &points,
                                             float size,
                                             const Color &color )
{
   glPointSize( (float)Max( (uint32)(size / getViewerPtr( )->worldPix( )), 1) );
   this->SetColor( color );
   this->Begin(GL_POINTS);
   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
      this->SetVertex( points[i] );
   this->End( );
}

void TCOglPointRenderer::RenderFilledSquares( const std::vector< TC::Math::Coord3D< float > > &points,
                                             float size,
                                             const std::vector< Color > &color )
{
   glPointSize( (float)Max( (uint32)(size / getViewerPtr( )->worldPix( )), 1) );
   this->Begin(GL_POINTS);
   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      this->SetColor( color[i] );
      this->SetVertex( points[i] );
   }
   this->End( );
}

void TCOglPointRenderer::RenderFilledSquares( const std::vector< TC::Math::Coord3D< float > > &points,
                                             const std::vector< float > &size,
                                             const std::vector< Color > &color )
{
   sint32 sizeOld = -1, sizeNew=0;
   this->Begin(GL_POINTS);
   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      sizeNew = Max( (uint32)(size[i] / getViewerPtr( )->worldPix( )), 1);
      if ( sizeOld != sizeNew )
      {
         sizeOld = sizeNew;

         this->End( );
         glPointSize( (float)sizeNew );
         this->Begin( GL_POINTS );
      }

      this->SetColor( color[i] );
      this->SetVertex( points[i] );
   }
   this->End( );
}

void TCOglPointRenderer::RenderCube( const TC::Math::Coord3D< float > &point, float size, const Color &color )
{
   // Render shading because of color
   this->EnableColoredShading( color);

   glTranslatef( point[0], point[1], point[2]);
   renderCube(size);
   glTranslatef( -point[0], -point[1], -point[2]);

   this->DisableColoredShading( );
}

void TCOglPointRenderer::RenderCubes( const std::vector< TC::Math::Coord3D< float > > &points,
                                     const std::vector< float > &size,
                                     const Color &color)
{
   // Render shading because of color
   this->EnableColoredShading( color);

   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      glTranslatef( points[i][0], points[i][1], points[i][2]);
      renderCube(size[i]);
      glTranslatef( -points[i][0], -points[i][1], -points[i][2]);
   }

   this->DisableColoredShading( );
}

void TCOglPointRenderer::RenderCubes( const std::vector< TC::Math::Coord3D< float > > &points,
                                     float size,
                                     const std::vector< Color > &color)
{
   // Render shading because of color
   this->EnableColoredShading( );

   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      glTranslatef( points[i][0], points[i][1], points[i][2]);
      this->SetColor( color[i] );
      renderCube(size);
      glTranslatef( -points[i][0], -points[i][1], -points[i][2]);
   }

   this->DisableColoredShading( );
}

void TCOglPointRenderer::RenderCubes( const std::vector< TC::Math::Coord3D< float > > &points,
                                     const std::vector< float > &size,
                                     const std::vector< Color > &color)
{
   // Render shading because of color
   this->EnableColoredShading( );

   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      glTranslatef( points[i][0], points[i][1], points[i][2]);
      this->SetColor( color[i] );
      renderCube(size[i]);
      glTranslatef( -points[i][0], -points[i][1], -points[i][2]);
   }

   this->DisableColoredShading( );
}


void TCOglPointRenderer::RenderCross( const TC::Math::Coord3D< float > &point, float size, const Color &color )
{
   this->SetColor( color );

   this->Begin(GL_LINES);
   glVertex3f(point[0]-size, point[1], point[2]);
   glVertex3f(point[0]+size, point[1], point[2]);
   glVertex3f(point[0], point[1]-size, point[2]);
   glVertex3f(point[0], point[1]+size, point[2]);
   glVertex3f(point[0], point[1], point[2]-size);
   glVertex3f(point[0], point[1], point[2]+size);
   this->End( );
}

void TCOglPointRenderer::RenderCrosses( const std::vector< TC::Math::Coord3D< float > > &points,
                                       const std::vector< float > &size,
                                       const Color &color)
{
   this->SetColor( color );

   this->Begin(GL_LINES);
   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      glVertex3f(points[i][0]-size[i], points[i][1], points[i][2]);
      glVertex3f(points[i][0]+size[i], points[i][1], points[i][2]);
      glVertex3f(points[i][0], points[i][1]-size[i], points[i][2]);
      glVertex3f(points[i][0], points[i][1]+size[i], points[i][2]);
      glVertex3f(points[i][0], points[i][1], points[i][2]-size[i]);
      glVertex3f(points[i][0], points[i][1], points[i][2]+size[i]);
   }
   this->End( );
}

void TCOglPointRenderer::RenderCrosses( const std::vector< TC::Math::Coord3D< float > > &points,
                                       float size,
                                       const std::vector< Color > &color )
{
   this->Begin(GL_LINES);
   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      this->SetColor( color[i] );

      glVertex3f(points[i][0]-size, points[i][1], points[i][2]);
      glVertex3f(points[i][0]+size, points[i][1], points[i][2]);
      glVertex3f(points[i][0], points[i][1]-size, points[i][2]);
      glVertex3f(points[i][0], points[i][1]+size, points[i][2]);
      glVertex3f(points[i][0], points[i][1], points[i][2]-size);
      glVertex3f(points[i][0], points[i][1], points[i][2]+size);
   }
   this->End( );
}

void TCOglPointRenderer::RenderCrosses( const std::vector< TC::Math::Coord3D< float > > &points,
                                       const std::vector< float > &size,
                                       const std::vector< Color > &color )
{
   this->Begin(GL_LINES);
   for ( uint32 i=0; i<points.size( ); i+=this->GetTurboStep( ) )
   {
      this->SetColor( color[i] );

      glVertex3f(points[i][0]-size[i], points[i][1], points[i][2]);
      glVertex3f(points[i][0]+size[i], points[i][1], points[i][2]);
      glVertex3f(points[i][0], points[i][1]-size[i], points[i][2]);
      glVertex3f(points[i][0], points[i][1]+size[i], points[i][2]);
      glVertex3f(points[i][0], points[i][1], points[i][2]-size[i]);
      glVertex3f(points[i][0], points[i][1], points[i][2]+size[i]);
   }
   this->End( );
}

void TCOglPointRenderer::renderCube(float size)
{
   // This ASCII-art tells the vertex ordering.
   // Viewing in Sniff is difficult because of italics, sorry.
   //
   //     2_________3
   //     /|       /|         y
   //    /_|______/ |        |
   //   |6 |     |7 |        |
   //   |  |     |  |        |____ x
   //   |  |     |  |       /
   //   | 0|_____|__|1     /z
   //   | /      | /
   //   |/_______|/
   //   4        5         
   //
   // The code is reasonably optimized.

   Begin(GL_QUADS);

   float pPos = 0.5f*size;
   float pNeg = -pPos;
   // Face 0. (left)
   glNormal3f(-1.0f, 0.0f, 0.0f);
   glVertex3f(pNeg, pNeg, pNeg);
   glVertex3f(pPos, pNeg, pNeg);
   glVertex3f(pPos, pPos, pNeg);
   glVertex3f(pNeg, pPos, pNeg);
   // Face 1. (right)
   glNormal3f(1.0f, 0.0f, 0.0f);
   glVertex3f(pNeg, pNeg, pPos);
   glVertex3f(pNeg, pPos, pPos);
   glVertex3f(pPos, pPos, pPos);
   glVertex3f(pPos, pNeg, pPos);
   // Face 2. (bottom)
   glNormal3f(0.0f, -1.0f, 0.0f);
   glVertex3f(pNeg, pNeg, pNeg);
   glVertex3f(pNeg, pNeg, pPos);
   glVertex3f(pPos, pNeg, pPos);
   glVertex3f(pPos, pNeg, pNeg);
   // Face 3. (top)
   glNormal3f(0.0f, 1.0f, 0.0f);
   glVertex3f(pNeg, pPos, pNeg);
   glVertex3f(pPos, pPos, pNeg);
   glVertex3f(pPos, pPos, pPos);
   glVertex3f(pNeg, pPos, pPos);
   // Face 4. (back)
   glNormal3f(0.0f, 0.0f, -1.0f);
   glVertex3f(pNeg, pNeg, pNeg);
   glVertex3f(pNeg, pPos, pNeg);
   glVertex3f(pNeg, pPos, pPos);
   glVertex3f(pNeg, pNeg, pPos);
   // Face 5. (front)
   glNormal3f(0.0f, 0.0f, 1.0f);
   glVertex3f(pPos, pNeg, pNeg);
   glVertex3f(pPos, pNeg, pPos);
   glVertex3f(pPos, pPos, pPos);
   glVertex3f(pPos, pPos, pNeg);

   End();
}


void TCOglPointRenderer::renderColoredCube(float size,
                                           const Color &color0, const Color &color1, const Color &color2, const Color &color3, const Color &color4, const Color &color5)
{
   // This ASCII-art tells the vertex ordering.
   // Viewing in Sniff is difficult because of italics, sorry.
   //
   //     2_________3
   //     /|       /|         y
   //    /_|______/ |        |
   //   |6 |     |7 |        |
   //   |  |     |  |        |____ x
   //   |  |     |  |       /
   //   | 0|_____|__|1     /z
   //   | /      | /
   //   |/_______|/
   //   4        5         
   //
   // The code is reasonably optimized.

   Begin(GL_QUADS);

   float pPos = 0.5f*size;
   float pNeg = -pPos;
   // Face 0. (left)
   SetColor( color0 );
   glNormal3f(-1.0f, 0.0f, 0.0f);
   glVertex3f(pPos, pNeg, pNeg);
   glVertex3f(pPos, pNeg, pPos);
   glVertex3f(pPos, pPos, pPos);
   glVertex3f(pPos, pPos, pNeg);
   // Face 1. (right)
   SetColor( color1 );
   glNormal3f(1.0f, 0.0f, 0.0f);
   glVertex3f(pNeg, pNeg, pNeg);
   glVertex3f(pNeg, pPos, pNeg);
   glVertex3f(pNeg, pPos, pPos);
   glVertex3f(pNeg, pNeg, pPos);
   // Face 2. (bottom)
   SetColor( color2 );
   glNormal3f(0.0f, -1.0f, 0.0f);
   glVertex3f(pNeg, pNeg, pNeg);
   glVertex3f(pNeg, pNeg, pPos);
   glVertex3f(pPos, pNeg, pPos);
   glVertex3f(pPos, pNeg, pNeg);
   // Face 3. (top)
   SetColor( color3 );
   glNormal3f(0.0f, 1.0f, 0.0f);
   glVertex3f(pNeg, pPos, pNeg);
   glVertex3f(pPos, pPos, pNeg);
   glVertex3f(pPos, pPos, pPos);
   glVertex3f(pNeg, pPos, pPos);
   // Face 4. (back)
   SetColor( color4 );
   glNormal3f(0.0f, 0.0f, -1.0f);
   glVertex3f(pNeg, pNeg, pNeg);
   glVertex3f(pPos, pNeg, pNeg);
   glVertex3f(pPos, pPos, pNeg);
   glVertex3f(pNeg, pPos, pNeg);
   // Face 5. (front)
   SetColor( color5 );
   glNormal3f(0.0f, 0.0f, 1.0f);
   glVertex3f(pNeg, pNeg, pPos);
   glVertex3f(pNeg, pPos, pPos);
   glVertex3f(pPos, pPos, pPos);
   glVertex3f(pPos, pNeg, pPos);

   End();
}
