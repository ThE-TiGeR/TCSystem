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
//  $Id: TCOpenGLGridRenderer.h 766 2007-10-24 22:42:17Z the_____tiger $
//*******************************************************************************

#ifndef _TCOGLGRIDRENDERER_H_
#define _TCOGLGRIDRENDERER_H_

#include "TCOpenGLRenderer.h"

class TCGridCartesian;       /* for Render() method                 */
class TCGridPolar;           /* for other Render() method           */
class TCGrid;                /* for renderDiagonalUserTransformed() */
class TCOglColor;            /* for member color object             */
class TCTriangleLightWeight; /* for RenderTriangle() method         */

/**
 * The Ogl Grid Renderer is responsible for rendering cartesian
 * and polar (at least planned) grids.  Currently, it is a 
 * member of the TCResamplerEventHandler class.
 *
 * @author larameer
 */

class TCOglGridRenderer: public TCOglRenderer
{
IMPLEMENT_FIRE_BASE(TCOglGridRenderer, TCOglRenderer)

public:

   /**
    * This is the constructor that is actually used.
    * <P>
    * @from
    * @param viewer a pointer to the OglViewer
    */
   TCOglGridRenderer(TCOglViewer* viewer);
   /** destructor */
   virtual ~TCOglGridRenderer();

   enum OglGridRendererConstant {

   };
   /**
    * @return TRUE of anti-aliasing has been turned on by
    *         the user.
    */
   inline bool AntiAliasOn() { return m_AntiAliasOn; } 

   /**
    * Toggles the user-specified anti-aliasing option.
    *
    * @from TCResamplerEventHandler::SelectCommandAntiAlias()
    */
   void ToggleAntiAlias();
      
   /**
    * @from TCResampler::renderGrid()
    * @param gridCartesian -the cartesian grid to render
    * @return TRUE if the grid was rendered successfully
    */
   bool Render(TCGridCartesian& gridCartesian);

   /**
    * @from TCResampler::renderGrid()
    * @param gridPolar -the polar grid to render
    * @return TRUE if the grid was rendered successfully
    */
   bool Render(TCGridPolar& gridPolar);

   /**
    * This is mainly used for testing purposes.
    *
    * @from TCResampler::DrawIt()
    * @param triangleLW -the triangle object to render
    * @return TRUE if the grid was rendered successfully
    */
   bool RenderTriangle(const TCTriangleLightWeight& triangleLW);

   /**
    * This is mainly used for testing purposes.
    *
    * @from TCResampler::DrawIt()
    * @param center -the center of the circle to render
    * @param radius -the radius of the circle
    * @param color  -the color of the circle
    * @return TRUE if the grid was rendered successfully
    */
   bool RenderCircle(const TC::Math::Coord3D<float>& center,
                        const float radius,
                        const TCOglColor color);

private:

   /** the color of this grid */
   TCOglColor m_GridColor;
   /** an anti-aliasing flag */
   bool m_AntiAliasOn;
   
   /** @return a reference to this grid renderer's color object */
   TCOglColor& getColorRef() { return m_GridColor; }

   /**
    * We can render the grid a little faster if we already know
    * what plane it is in.
    *
    * @from TCOglGrid::Render()
    * @return TRUE if the grid was rendered successfully
    */
   bool renderInYZ(TCGridCartesian& gridCartesian);

   /**
    * We can render the grid a little faster if we already know
    * what plane it is in.
    *
    * @from TCOglGrid::Render()
    * @return TRUE if the grid was rendered successfully
    */
   bool renderInXY(TCGridCartesian& gridCartesian);

   /**
    * We can render the grid a little faster if we already know
    * what plane it is in.
    *
    * @from TCOglGrid::Render()
    * @return TRUE if the grid was rendered successfully
    */
   bool renderInXZ(TCGridCartesian& gridCartesian);
   /**
    * @from TCOglGrid::Render()
    * @return TRUE if the grid was rendered successfully
    */
   bool renderInYZplane(TCGridPolar& gridPolar);
   /**
    * @from TCOglGrid::Render()
    * @return TRUE if the grid was rendered successfully
    */
   bool renderInXZplane(TCGridPolar& gridPolar);
   /**
    * @from TCOglGrid::Render()
    * @return TRUE if the grid was rendered successfully
    */
   bool renderInXYplane(TCGridPolar& gridPolar);
   /**
    * Used for testing.
    * @from TCOglGrid::renderInYZplane()
    * @return TRUE if the grid was rendered successfully
    */
   bool render9oclockInYZplane(TCGridPolar& gridPolar);
   /**
    * Used for testing.
    * @from TCOglGrid::renderInXZplane()
    * @return TRUE if the grid was rendered successfully
    */
   bool render9oclockInXZplane(TCGridPolar& gridPolar);
   /**
    * Used for testing.
    * @from TCOglGrid::renderInXYplane()
    * @return TRUE if the grid was rendered successfully
    */
   bool render9oclockInXYplane(TCGridPolar& gridPolar);
   /**
    * Used for testing.
    * @from TCOglGrid::renderInAribraryPlane()
    * @return TRUE if the grid was rendered successfully
    */
   bool render9oclockInArbiPlane(TCGridPolar& gridPolar);
   /**
    * Used for testing.
    * @from TCOglGrid::renderInAribraryPlane()
    * @return TRUE if the grid was rendered successfully
    */
   bool render9oclockTransformedToXYplane(TCGridPolar& gridPolar);
   
   /**
    * Render this grid in an arbitrary plane.
    *
    * @from TCOglGrid::Render()
    * @return TRUE if the grid was rendered successfully
    */
   bool renderInPlaneArbitrary(TCGridCartesian& gridCartesian);
   /**
    * Render this grid in an arbitrary plane.
    *
    * @from TCOglGrid::Render()
    * @return TRUE if the grid was rendered successfully
    */
   bool renderInPlaneArbitrary(TCGridPolar& gridPolar);
   /**
    * Render the given grid after it has been transformed from an
    * arbitrary plane into the XY plane.  Used for testing.
    *
    * @from TCOglGrid::Render()
    * @return TRUE if the grid was rendered successfully
    */
   bool renderInPlaneTransformedToXY(TCGridCartesian& gridCartesian);
   /**
    * Render the given grid after it has been transformed from an
    * arbitrary plane into the XY plane.  Used for testing.
    *
    * @from TCOglGrid::Render()
    * @return TRUE if the grid was rendered successfully
    */
   bool renderInPlaneTransformedToXY(TCGridPolar& gridPolar);
   /**
    * Renders the given grid's user-transformed diagonal line.
    *
    * @from TCResampler::RenderInXY()
    * @param gridCartesian -the cartesian grid whose diagonal render
    * @return TRUE if the diagonal was renderered successfully
    */
   bool renderDiagonalUserTransformed(TCGrid& grid);
   /**
    * Renders the given grid's diagonal line transformed from
    * an arbitrary plane to the XY plane.
    *
    * @from TCResampler::Render()
    * @param gridCartesian -the cartesian grid whose diagonal render
    * @return TRUE if the diagonal was renderered successfully
    */
   bool renderDiagonalInXYplane(TCGridCartesian& gridCartesian);
   /**
    * Renders the given grid's original diagonal line.
    *
    * @from TCResampler::Render()
    * @param gridCartesian -the cartesian grid whose diagonal render
    * @return TRUE if the diagonal was renderered successfully
    */
   bool renderDiagonalOriginal(TCGridCartesian& gridCartesian);

};
#endif /* _TCOGLGRIDRENDERER_ */
