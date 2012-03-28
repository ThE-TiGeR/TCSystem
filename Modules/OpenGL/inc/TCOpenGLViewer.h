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
// Copyright (C) 2003 - 2012 Thomas Goessler. All Rights Reserved. 
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

#ifndef _TC_OPENGL_VIEWER_H_
#define _TC_OPENGL_VIEWER_H_

#include "TCOpenGLMaterial.h"
#include "TCOpenGLObjectGroup.h"

//*******************************************************************************
//  This file is based on the FX::FXGLViewer of FOX 1.6.30
//*******************************************************************************

namespace tc
{

   namespace open_gl
   {
      // GL Viewer options
      enum 
      {
         VIEWER_LIGHTING = 0x00008000,    /// Lighting is on
         VIEWER_FOG      = 0x00010000,    /// Fog mode on
         VIEWER_DITHER   = 0x00020000,    /// Dithering
         VIEWER_CULLFACE = 0x00040000     /// enable GL_CULL_FACE
      };


      /*******************************  Viewer  Structs  *****************************/

      /// open_gl Viewer Viewport
      struct Viewport 
      {
         sint32   w,h;               // Viewport dimensions
         double   left,right;        // World box
         double   bottom,top;
         double   hither,yon;
      };

      // Feedback buffer sort routine
      typedef bool (*ZSortFunc)(float*& buffer,sint32& used,sint32& size);

      struct FXQuatf2Coord4f
      {
         FXQuatf2Coord4f(const FX::FXQuatf& q)
            :c((const FX::FXfloat*)q)
         {
         }

         operator const math::CoordN<float, 4>& ()
         {
            return c;
         }

      private:
         math::CoordN<float, 4> c;
      };

      struct Coord4f2FXQuatf
      {
         Coord4f2FXQuatf(const math::CoordN<float, 4>& c)
            :q((const FX::FXfloat*)c)
         {
         }

         operator const FX::FXQuatf& ()
         {
            return q;
         }

      private:
         FX::FXQuatf q;
      };

      /// open_gl viewer widget
      class TCOGL_API Viewer: public FX::FXGLCanvas 
      {
         FXDECLARE(Viewer)
      public:
         // Common DND types
         static FX::FXDragType objectType;     // GL Object type

         // Mouse actions when in viewing window
         enum OperationMode
         {
            HOVERING,                       // Hovering mouse w/o doing anything
            PICKING,                        // Pick mode
            ROTATING,                       // Rotating camera around target
            POSTING,                        // Posting right-mouse menu
            TRANSLATING,                    // Translating camera
            ZOOMING,                        // Zooming
            FOVING,                         // Change field-of-view
            DRAGGING,                       // Dragging objects
            TRUCKING,                       // Trucking camera
            GYRATING,                       // Rotation of camera around eye
            DO_LASSOSELECT,                 // Lasso select when mouse pressed
            LASSOSELECT,                    // Anchor of lasso rectangle
            DO_LASSOZOOM,                   // Zoom when mouse pressed
            LASSOZOOM                       // Zoom rectangle
         };

      protected:
         Viewer();
         void glsetup();
         virtual void updateProjection();
         virtual void updateTransform();
         Vertex3D spherePoint(sint32 px,sint32 py);
         FX::FXQuatf turn(sint32 fx,sint32 fy,sint32 tx,sint32 ty);
         virtual void DrawScene(Viewport& wv);
         void drawAnti(Viewport& wv);
         void drawLasso(sint32 x0,sint32 y0,sint32 x1,sint32 y1);
         sint32 selectHits(uint32*& hits,sint32& nhits,sint32 x,sint32 y,sint32 w,sint32 h);
         sint32 renderFeedback(float *buffer,sint32 x,sint32 y,sint32 w,sint32 h,sint32 maxbuffer);
         void drawFeedback(FX::FXDCPrint& pdc,const float* buffer,sint32 used);
         virtual ObjectPtr processHits(const uint32 *pickbuffer,sint32 nhits);
         void setOp(OperationMode o);

      private:
         void Init();
         void Redraw();

      public:

         // Events
         long onPaint(FX::FXObject*,FX::FXSelector,void*);
         long onEnter(FX::FXObject*,FX::FXSelector,void*);
         long onLeave(FX::FXObject*,FX::FXSelector,void*);
         long onMotion(FX::FXObject*,FX::FXSelector,void*);
         long onMouseWheel(FX::FXObject*,FX::FXSelector,void*);
         long onChanged(FX::FXObject*,FX::FXSelector,void*);
         long onPick(FX::FXObject*,FX::FXSelector,void*);
         long onClicked(FX::FXObject*,FX::FXSelector,void*);
         long onDoubleClicked(FX::FXObject*,FX::FXSelector,void*);
         long onTripleClicked(FX::FXObject*,FX::FXSelector,void*);
         long onLassoed(FX::FXObject*,FX::FXSelector,void*);
         long onSelected(FX::FXObject*,FX::FXSelector,void*);
         long onDeselected(FX::FXObject*,FX::FXSelector,void*);
         long onInserted(FX::FXObject*,FX::FXSelector,void*);
         long onDeleted(FX::FXObject*,FX::FXSelector,void*);
         long onLeftBtnPress(FX::FXObject*,FX::FXSelector,void*);
         long onLeftBtnRelease(FX::FXObject*,FX::FXSelector,void*);
         long onMiddleBtnPress(FX::FXObject*,FX::FXSelector,void*);
         long onMiddleBtnRelease(FX::FXObject*,FX::FXSelector,void*);
         long onRightBtnPress(FX::FXObject*,FX::FXSelector,void*);
         long onRightBtnRelease(FX::FXObject*,FX::FXSelector,void*);
         long onUngrabbed(FX::FXObject*,FX::FXSelector,void*);
         long onKeyPress(FX::FXObject*,FX::FXSelector,void*);
         long onKeyRelease(FX::FXObject*,FX::FXSelector,void*);
         long onFocusIn(FX::FXObject*,FX::FXSelector,void*);
         long onFocusOut(FX::FXObject*,FX::FXSelector,void*);
         long onClipboardLost(FX::FXObject*,FX::FXSelector,void*);
         long onClipboardGained(FX::FXObject*,FX::FXSelector,void*);
         long onClipboardRequest(FX::FXObject*,FX::FXSelector,void*);

         // Commands
         long OnCmdPerspective(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdPerspective(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdParallel(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdParallel(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdFront(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdFront(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdBack(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdBack(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdLeft(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdLeft(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdRight(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdRight(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdTop(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdTop(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdBottom(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdBottom(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdResetView(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdFitView(FX::FXObject*,FX::FXSelector,void*);
         long onDNDEnter(FX::FXObject*,FX::FXSelector,void*);
         long onDNDLeave(FX::FXObject*,FX::FXSelector,void*);
         long onDNDMotion(FX::FXObject*,FX::FXSelector,void*);
         long onDNDDrop(FX::FXObject*,FX::FXSelector,void*);
         long onTipTimer(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdXYZDial(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdXYZDial(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdRollPitchYaw(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdRollPitchYaw(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdXYZScale(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdXYZScale(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdCurrent(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdBackColor(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdBackColor(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdGradientBackColor(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdGradientBackColor(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdAmbientColor(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdAmbientColor(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdLighting(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdLighting(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdFog(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdFog(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdDither(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdDither(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdCullFace(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdCullFace(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdFov(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdFov(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdZoom(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdZoom(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdLightAmbient(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdLightAmbient(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdLightDiffuse(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdLightDiffuse(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdLightSpecular(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdLightSpecular(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdTurbo(FX::FXObject*,FX::FXSelector,void*);
         long OnUpdTurbo(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdPrintImage(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdPrintVector(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdLassoZoom(FX::FXObject*,FX::FXSelector,void*);
         long OnCmdLassoSelect(FX::FXObject*,FX::FXSelector,void*);
         long onQueryHelp(FX::FXObject*,FX::FXSelector,void*);
         long onQueryTip(FX::FXObject*,FX::FXSelector,void*);
         virtual long onDefault(FX::FXObject*,FX::FXSelector,void*);

      public:

         // Projection modes
         enum 
         {
            PARALLEL,		  // Parallel projection
            PERSPECTIVE		  // Perspective projection
         };

         // Messages
         enum 
         {
            ID_PERSPECTIVE=FX::FXGLCanvas::ID_LAST,
            ID_PARALLEL,
            ID_FRONT,
            ID_BACK,
            ID_LEFT,
            ID_RIGHT,
            ID_TOP,
            ID_BOTTOM,
            ID_RESETVIEW,
            ID_FITVIEW,
            ID_TOP_LEFT_COLOR,
            ID_TOP_RIGHT_COLOR,
            ID_BOTTOM_LEFT_COLOR,
            ID_BOTTOM_RIGHT_COLOR,
            ID_BACK_COLOR,
            ID_AMBIENT_COLOR,
            ID_LIGHT_AMBIENT,
            ID_LIGHT_DIFFUSE,
            ID_LIGHT_SPECULAR,
            ID_LIGHTING,
            ID_TURBO,
            ID_FOG,
            ID_DITHER,
            ID_CULL_FACE,
            ID_SCALE_X,
            ID_SCALE_Y,
            ID_SCALE_Z,
            ID_DIAL_X,
            ID_DIAL_Y,
            ID_DIAL_Z,
            ID_ROLL,
            ID_PITCH,
            ID_YAW,
            ID_FOV,
            ID_ZOOM,
            ID_PRINT_IMAGE,
            ID_PRINT_VECTOR,
            ID_LASSO_ZOOM,
            ID_LASSO_SELECT,
            ID_LAST
         };

      public:

         // Common DND type names
         static const FX::FXchar objectTypeName[];

      public:

         /// Construct GL viewer widget
         Viewer(FX::FXComposite* p,FX::FXGLVisual *vis,FX::FXObject* tgt=0,FX::FXSelector sel=0,uint32 opts=0,sint32 x=0,sint32 y=0,sint32 w=0,sint32 h=0);

         /// Construct GL viewer widget sharing display list with another GL viewer
         Viewer(FX::FXComposite* p,FX::FXGLVisual *vis,Viewer* sharegroup,FX::FXObject* tgt=0,FX::FXSelector sel=0,uint32 opts=0,sint32 x=0,sint32 y=0,sint32 w=0,sint32 h=0);

         /// Create all of the server-side resources for this window
         virtual void create();

         /// Detach server-side resources
         virtual void detach();

         /// Perform layout
         virtual void layout();

         /// Return size of pixel in world coordinates
         double worldPix() const { return worldpx; }

         /// Return size of pixel in model coordinates
         double modelPix() const { return modelpx; }

         /// fills thw vector with the selected objects an returns true if found
         bool lasso(sint32 x1,sint32 y1,sint32 x2,sint32 y2, std::vector<ObjectPtr>& selected_objects);

         /// fills thw vector with the selected objects an returns true if found
         virtual bool select(sint32 x,sint32 y,sint32 w,sint32 h, std::vector<ObjectPtr>& selected_objects);

         /// Perform a pick operation, returning the object at the given x,y position, or 0
         virtual ObjectPtr pick(sint32 x,sint32 y);

         /// Change the model bounding box; this adjusts the viewer
         virtual bool setBounds(const BoundingBox3D& box);

         /// Fit viewer to the given bounding box
         bool fitToBounds(const BoundingBox3D& box);

         /// Return the viewer's viewport
         void getViewport(Viewport& v) const;

         /// Translate eye-coordinate to screen coordinate
         void eyeToScreen(sint32& sx,sint32& sy,Vertex3D e);

         /// Translate screen coordinate to eye coordinate at the given depth
         Vertex3D screenToEye(sint32 sx,sint32 sy,float eyez=0.0);

         /// Translate screen coordinate to eye coordinate at the target point depth
         Vertex3D screenToTarget(sint32 sx,sint32 sy);

         /// Translate world coordinate to eye coordinate
         Vertex3D worldToEye(Vertex3D w);

         /// Translate world coordinate to eye coordinate depth
         float worldToEyeZ(Vertex3D w);

         /// Translate eye coordinate to eye coordinate
         Vertex3D eyeToWorld(Vertex3D e);

         /// Calculate world coordinate vector from screen movement
         Vertex3D worldVector(sint32 fx,sint32 fy,sint32 tx,sint32 ty);

         ///  Change default object material setting
         void setMaterial(const Material &mtl);

         /// Return default object material setting
         void getMaterial(Material &mtl) const;

         /// Change camera field of view angle (in degrees)
         void setFieldOfView(double fv);

         /// Return camera field of view angle
         double getFieldOfView() const { return fov; }

         /// Change camera zoom factor
         void setZoom(double zm);

         /// Return camera zoom factor
         double getZoom() const { return zoom; }

         /// Change target point distance
         void setDistance(double ed);

         /// Return target point distance
         double getDistance() const { return distance; }

         /// Return target point distance
         double getDiameter() const { return diameter; }

         /// Change unequal model scaling factors
         void setScale(Vertex3D s);

         /// Return current scaling factors
         const Vertex3D& getScale() const { return scale; }

         /// Change camera orientation from quaternion
         void setOrientation(FX::FXQuatf rot);

         /// Return current camera orientation quaternion
         math::CoordN<float, 4> getOrientation() const { return FXQuatf2Coord4f(rotation); }

         /// Change object center (tranlation)
         void setCenter(Vertex3D cntr);

         /// Return object center
         const Vertex3D& getCenter() const { return center; }

         /// Translate object center
         void translate(Vertex3D vec);

         /// Return boresight vector
         bool getBoreVector(sint32 sx,sint32 sy,Vertex3D& point,Vertex3D& dir);

         /// Return eyesight vector
         Vertex3D getEyeVector() const;

         /// Return eye position
         Vertex3D getEyePosition() const;

         /// Change help text
         void setHelpText(const FX::FXString& text);

         /// Return help text
         const FX::FXString& getHelpText() const { return help; }

         /// Change tip text
         void setTipText(const FX::FXString&  text);

         /// Return tip text
         const FX::FXString& getTipText() const { return tip; }

         /// Return the current transformation matrix
         const FX::FXMat4f& getTransform() const { return m_transformation; }

         /// Return the inverse of the current transformation matrix
         const FX::FXMat4f& getInvTransform() const { return itransform; }

         /// Change the scene, i.e. the object being displayed.
         void SetScene(ObjectGroupPtr sc);

         /// Return the current scene object
         ObjectGroupPtr GetScene() const { return scene; }

         /// Change the projection mode, PERSPECTIVE or PARALLEL
         void setProjection(uint32 proj);

         /// Return the projection mode
         uint32 getProjection() const { return projection; }

         /// Change top or bottom or both background colors
         void setBackgroundColor(const Color& clr, sint32 idx);

         /// Return top or bottom window background color.
         const Color& getBackgroundColor(sint32 idx) const { return m_background[idx]; }

         /// Change global ambient light color
         void setAmbientColor(const Color& clr);

         /// Return global ambient light color
         const Color& getAmbientColor() const { return ambient; }

         /**
         * Read the pixels off the screen as array of FX::FXColor;
         * this array can be directly passed to fxsaveBMP and other image
         * output routines.
         */
         bool readPixels(std::vector<FX::FXColor>& buffer,uint32 x,uint32 y,uint32 w,uint32 h);

         /**
         * Read the feedback buffer containing the current scene, returning used
         * and allocated size.
         */
         bool readFeedback(float*& buffer,sint32& used,sint32& size,sint32 x,sint32 y,sint32 w,sint32 h);

         /**
         * Change hidden-surface feedback buffer sorting algorithm.
         * This can be used for move/draw printed output depth sorting.
         */
         void setZSortFunc(ZSortFunc func){ zsortfunc=func; }

         /// Return hidden surface sorting function.
         ZSortFunc getZSortFunc() const { return zsortfunc; }

         /**
         * Change the maximum hits, i.e. the maximum size of the pick buffer.
         * When set to less than or equal to zero, picking is essentially turned off.
         */
         void setMaxHits(sint32 maxh) { maxhits=maxh; }

         /// Return maximum pickbuffer size
         sint32 getMaxHits() const { return maxhits; }

         /**
         * When drawing a GL object, if DoesTurbo() is true, the object
         * may choose to perform a reduced complexity drawing as the user is
         * interactively manipulating; another update will be done later when
         * the full complexity drawing can be performed again.
         */
         bool DoesTurbo() const { return doesturbo; }

         /// Return turbo mode setting
         bool GetTurboMode() const { return turbomode; }

         /// Set turbo mode
         void SetTurboMode(bool turbo=true);

         /// Get Current operationMode
         OperationMode GetOperationMode() const {return m_op_mode;}

         /// Return light source settings
         void getLight(Light& lite) const;

         /// Change light source settings
         void setLight(const Light& lite);

         /// Change m_selected_objects
         void SetSelection(const ObjectPtrVector& sel);
         void SetSelection(const std::vector<std::string>& sel);
         /// Return m_selected_objects
         const ObjectPtrVector& GetSelection() const { return m_selected_objects; }

         bool MakeCurrent() {return makeCurrent() == TRUE;}
         bool MakeNonCurrent() {return makeNonCurrent() == TRUE;}

         /// Destructor
         virtual ~Viewer();
      private:
         Viewport      m_view_port;              // Window viewport transform
         FX::FXMat4f         m_transformation;        // Current transformation matrix
         FX::FXMat4f         itransform;       // Inverse of current transformation matrix
         uint32          projection;       // Projection mode
         FX::FXQuatf         rotation;         // Viewer orientation
         double        fov;              // Field of view
         double        zoom;             // Zoom factor
         Vertex3D         center;           // Model center
         Vertex3D         scale;            // Model scale
         double        worldpx;          // Pixel size in world
         double        modelpx;          // Pixel size in model
         sint32           maxhits;          // Maximum number of hits
         double        ax,ay;            // Quick view->world coordinate mapping
         double        diameter;         // Size of model diameter (always > 0)
         double        distance;         // Distance of PRP to target
         /** Background colors for each corner for clearing */
         Color m_background[4];
         Color         ambient;          // Global ambient light
         Light           light;            // Light source
         Material        material;         // Base material properties
         sint32           dial[3];          // Dial positions
         FX::FXString        help;             // Status help
         FX::FXString        tip;              // Tooltip for background
         ObjectPtr     dropped;          // Object being dropped on
         ObjectPtrVector     m_selected_objects;        // Current object
         ZSortFunc     zsortfunc;        // Routine to sort feedback buffer
         ObjectGroupPtr     scene;            // What we're looking at
         bool          doesturbo;        // Doing turbo mode
         bool          turbomode;        // Turbo mode
         OperationMode m_op_mode;             // Mode the widget is in
         double        m_redraw_time;    // time needed for last redraw
      };

      typedef SharedPtr<Viewer> ViewerPtr;
   }
}

#endif // _TC_OPENGL_VIEWER_H_


