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

#include "TCOpenGLViewer.h"

#include "TCOpenGLObject.h"
#include "TCOpenGLUtil.h"
#include <fxkeys.h>
#include <limits>

#include "TCNewEnable.h"

/*
To Do:
- Initialize GL to fastest of everything for drawing lines.
- Group object needs current element.
- use app->getDragDelta() for motion tolerance.
- Default op to noop mode; all returns 0 in noop mode.
- GLuint unfortunately not always big enough to store a pointer...
- The m_selected_objects seems to get lost with multiple viewers into.
the same scene.  If you select a cube in one view, then select another
cube in another view, both seem to get selected.  Can we push the
"m_selected_objects" member from the view to the scene object?
- Instead of select/deselect, do focus gain/lost type deal.
- Add methods for inquire of pick-ray.
- Fix FXGLGroup to identify child or itself..
- Need some way of updating ALL viewers.
- Need a document/view type concept?
- Load/Save need to save more...
- Build mini display lists for offset/surface drawing.
- Pass clicked/double-clicked/triple-clicked messages to object.
- Distinguish between current object and selected ones.
only one is current, but many may be selected.
- When object(s) deleted, need to fix up m_selected_objects...
- GLViewer should source some messages to its target for important events.
- Zoom-lasso feature.
- Basic mouse actions:

State     Event      Modifiers         Where           State          Meaning
--------------------------------------------------------------------------------------------
HOVERING  Left       --                outside         PICKING        Pick an object if no move
PICKING   Motion     --                --              ROTATING       Rotate camera about target point
HOVERING  Left       --                inside object   DRAGGING       Drag object
HOVERING  Left       Shift             --              LASSOSELECT    Select
HOVERING  Left       Control           --              LASSOSELECT    Toggle m_selected_objects
HOVERING  Left       Right             --              ZOOMING        Zoom in
HOVERING  Left       Right + Shift     --              TRUCKING       Trucking camera
HOVERING  Middle     --                --              ZOOMING        Zoom in/out
HOVERING  Middle     Shift             --              TRUCKING       Trucking camera
HOVERING  Right      --                --              POSTING        Post popup menu if no move
POSTING   Motion     --                --              TRANSLATING    Translate camera
HOVERING  Right      Shift             --              GYRATING       Rotate object about camera
HOVERING  Right      Control           --              FOVING         Change field of view
HOVERING  Right      Left              --              ZOOMING        Zoom in
HOVERING  Right      Left +Shift       --              TRUCKING       Trucking camera

FIXME FIXME FIXME FIXME FIXME FIXME FIXME

Should remove "m_selected_objects" member.  Viewer should have no knowledge
of any GL object besides scene.

Should delegate all messages it does not understand to "target" and not
to current object.

Picking, m_selected_objects should pass list of records in callback; this list
should NOT contain zmin, zmax, but just, for each record: number of
names, and the names.  List is ended by 0 (no names) record.

The list should be sorted based on zmin, zmax, with first record in list
being the one closest to the eye.

Should add FXGLLight objects, which can be added to FXGLGroups.

Should add subclass of FXGLGroup which pushes/pops attributes.

Note that we will keep the camera state in the Viewer widget, i.e.
won't have camera objects.

FIXME FIXME FIXME FIXME FIXME FIXME FIXME

- Need 3 dials to rotate about model (not screen) axes.

- Zoom centered on point at which you started the zoom operation.
*/


// Size of pick buffer
#define MAX_PICKBUF    1024

// Maximum length of m_selected_objects path
#define MAX_SELPATH    64

// Rotation tolerance
#define EPS            1.0E-2

// Pick tolerance
#define PICK_TOL       3

#include "TCOutput.h"
#include "TCTime.h"

using namespace FX;

#ifdef _MSC_VER
#pragma warning(disable : 4390 )
#pragma warning(disable : 4311 )
#pragma warning(disable : 4312 )
#endif

/*******************************************************************************/

namespace tc
{
   namespace open_gl
   {
      // Map
      FXDEFMAP(Viewer) FXGLViewerMap[]=
      {
         FXMAPFUNC(SEL_PAINT,0,Viewer::onPaint),
         FXMAPFUNC(SEL_MOTION,0,Viewer::onMotion),
         FXMAPFUNC(SEL_MOUSEWHEEL,0,Viewer::onMouseWheel),
         FXMAPFUNC(SEL_TIMEOUT,Viewer::ID_TIPTIMER,Viewer::onTipTimer),
         FXMAPFUNC(SEL_DND_ENTER,0,Viewer::onDNDEnter),
         FXMAPFUNC(SEL_DND_LEAVE,0,Viewer::onDNDLeave),
         FXMAPFUNC(SEL_DND_DROP,0,Viewer::onDNDDrop),
         FXMAPFUNC(SEL_DND_MOTION,0,Viewer::onDNDMotion),
         FXMAPFUNC(SEL_ENTER,0,Viewer::onEnter),
         FXMAPFUNC(SEL_LEAVE,0,Viewer::onLeave),
         FXMAPFUNC(SEL_LEFTBUTTONPRESS,0,Viewer::onLeftBtnPress),
         FXMAPFUNC(SEL_LEFTBUTTONRELEASE,0,Viewer::onLeftBtnRelease),
         FXMAPFUNC(SEL_MIDDLEBUTTONPRESS,0,Viewer::onMiddleBtnPress),
         FXMAPFUNC(SEL_MIDDLEBUTTONRELEASE,0,Viewer::onMiddleBtnRelease),
         FXMAPFUNC(SEL_RIGHTBUTTONPRESS,0,Viewer::onRightBtnPress),
         FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,0,Viewer::onRightBtnRelease),
         FXMAPFUNC(SEL_UNGRABBED,0,Viewer::onUngrabbed),
         FXMAPFUNC(SEL_KEYPRESS,0,Viewer::onKeyPress),
         FXMAPFUNC(SEL_KEYRELEASE,0,Viewer::onKeyRelease),
         FXMAPFUNC(SEL_FOCUSIN,0,Viewer::onFocusIn),
         FXMAPFUNC(SEL_FOCUSOUT,0,Viewer::onFocusOut),
         FXMAPFUNC(SEL_CHANGED,0,Viewer::onChanged),
         FXMAPFUNC(SEL_CLICKED,0,Viewer::onClicked),
         FXMAPFUNC(SEL_DOUBLECLICKED,0,Viewer::onDoubleClicked),
         FXMAPFUNC(SEL_TRIPLECLICKED,0,Viewer::onTripleClicked),
         FXMAPFUNC(SEL_LASSOED,0,Viewer::onLassoed),
         FXMAPFUNC(SEL_SELECTED,0,Viewer::onSelected),
         FXMAPFUNC(SEL_DESELECTED,0,Viewer::onDeselected),
         FXMAPFUNC(SEL_INSERTED,0,Viewer::onInserted),
         FXMAPFUNC(SEL_DELETED,0,Viewer::onDeleted),
         FXMAPFUNC(SEL_PICKED,0,Viewer::onPick),
         FXMAPFUNC(SEL_CLIPBOARD_LOST,0,Viewer::onClipboardLost),
         FXMAPFUNC(SEL_CLIPBOARD_GAINED,0,Viewer::onClipboardGained),
         FXMAPFUNC(SEL_CLIPBOARD_REQUEST,0,Viewer::onClipboardRequest),
         FXMAPFUNC(SEL_QUERY_TIP,0,Viewer::onQueryTip),
         FXMAPFUNC(SEL_QUERY_HELP,0,Viewer::onQueryHelp),
         FXMAPFUNCS(SEL_UPDATE,Viewer::ID_DIAL_X,Viewer::ID_DIAL_Z,Viewer::OnUpdXYZDial),
         FXMAPFUNCS(SEL_CHANGED,Viewer::ID_DIAL_X,Viewer::ID_DIAL_Z,Viewer::OnCmdXYZDial),
         FXMAPFUNCS(SEL_COMMAND,Viewer::ID_DIAL_X,Viewer::ID_DIAL_Z,Viewer::OnCmdXYZDial),
         FXMAPFUNCS(SEL_UPDATE,Viewer::ID_ROLL,Viewer::ID_YAW,Viewer::OnUpdRollPitchYaw),
         FXMAPFUNCS(SEL_COMMAND,Viewer::ID_ROLL,Viewer::ID_YAW,Viewer::OnCmdRollPitchYaw),
         FXMAPFUNCS(SEL_CHANGED,Viewer::ID_ROLL,Viewer::ID_YAW,Viewer::OnCmdRollPitchYaw),
         FXMAPFUNCS(SEL_UPDATE,Viewer::ID_SCALE_X,Viewer::ID_SCALE_Z,Viewer::OnUpdXYZScale),
         FXMAPFUNCS(SEL_COMMAND,Viewer::ID_SCALE_X,Viewer::ID_SCALE_Z,Viewer::OnCmdXYZScale),
         FXMAPFUNCS(SEL_CHANGED,Viewer::ID_SCALE_X,Viewer::ID_SCALE_Z,Viewer::OnCmdXYZScale),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_PERSPECTIVE,Viewer::OnUpdPerspective),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_PERSPECTIVE,Viewer::OnCmdPerspective),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_PARALLEL,Viewer::OnUpdParallel),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_PARALLEL,Viewer::OnCmdParallel),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_FRONT,Viewer::OnUpdFront),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_FRONT,Viewer::OnCmdFront),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_BACK,Viewer::OnUpdBack),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_BACK,Viewer::OnCmdBack),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_LEFT,Viewer::OnUpdLeft),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_LEFT,Viewer::OnCmdLeft),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_RIGHT,Viewer::OnUpdRight),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_RIGHT,Viewer::OnCmdRight),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_TOP,Viewer::OnUpdTop),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_TOP,Viewer::OnCmdTop),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_BOTTOM,Viewer::OnUpdBottom),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_BOTTOM,Viewer::OnCmdBottom),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_RESETVIEW,FXWindow::onUpdYes),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_RESETVIEW,Viewer::OnCmdResetView),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_FITVIEW,FXWindow::onUpdYes),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_FITVIEW,Viewer::OnCmdFitView),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_BACK_COLOR,Viewer::OnUpdBackColor),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_BACK_COLOR,Viewer::OnCmdBackColor),
         FXMAPFUNC(SEL_CHANGED,Viewer::ID_BACK_COLOR,Viewer::OnCmdBackColor),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_AMBIENT_COLOR,Viewer::OnUpdAmbientColor),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_AMBIENT_COLOR,Viewer::OnCmdAmbientColor),
         FXMAPFUNC(SEL_CHANGED,Viewer::ID_AMBIENT_COLOR,Viewer::OnCmdAmbientColor),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_LIGHTING,Viewer::OnUpdLighting),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_LIGHTING,Viewer::OnCmdLighting),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_FOG,Viewer::OnUpdFog),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_DITHER,Viewer::OnUpdDither),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_CULL_FACE,Viewer::OnUpdCullFace),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_LIGHT_AMBIENT,Viewer::OnUpdLightAmbient),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_LIGHT_AMBIENT,Viewer::OnCmdLightAmbient),
         FXMAPFUNC(SEL_CHANGED,Viewer::ID_LIGHT_AMBIENT,Viewer::OnCmdLightAmbient),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_LIGHT_DIFFUSE,Viewer::OnUpdLightDiffuse),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_LIGHT_DIFFUSE,Viewer::OnCmdLightDiffuse),
         FXMAPFUNC(SEL_CHANGED,Viewer::ID_LIGHT_DIFFUSE,Viewer::OnCmdLightDiffuse),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_LIGHT_SPECULAR,Viewer::OnUpdLightSpecular),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_LIGHT_SPECULAR,Viewer::OnCmdLightSpecular),
         FXMAPFUNC(SEL_CHANGED,Viewer::ID_LIGHT_SPECULAR,Viewer::OnCmdLightSpecular),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_TURBO,Viewer::OnUpdTurbo),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_TURBO,Viewer::OnCmdTurbo),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_PRINT_IMAGE,Viewer::onUpdYes),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_PRINT_VECTOR,Viewer::onUpdYes),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_LASSO_ZOOM,Viewer::onUpdYes),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_LASSO_ZOOM,Viewer::OnCmdLassoZoom),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_LASSO_SELECT,Viewer::onUpdYes),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_LASSO_SELECT,Viewer::OnCmdLassoSelect),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_FOG,Viewer::OnCmdFog),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_DITHER,Viewer::OnCmdDither),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_CULL_FACE,Viewer::OnCmdCullFace),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_FOV,Viewer::OnUpdFov),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_FOV,Viewer::OnCmdFov),
         FXMAPFUNC(SEL_CHANGED,Viewer::ID_FOV,Viewer::OnCmdFov),
         FXMAPFUNC(SEL_UPDATE,Viewer::ID_ZOOM,Viewer::OnUpdZoom),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_ZOOM,Viewer::OnCmdZoom),
         FXMAPFUNC(SEL_CHANGED,Viewer::ID_ZOOM,Viewer::OnCmdZoom),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_PRINT_IMAGE,Viewer::OnCmdPrintImage),
         FXMAPFUNC(SEL_COMMAND,Viewer::ID_PRINT_VECTOR,Viewer::OnCmdPrintVector),

         FXMAPFUNCS(SEL_UPDATE,Viewer::ID_TOP_LEFT_COLOR,Viewer::ID_BOTTOM_RIGHT_COLOR,Viewer::OnUpdGradientBackColor),
         FXMAPFUNCS(SEL_COMMAND,Viewer::ID_TOP_LEFT_COLOR,Viewer::ID_BOTTOM_RIGHT_COLOR,Viewer::OnCmdGradientBackColor),
         FXMAPFUNCS(SEL_CHANGED,Viewer::ID_TOP_LEFT_COLOR,Viewer::ID_BOTTOM_RIGHT_COLOR,Viewer::OnCmdGradientBackColor),
      };


      // Object implementation
      FXIMPLEMENT(Viewer,FXGLCanvas,FXGLViewerMap,ARRAYNUMBER(FXGLViewerMap))


         /*******************************************************************************/

         // Drag type names for generic object
         const FXchar Viewer::objectTypeName[]="application/x-globject";


      // Drag type for generic object
      FXDragType Viewer::objectType=0;


      /*******************************************************************************/


      // For deserialization
      Viewer::Viewer(){
         flags|=FLAG_ENABLED|FLAG_DROPTARGET;
         dial[0]=0;
         dial[1]=0;
         dial[2]=0;
         zsortfunc=0;
         doesturbo=false;
         mode=HOVERING;
      }


      // Construct GL viewer widget with private display list
      Viewer::Viewer(FXComposite* p,FXGLVisual *vis,FXObject* tgt,FXSelector sel,uint32 opts,sint32 x,sint32 y,sint32 w,sint32 h):
      FXGLCanvas(p,vis,0,tgt,sel,opts,x,y,w,h){
         Init();
      }


      // Construct GL viewer widget with shared display list
      Viewer::Viewer(FXComposite* p,FXGLVisual *vis,Viewer* sharegroup,FXObject* tgt,FXSelector sel,uint32 opts,sint32 x,sint32 y,sint32 w,sint32 h):
      FXGLCanvas(p,vis,sharegroup,tgt,sel,opts,x,y,w,h){
         Init();
      }


      // Common initialization for constructor
      void Viewer::Init()
      {
         flags|=FLAG_ENABLED|FLAG_DROPTARGET;
         defaultCursor=getApp()->getDefaultCursor(DEF_CROSSHAIR_CURSOR);
         dragCursor=getApp()->getDefaultCursor(DEF_CROSSHAIR_CURSOR);
         projection=PARALLEL;                       // Projection
         zoom=1.0;                                     // Zoom factor
         fov=30.0;                                     // Field of view (1 to 90)
         m_view_port.left=-1.0;                                // Init world box
         m_view_port.right=1.0;
         m_view_port.top=1.0;
         m_view_port.bottom=-1.0;
         m_view_port.hither=0.1;
         m_view_port.yon=1.0;
         m_view_port.w=100;                                    // Viewport width
         m_view_port.h=100;                                    // Viewport height
         diameter=2.0;                                 // Size of model
         distance=7.464116;                            // Distance of PRP to target
         rotation[0]=0.0f;                              // Orientation
         rotation[1]=0.0f;
         rotation[2]=0.0f;
         rotation.w=1.0f;
         center[0]=0.0f;                                // Model center
         center[1]=0.0f;
         center[2]=0.0f;
         scale[0]=1.0f;                                 // Model scaling
         scale[1]=1.0f;
         scale[2]=1.0f;
         updateProjection();                           // Initial projection
         updateTransform();                            // Set transformation
         maxhits=512;                                  // Maximum hit buffer size
         m_background[0]= Color(0.85f, 0.80f, 0.85f); // Top left background
         m_background[1]= Color(1.00f, 0.60f, 1.00f); // Top right background
         m_background[2]= Color(0.50f, 0.15f, 0.50f); // Bottom left background
         m_background[3]= Color(0.25f, 0.00f, 0.25f); // Bottom right background

         ambient[0]=0.2f;                              // Scene ambient
         ambient[1]=0.2f;
         ambient[2]=0.2f;
         ambient[3]=1.0f;
         light.ambient[0]=0.0f;                        // Light ambient
         light.ambient[1]=0.0f;
         light.ambient[2]=0.0f;
         light.ambient[3]=1.0f;
         light.diffuse[0]=1.0f;                        // Light diffuse
         light.diffuse[1]=1.0f;
         light.diffuse[2]=1.0f;
         light.diffuse[3]=1.0f;
         light.specular[0]=0.0f;                       // Light specular
         light.specular[1]=0.0f;
         light.specular[2]=0.0f;
         light.specular[3]=1.0f;
         light.position[0]=-2.0f;                      // Light position
         light.position[1]= 2.0f;
         light.position[2]= 5.0f;
         light.position[3]= 0.0f;
         light.direction[0]= 0.0f;                     // Light direction
         light.direction[1]= 0.0f;
         light.direction[2]=-1.0f;
         light.exponent=0.0f;                          // Light spot exponent
         light.cutoff=180.0f;                          // Light spot cutoff
         light.c_attn=1.0f;                            // Light constant attenuation
         light.l_attn=0.0f;                            // Light linear attenuation
         light.q_attn=0.0f;                            // Light quadratic attenuation
         material.ambient[0]=0.2f;                     // Material ambient reflectivity
         material.ambient[1]=0.2f;
         material.ambient[2]=0.2f;
         material.ambient[3]=1.0f;
         material.diffuse[0]=0.8f;                     // Material diffuse reflectivity
         material.diffuse[1]=0.8f;
         material.diffuse[2]=0.8f;
         material.diffuse[3]=1.0f;
         material.specular[0]=1.0f;                    // Material specular reflectivity
         material.specular[1]=1.0f;
         material.specular[2]=1.0f;
         material.specular[3]=1.0f;
         material.emission[0]=0.0f;                    // Material emissivity
         material.emission[1]=0.0f;
         material.emission[2]=0.0f;
         material.emission[3]=1.0f;
         material.shininess=30.0f;                     // Material shininess
         dial[0]=0;                                    // Old dial position
         dial[1]=0;                                    // Old dial position
         dial[2]=0;                                    // Old dial position
         zsortfunc=0;                               // Routine to sort feedback buffer
         doesturbo=false;                              // In interaction
         turbomode=false;                              // Turbo mode
         mode=HOVERING;                                // Mouse operation
      }


      // Create window
      void Viewer::create()
      {
         // We would like to have this be true
         FXASSERT(sizeof(uint32)==sizeof(GLuint));

         // Create Window
         FXGLCanvas::create();

         // Set up open_gl environment
         glsetup();

         // Register drag type for color
         if(!colorType){colorType=getApp()->registerDragType(colorTypeName);}
         if(!objectType){objectType=getApp()->registerDragType(objectTypeName);}

         // If have scene already, get correct bounds
         BoundingBox3D r;
         if(scene) scene->GetBoundingBox(r);

         // Set initial viewing volume
         setBounds(r);
         update();
      }


      // Detach window
      void Viewer::detach(){
         FXGLCanvas::detach();
         colorType=0;
         objectType=0;
      }


      /*********************  V i e w i n g   S e t u p  ***************************/


      // Set up GL context
      void Viewer::glsetup()
      {
         // Make GL context current
         if(makeCurrent()){

            // Initialize GL context
            glRenderMode(GL_RENDER);

            // Fast hints
            glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST);
            glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
            glHint(GL_FOG_HINT,GL_FASTEST);
            glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);
            glHint(GL_POINT_SMOOTH_HINT,GL_FASTEST);

            // Z-buffer test on
            glEnable(GL_DEPTH_TEST);
            //glDepthFunc(GL_LESS);
            glDepthFunc(GL_LEQUAL);
            glDepthRange(0.0,1.0);
            glClearDepth(1.0);
            glClearColor(m_background[0][0],m_background[0][1],m_background[0][2],m_background[0][3]);

            // No face culling
            glDisable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);

            // Two sided lighting
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,true);
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient);

            // Preferred blend over background
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

            // Light on
            glEnable(GL_LIGHT0);
            glLightfv(GL_LIGHT0,GL_AMBIENT,light.ambient);
            glLightfv(GL_LIGHT0,GL_DIFFUSE,light.diffuse);
            glLightfv(GL_LIGHT0,GL_SPECULAR,light.specular);
            glLightfv(GL_LIGHT0,GL_POSITION,light.position);
            glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light.direction);
            glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,light.exponent);
            glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,light.cutoff);
            glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,light.c_attn);
            glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,light.l_attn);
            glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,light.q_attn);

            // Viewer is close
            glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,true);

            // Material colors
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,material.ambient);
            glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,material.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,material.specular);
            glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,material.emission);
            glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,material.shininess);

            // Vertex colors change both diffuse and ambient
            glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
            glDisable(GL_COLOR_MATERIAL);

            // Simplest and fastest drawing is default
            glShadeModel(GL_FLAT);
            glDisable(GL_BLEND);
            glDisable(GL_LINE_SMOOTH);
            glDisable(GL_POINT_SMOOTH);
            glDisable(GL_COLOR_MATERIAL);

            // Lighting
            glDisable(GL_LIGHTING);

            // No normalization of normals (it's broken on some machines anyway)
            glDisable(GL_NORMALIZE);

            // Dithering if needed
            glDisable(GL_DITHER);

            // set polygon offset
            glPolygonOffset(0.5f, 0.002f);

            PrintOpenGLExtensions();
            PrintOpenGLBufferInfo();
            PrintOpenGLTextureSupport();

            makeNonCurrent();
         }
      }


      void Viewer::Redraw()
      {
         FXGLVisual *vis = (FXGLVisual*)getVisual();
         FXASSERT(xid);
         if(makeCurrent())
         {
            Time draw_start_time = Time::Now();
            DrawScene(m_view_port);
            if(vis->isDoubleBuffer()) swapBuffers();
            ::glFinish();
            ::glFlush();
            Time total_time = Time::Now() - draw_start_time;
            TCTRACE1("open_gl", 0, "Current fps = %f", 1000.0f/(total_time.ToMilliSeconds()+1));

            makeNonCurrent();
         }
      }

      // Render all the graphics into a world box
      void Viewer::DrawScene(Viewport& wv)
      {
         // Set viewport
         glViewport(0, 0, wv.w, wv.h);

         // Reset important stuff
         glShadeModel(GL_SMOOTH);
         glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         glDisable(GL_LIGHTING);
         glDisable(GL_ALPHA_TEST);
         glDisable(GL_BLEND);
         glDisable(GL_DITHER);
         glDisable(GL_FOG);
         glDisable(GL_LOGIC_OP);
         glDisable(GL_POLYGON_SMOOTH);
         glDisable(GL_POLYGON_STIPPLE);
         glDisable(GL_STENCIL_TEST);
         glDisable(GL_CULL_FACE);
         glDisable(GL_COLOR_MATERIAL);

         // Reset matrices
         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         glMatrixMode(GL_MODELVIEW);
         glLoadIdentity();

         // Clear to solid background
         glClearDepth(1.0);
         glClearColor(m_background[0][0],m_background[0][1],m_background[0][2],m_background[0][3]);
         if(m_background[0]==m_background[1])
         {
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
         }

         // Clear to gradient background
         else
         {
            glClear(GL_DEPTH_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);
            glBegin(GL_TRIANGLE_STRIP);
            Color center_color = m_background[3]*0.25f + m_background[2]*0.25f +
                                 m_background[1]*0.25f + m_background[0]*0.25f;
            glColor4fv(center_color); glVertex3f(-0.5f,-0.5f,0.0f); 
            glColor4fv(m_background[2]); glVertex3f(-1.0f,-1.0f,0.0f); 
            glColor4fv(m_background[3]); glVertex3f(1.0f,-1.0f,0.0f);
            glColor4fv(m_background[0]); glVertex3f(-1.0f, 1.0f,0.0f); 
            glColor4fv(m_background[1]); glVertex3f(1.0f, 1.0f,0.0f);
            glEnd();
         }

         // Depth test on by default
         glDepthMask(GL_TRUE);
         glEnable(GL_DEPTH_TEST);

         // Switch to projection matrix
         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         switch(projection)
         {
         case PARALLEL:
            glOrtho(wv.left,wv.right,wv.bottom,wv.top,wv.hither,wv.yon);
            break;
         case PERSPECTIVE:
            glFrustum(wv.left,wv.right,wv.bottom,wv.top,wv.hither,wv.yon);
            break;
         }

         // Switch to model matrix
         glMatrixMode(GL_MODELVIEW);
         glLoadIdentity();

         // Set light parameters
         glEnable(GL_LIGHT0);
         glLightfv(GL_LIGHT0,GL_AMBIENT,light.ambient);
         glLightfv(GL_LIGHT0,GL_DIFFUSE,light.diffuse);
         glLightfv(GL_LIGHT0,GL_SPECULAR,light.specular);
         glLightfv(GL_LIGHT0,GL_POSITION,light.position);
         glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light.direction);
         glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,light.exponent);
         glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,light.cutoff);
         glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,light.c_attn);
         glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,light.l_attn);
         glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,light.q_attn);

         // Default material colors
         glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,material.ambient);
         glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,material.diffuse);
         glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,material.specular);
         glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,material.emission);
         glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,material.shininess);

         // Color commands change both
         glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);

         // Global ambient light
         glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient);

         // Enable fog
         if(options&VIEWER_FOG)
         {
            glEnable(GL_FOG);
            glFogfv(GL_FOG_COLOR,m_background[0]);                // Disappear into the background
            //glFogf(GL_FOG_DENSITY,1.0f);
            glFogf(GL_FOG_START,(GLfloat)(distance-diameter));  // Range tight around model position
            glFogf(GL_FOG_END,(GLfloat)(distance+diameter));    // Far place same as clip plane:- clipped stuff is in the mist!
            glFogi(GL_FOG_MODE,GL_LINEAR);	                // Simple linear depth cueing
         }

         // Dithering
         if(options&VIEWER_DITHER)
         {
            glEnable(GL_DITHER);
         }

         // Enable lighting
         if(options&VIEWER_LIGHTING)
         {
            glEnable(GL_LIGHTING);
         }

         if (options&VIEWER_CULLFACE)
         {
            glEnable(GL_CULL_FACE);
         }

         // Set model matrix
         glLoadMatrixf(m_transformation);

         // Draw what's visible
         if(scene)
         {
            scene->Update(*this, 0, 0);
            scene->Draw(*this);
         }
      }


      // Render with anti-aliasing
      void Viewer::drawAnti(Viewport& wv)
      {
         static const double jitter[][2] = 
         {          // Eight
            {-0.334818,  0.435331},
            { 0.286438, -0.393495},
            { 0.459462,  0.141540},
            {-0.414498, -0.192829},
            {-0.183790,  0.082102},
            {-0.079263, -0.317383},
            { 0.102254,  0.299133},
            { 0.164216, -0.054399}
         };

         Viewport jt=wv;
         double d=0.5*worldpx;
         register uint32 i;
         glClearAccum(0.0f,0.0f,0.0f,0.0f);
         glClear(GL_ACCUM_BUFFER_BIT);
         for(i=0; i<ARRAYNUMBER(jitter); i++)
         {
            jt.left = wv.left+jitter[i][0]*d;
            jt.right = wv.right+jitter[i][0]*d;
            jt.top = wv.top+jitter[i][1]*d;
            jt.bottom = wv.bottom+jitter[i][1]*d;
            DrawScene(jt);
            glAccum(GL_ACCUM,1.0/ARRAYNUMBER(jitter));
         }
         glAccum(GL_RETURN,1.0);
      }


      // Fill select buffer with hits in rectangle
      sint32 Viewer::selectHits(uint32*& hits,sint32& nhits,sint32 x,sint32 y,sint32 w,sint32 h)
      {
         register float pickx,picky,pickw,pickh;
         register sint32 mh=maxhits;
         hits=0;
         nhits=0;
         if(makeCurrent())
         {
            // Where to pick
            pickx=(m_view_port.w-2.0f*x-w)/((float)w);
            picky=(2.0f*y+h-m_view_port.h)/((float)h);
            pickw=m_view_port.w/((float)w);
            pickh=m_view_port.h/((float)h);

            // Set pick projection matrix
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glTranslatef(pickx,picky,0.0f);
            glScalef(pickw,pickh,1.0f);
            switch(projection)
            {
            case PARALLEL:
               glOrtho(m_view_port.left,m_view_port.right,m_view_port.bottom,m_view_port.top,m_view_port.hither,m_view_port.yon);
               break;
            case PERSPECTIVE:
               glFrustum(m_view_port.left,m_view_port.right,m_view_port.bottom,m_view_port.top,m_view_port.hither,m_view_port.yon);
               break;
            }

            // Model matrix
            glMatrixMode(GL_MODELVIEW);
            glLoadMatrixf(m_transformation);

            // Loop until room enough to fit
            do
            {
               nhits=0;
               hits = new uint32[mh];
               if(!hits) break;
               glSelectBuffer(mh, hits);
               glRenderMode(GL_SELECT);
               glInitNames();
               glPushName(0);
               if(scene) scene->DrawForHit(*this);
               glPopName();
               nhits=glRenderMode(GL_RENDER);
               mh<<=1;
            }
            while(nhits<0);
            makeNonCurrent();
            if(!nhits) 
            {
               delete []hits;
               hits= 0;
            }
            return nhits;
         }
         return 0;
      }


      // Process picks
      ObjectPtr Viewer::processHits(const uint32* pickbuffer,sint32 nhits)
      {
         uint32 d1,d2,i,n,zmin,zmax,sel=0;
         if(0<=nhits)
         {
            for(i=0,zmin=zmax=4294967295U; nhits>0; i+=n+3,nhits--)
            {
               n=pickbuffer[i];
               d1=pickbuffer[1+i];
               d2=pickbuffer[2+i];
               if(d1<zmin || (d1==zmin && d2<=zmax))
               {
                  zmin=d1;
                  zmax=d2;
                  sel=i;
               }
            }
            return scene->GetHitObject(&pickbuffer[4+sel]);
         }
         return ObjectPtr();
      }


      // Build 0-terminated list of ALL picked objects overlapping rectangle
      bool Viewer::select(sint32 x,sint32 y,sint32 w,sint32 h, std::vector<ObjectPtr>& selected_objects)
      {
         sint32 nhits;
         uint32 *hits = 0;
         if(scene && maxhits)
         {
            if(selectHits(hits,nhits,x,y,w,h))
            {
               for(uint32 i=0; nhits>0; i+=hits[i]+3,nhits--)
               {
                  ObjectPtr obj = scene->GetHitObject(&hits[4+i]);
                  if(obj)
                  {
                     selected_objects.push_back(obj);
                  }
               }
            }
            delete []hits;
         }
         return selected_objects.size() > 0;
      }


      // Lasso objects
      bool Viewer::lasso(sint32 x1,sint32 y1,sint32 x2,sint32 y2, std::vector<ObjectPtr>& selected_objects)
      {
         sint32 xlo,xhi,ylo,yhi;
         FXMINMAX(xlo,xhi,x1,x2);
         FXMINMAX(ylo,yhi,y1,y2);
         return select(xlo,ylo,xhi-xlo+1,yhi-ylo+1, selected_objects);
      }


      // Pick ONE object at x,y
      ObjectPtr Viewer::pick(sint32 x,sint32 y)
      {
         ObjectPtr obj;
         uint32 *hits = 0;
         sint32 nhits;
         if(scene && maxhits)
         {
            if(selectHits(hits,nhits,x-PICK_TOL,y-PICK_TOL,PICK_TOL*2,PICK_TOL*2))
            {
               obj=processHits(hits,nhits);
            }
            delete []hits;
         }
         return obj;
      }


      // Repaint the GL window
      long Viewer::onPaint(FXObject*,FXSelector,void*)
      {
         Redraw();
         return 1;
      }


      // Handle configure notify
      void Viewer::layout(){
         m_view_port.w=width;
         m_view_port.h=height;
         updateProjection();
         flags&=~FLAG_DIRTY;
      }


      // Start motion timer while in this window
      long Viewer::onEnter(FXObject* sender,FXSelector sel,void* ptr){
         FXGLCanvas::onEnter(sender,sel,ptr);
         if(isEnabled()){
            getApp()->addTimeout(this,ID_TIPTIMER,getApp()->getMenuPause());
         }
         return 1;
      }


      // Stop motion timer when leaving window
      long Viewer::onLeave(FXObject* sender,FXSelector sel,void* ptr){
         FXGLCanvas::onLeave(sender,sel,ptr);
         if(isEnabled()){
            getApp()->removeTimeout(this,ID_TIPTIMER);
         }
         return 1;
      }

      class ForeachHandle
      {
      public:
         ForeachHandle(FXObject* sender, FXSelector sel, void* ptr)
            :m_sender(sender)
            ,m_sel(sel)
            ,m_ptr(ptr)
            ,m_handled(false)
         {
         }

         void operator()(ObjectPtr obj)
         {
            m_handled = obj->handle(m_sender, m_sel, m_ptr) > 0 || m_handled;
         }

         bool WasHandled() const {return m_handled;}
      private:
         FXObject* m_sender;
         FXSelector m_sel;
         void* m_ptr;
         bool m_handled;
      };

      // Gained focus
      long Viewer::onFocusIn(FXObject* sender,FXSelector sel,void* ptr)
      {
         FXGLCanvas::onFocusIn(sender,sel,ptr);
         if(m_selected_objects.size())
         {
            ForeachHandle fh(this, FXSEL(SEL_FOCUSIN,0), ptr);
            fh = std::for_each(m_selected_objects.begin(), m_selected_objects.end(), fh);
            if (fh.WasHandled()) update();
         }
         return 1;
      }


      // Lost focus
      long Viewer::onFocusOut(FXObject* sender,FXSelector sel,void* ptr)
      {
         FXGLCanvas::onFocusOut(sender,sel,ptr);
         if(m_selected_objects.size())
         {
            ForeachHandle fh(this, FXSEL(SEL_FOCUSOUT,0), ptr);
            fh = std::for_each(m_selected_objects.begin(), m_selected_objects.end(), fh);
            if (fh.WasHandled()) update();
         }
         return 1;
      }


      // Change scene
      void Viewer::setScene(ObjectGroupPtr sc){
         scene=sc;
         update();
      }


      // Change field of view
      void Viewer::setFieldOfView(double fv){
         double tn;
         fov=FXCLAMP(2.0,fv,90.0);
         tn=tan(0.5*DTOR*fov);
         FXASSERT(tn>0.0);
         distance=diameter/tn;
         FXASSERT(distance>0.0);
         updateProjection();
         updateTransform();
         update();
      }


      // Change eye distance
      void Viewer::setDistance(double d){
         if(d<diameter) d=diameter;
         if(d>114.0*diameter) d=114.0*diameter;
         if(d!=distance){
            distance=d;
            FXASSERT(distance>0.0);
            fov=2.0*RTOD*atan2(diameter,distance);
            updateProjection();
            updateTransform();
            update();
         }
      }


      // Change zoom factor
      void Viewer::setZoom(double zm){
         if(zm<1.0E-30) zm=1.0E-30;
         if(zm!=zoom){
            zoom=zm;
            updateProjection();
            update();
         }
      }


      // Change scale factors
      void Viewer::setScale(Vertex3D s)
      {
         if(s[0]<0.000001f) s[0]=0.000001f;
         if(s[1]<0.000001f) s[1]=0.000001f;
         if(s[2]<0.000001f) s[2]=0.000001f;
         if(scale!=s){
            scale=s;
            updateTransform();
            update();
         }
      }


      // Change orientation to new quaternion
      void Viewer::setOrientation(FXQuatf rot){
         if(rot!=rotation){
            rotation=rot;
            rotation.adjust();
            updateTransform();
            update();
         }
      }


      // Change world projection
      void Viewer::updateProjection(){
         double hither_fac,r,aspect;

         // Should be non-0 size viewport
         if(m_view_port.w>0 && m_view_port.h>0){

            // Aspect ratio of viewer
            aspect = (double)m_view_port.h / (double)m_view_port.w;

            // Get world box
            r=0.5*diameter/zoom;
            if(m_view_port.w<=m_view_port.h){
               m_view_port.left=-r;
               m_view_port.right=r;
               m_view_port.bottom=-r*aspect;
               m_view_port.top=r*aspect;
            }
            else{
               m_view_port.left=-r/aspect;
               m_view_port.right=r/aspect;
               m_view_port.bottom=-r;
               m_view_port.top=r;
            }
            FXASSERT(distance>0.0);
            FXASSERT(diameter>0.0);

            // A more intelligent method for setting the
            // clip planes might be interesting...
            m_view_port.yon=distance+diameter;
            m_view_port.hither=0.1*m_view_port.yon;
            //wvt.hither=distance-diameter;
            //  if(wvt.hither<distance-diameter) wvt.hither=distance-diameter;

            // New window
            FXTRACE((100,"wvt.left=%g wvt.right=%g wvt.top=%g wvt.bottom=%g wvt.hither=%g wvt.yon=%g\n",m_view_port.left,m_view_port.right,m_view_port.top,m_view_port.bottom,m_view_port.hither,m_view_port.yon));

            // Size of a pixel in world and model
            worldpx=(m_view_port.right-m_view_port.left)/m_view_port.w;
            modelpx=worldpx*diameter;

            // Precalc stuff for view->world backmapping
            ax=m_view_port.left;
            ay=m_view_port.top-worldpx;

            // Report factors
            FXTRACE((100,"worldpx=%g modelpx=%g\n",worldpx,modelpx));

            // Correction for perspective
            if(projection==PERSPECTIVE){
               FXASSERT(distance>0.0);
               hither_fac=m_view_port.hither/distance;
               m_view_port.left*=hither_fac;
               m_view_port.right*=hither_fac;
               m_view_port.top*=hither_fac;
               m_view_port.bottom*=hither_fac;
            }
         }
      }


      // Change transformation matrix
      void Viewer::updateTransform(){
         m_transformation.eye();
         m_transformation.trans(0.0f,0.0f,(float)-distance);
         m_transformation.rot(rotation);
         m_transformation.scale(scale[0], scale[1], scale[2]);
         m_transformation.trans(-center[0], -center[1], -center[2]);
         itransform=m_transformation.invert();
         //   FXTRACE((150,"itrans=%11.8f %11.8f %11.8f %11.8f\n",itransform[0][0],itransform[0][1],itransform[0][2],itransform[0][3]));
         //   FXTRACE((150,"       %11.8f %11.8f %11.8f %11.8f\n",itransform[1][0],itransform[1][1],itransform[1][2],itransform[1][3]));
         //   FXTRACE((150,"       %11.8f %11.8f %11.8f %11.8f\n",itransform[2][0],itransform[2][1],itransform[2][2],itransform[2][3]));
         //   FXTRACE((150,"       %11.8f %11.8f %11.8f %11.8f\n",itransform[3][0],itransform[3][1],itransform[3][2],itransform[3][3]));
         //   FXTRACE((150,"\n"));
         //   FXTRACE((150," trans=%11.8f %11.8f %11.8f %11.8f\n",transform[0][0],transform[0][1],transform[0][2],transform[0][3]));
         //   FXTRACE((150,"       %11.8f %11.8f %11.8f %11.8f\n",transform[1][0],transform[1][1],transform[1][2],transform[1][3]));
         //   FXTRACE((150,"       %11.8f %11.8f %11.8f %11.8f\n",transform[2][0],transform[2][1],transform[2][2],transform[2][3]));
         //   FXTRACE((150,"       %11.8f %11.8f %11.8f %11.8f\n",transform[3][0],transform[3][1],transform[3][2],transform[3][3]));
         //   FXTRACE((150,"\n"));
         //   FXHMat check=itransform*transform;
         //   FXTRACE((150," check=%11.8f %11.8f %11.8f %11.8f\n",check[0][0],check[0][1],check[0][2],check[0][3]));
         //   FXTRACE((150,"       %11.8f %11.8f %11.8f %11.8f\n",check[1][0],check[1][1],check[1][2],check[1][3]));
         //   FXTRACE((150,"       %11.8f %11.8f %11.8f %11.8f\n",check[2][0],check[2][1],check[2][2],check[2][3]));
         //   FXTRACE((150,"       %11.8f %11.8f %11.8f %11.8f\n",check[3][0],check[3][1],check[3][2],check[3][3]));
         //   FXTRACE((150,"\n"));
      }


      // Set model bounding box
      bool Viewer::setBounds(const BoundingBox3D& box)
      {
         BoundingBox3D r(box);

         // Model center
         center=r.GetCenter();

         // Model size
         diameter=r.GetLongest();

         // Fix zero size model
         if(diameter<1.0E-30) diameter=1.0;

         // Set equal scaling initially
         scale=Vertex3D(1.0f,1.0f,1.0f);

         // Reset distance (and thus field of view)
         setDistance(1.1*diameter);

         return true;
      }

      static inline FX::FXVec3f Vertex2FXVec3f(const Vertex3D& vertex)
      {
         return FX::FXVec3f(vertex);
      }
      static inline Vertex3D FXVec3f2Vertex(const FXVec3f& vec3)
      {
         return Vertex3D(vec3[0], vec3[1], vec3[2]);
      }

      // Fit view to new bounds
      bool Viewer::fitToBounds(const BoundingBox3D& box_in)
      {
         FX::FXRangef r(std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), 
                        std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), 
                        std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
         FX::FXRangef box(Vertex2FXVec3f(box_in.GetMin()), Vertex2FXVec3f(box_in.GetMax()));
         FX::FXMat4f m;

         // Get rotation of model
         m.eye();
         m.rot(rotation);
         m.trans(-box.center());

         // Transform box
         for(int i=0; i<8; i++)
         {
            r.include(box.corner(i)*m);
         }

         BoundingBox3D rb(FXVec3f2Vertex(r[0]), FXVec3f2Vertex(r[1]));
         setBounds(rb);

         return true;
      }


      // Obtain viewport
      void Viewer::getViewport(Viewport& v) const 
      {
         v=m_view_port;
      }


      // Set material
      void Viewer::setMaterial(const Material &mtl){
         material=mtl;
         update();
      }


      // Get material
      void Viewer::getMaterial(Material &mtl) const {
         mtl=material;
      }


      // Get screen point from eye coordinate
      void Viewer::eyeToScreen(sint32& sx,sint32& sy,Vertex3D e){
         register double xp,yp;
         if(projection==PERSPECTIVE){
            if(e[2]==0.0f){ fxerror("%s::eyeToScreen: cannot transform point.\n",getClassName()); }
            xp=-distance*e[0]/e[2];
            yp=-distance*e[1]/e[2];
         }
         else{
            xp=e[0];
            yp=e[1];
         }
         sx=(int)((xp-ax)/worldpx);
         sy=(int)((ay-yp)/worldpx);
      }


      // Convert screen point to eye coords
      Vertex3D Viewer::screenToEye(sint32 sx,sint32 sy,float eyez){
         register float xp,yp;
         Vertex3D e;
         xp=(float)(worldpx*sx+ax);
         yp=(float)(ay-worldpx*sy);
         if(projection==PERSPECTIVE){
            FXASSERT(distance>0.0);
            e[0]=(float)(-eyez*xp/distance);
            e[1]=(float)(-eyez*yp/distance);
            e[2]=eyez;
         }
         else{
            e[0]=xp;
            e[1]=yp;
            e[2]=eyez;
         }
         return e;
      }


      // Convert screen to eye, on projection plane
      Vertex3D Viewer::screenToTarget(sint32 sx,sint32 sy)
      {
         return Vertex3D((float)(worldpx*sx+ax), (float)(ay-worldpx*sy), (float)-distance);
      }


      // Convert world to eye coords
      Vertex3D Viewer::worldToEye(Vertex3D w)
      {
         return FXVec3f2Vertex(Vertex2FXVec3f(w)*m_transformation);
      }


      // Get eye Z-coordinate of world point
      float Viewer::worldToEyeZ(Vertex3D w)
      {
         return w[0]*m_transformation[0][2]+w[1]*m_transformation[1][2]+w[2]*m_transformation[2][2]+m_transformation[3][2];
      }


      // Convert eye to world coords
      Vertex3D Viewer::eyeToWorld(Vertex3D e)
      {
         return FXVec3f2Vertex(Vertex2FXVec3f(e)*itransform);
      }


      // Get world vector
      Vertex3D Viewer::worldVector(sint32 fx,sint32 fy,sint32 tx,sint32 ty)
      {
         FX::FXVec3f wfm,wto,vec;
         wfm=screenToTarget(fx,fy);
         wto=screenToTarget(tx,ty);
         vec=wto*itransform-wfm*itransform;
         return FXVec3f2Vertex(vec);
      }


      // Get a bore vector
      bool Viewer::getBoreVector(sint32 sx,sint32 sy,Vertex3D& point,Vertex3D& dir)
      {
         Vertex3D p=eyeToWorld(screenToEye(sx,sy,(float)(diameter-distance)));
         if(PARALLEL==projection)
            point=eyeToWorld(screenToEye(sx,sy,0.0f));
         else
            point=eyeToWorld(Vertex3D(0.0f,0.0f,0.0f));
         dir=math::Normalize(p-point);
         return true;
      }


      // Get eye viewing direction (non-normalized)
      Vertex3D Viewer::getEyeVector() const {
         return Vertex3D(-itransform[2][0],-itransform[2][1],-itransform[2][2]);
      }


      // Get eye position
      Vertex3D Viewer::getEyePosition() const{
         return Vertex3D(itransform[3][0],itransform[3][1],itransform[3][2]);
      }


      // Change model center
      void Viewer::setCenter(Vertex3D cntr){
         if(center!=cntr){
            center=cntr;
            updateTransform();
            update();
         }
      }


      // Translate in world
      void Viewer::translate(Vertex3D vec){
         center+=vec;
         updateTransform();
         update();
      }


      // Change m_selected_objects
      void Viewer::SetSelection(const ObjectPtrVector& sel)
      {
         if(m_selected_objects!=sel)
         {
            for (ObjectPtrVector::iterator it=m_selected_objects.begin(); it!=m_selected_objects.end(); ++it)
            {
               (*it)->SetSelected(false);
            }
            m_selected_objects=sel;
            for (ObjectPtrVector::iterator it=m_selected_objects.begin(); it!=m_selected_objects.end(); ++it)
            {
               (*it)->SetSelected(true);
            }
            update();
         }
      }

      void Viewer::SetSelection(const std::vector<std::string>& sel)
      {
         ObjectPtrVector selected_objects;
         for (std::vector<std::string>::const_iterator it=sel.begin(); it!=sel.end(); ++it)
         {
            ObjectPtr object = scene->FindObject(*it);
            if (object)
            {
               selected_objects.push_back(object);
            }
         }

         SetSelection(selected_objects);
      }

      // Change help text
      void Viewer::setHelpText(const FXString& text){
         help=text;
      }


      // Change tip text
      void Viewer::setTipText(const FXString& text){
         tip=text;
      }


      // Translate point into unit-sphere coordinate
      Vertex3D Viewer::spherePoint(sint32 px,sint32 py)
      {
         float d,t,screenmin;
         Vertex3D v;
         if(m_view_port.w>m_view_port.h)
            screenmin=(float)m_view_port.h;
         else
            screenmin=(float)m_view_port.w;
         v[0]=2.0f*(px-0.5f*m_view_port.w)/screenmin;
         v[1]=2.0f*(0.5f*m_view_port.h-py)/screenmin;
         d=v[0]*v[0]+v[1]*v[1];
         if(d<0.75){
            v[2]=sqrtf(1.0-d);
         }
         else if(d<3.0f){
            d=1.7320508008f-sqrtf(d);
            t=1.0f-d*d;
            if(t<0.0f) t=0.0f;
            v[2]=1.0f-sqrtf(t);
         }
         else{
            v[2]=0.0f;
         }
         return math::Normalize(v);
      }


      // Turn camera; simpler now that arc() is changed
      FXQuatf Viewer::turn(sint32 fx,sint32 fy,sint32 tx,sint32 ty)
      {
         return FXQuatf(Vertex2FXVec3f(spherePoint(fx,fy)),
                        Vertex2FXVec3f(spherePoint(tx,ty)));
      }


      // Draw non-destructive lasso box; drawing twice will erase it again
      void Viewer::drawLasso(sint32 x0,sint32 y0,sint32 x1,sint32 y1){

         FXGLVisual *vis=(FXGLVisual*)getVisual();

         // With open_gl, first change back to 1:1 projection mode
         if(makeCurrent()){

            // Save state
            glPushAttrib(GL_COLOR_BUFFER_BIT|GL_ENABLE_BIT|GL_DEPTH_BUFFER_BIT|GL_LINE_BIT);
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();

            // Fix xform
            glLoadIdentity();
            glOrtho(0.0,width-1.0,0.0,height-1.0,0.0,1.0);

            // Draw in front buffer, so we see it w/o blowing
            // away the drawing by calling swapBuffers.
            if(vis->isDoubleBuffer()) glDrawBuffer(GL_FRONT);

            // Fancy stuff off
            glLineWidth(1.0);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_COLOR_MATERIAL);
            glDisable(GL_LIGHTING);
            glShadeModel(GL_FLAT);
            glDepthMask(false);
            glDisable(GL_DITHER);

            // Windows
#ifdef WIN32

#ifndef _ALPHA_
            // MS-Windows has logic ops, and they seem to work:- at least
            // with the unaccelerated software rendering they seem to...
            glEnable(GL_COLOR_LOGIC_OP);
            glLogicOp(GL_INVERT);

#else
            // ALPHA CPU's don't have logic ops; or at least they're broken :-(
            glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ZERO);
            glEnable(GL_BLEND);

#endif

            // UNIX
#else
#if !defined(GL_VERSION_1_1) || !defined(GL_VERSION_1_2)

            // If you don't have open_gl 1.1 or better, blending
            // to invert the lasso is your only choice...
            glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ZERO);
            glEnable(GL_BLEND);

#else

            // You have open_gl 1.1 or better, but chances are it
            // still doesn't work, because you may have an incomplete
            // implementation [DEC], or perhaps broken hardware.

            // If it works for you, uncomment the lines below,
            // and comment the ones above...
            glEnable(GL_COLOR_LOGIC_OP);
            glLogicOp(GL_INVERT);
#endif
#endif

            glBegin(GL_LINE_LOOP);
            glColor4ub(255,255,255,255);
            glVertex2i(x0,m_view_port.h-y0-1);
            glVertex2i(x0,m_view_port.h-y1-1);
            glVertex2i(x1,m_view_port.h-y1-1);
            glVertex2i(x1,m_view_port.h-y0-1);
            glEnd();
            glFinish();         // Moved back here because of driver issues

            // Restore to old state
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
            glPopAttrib();
            makeNonCurrent();
         }
      }

      /*************************  Mouse Actions in Viewer  ***************************/


      // Current object changed
      long Viewer::onChanged(FXObject*,FXSelector,void* ptr)
      {
         SetSelection(*(ObjectPtrVector*)ptr);
         if(target) target->tryHandle(this,FXSEL(SEL_CHANGED,message),ptr);
         return 1;
      }


      // Message that indicates where user picked
      long Viewer::onPick(FXObject*,FXSelector,void* ptr)
      {
         return target && target->tryHandle(this,FXSEL(SEL_PICKED,message),ptr);
      }


      // Clicked in widget
      long Viewer::onClicked(FXObject*,FXSelector,void* ptr)
      {
         if(target){
            if(target->tryHandle(this,FXSEL(SEL_CLICKED,message),ptr)) return 1;
            if(ptr && target->tryHandle(this,FXSEL(SEL_COMMAND,message),ptr)) return 1;
         }
         return 1;
      }


      // Double clicked in widget; ptr may or may not point to an object
      long Viewer::onDoubleClicked(FXObject*,FXSelector,void* ptr){
         return target && target->tryHandle(this,FXSEL(SEL_DOUBLECLICKED,message),ptr);
      }


      // Triple clicked in widget; ptr may or may not point to an object
      long Viewer::onTripleClicked(FXObject*,FXSelector,void* ptr){
         return target && target->tryHandle(this,FXSEL(SEL_TRIPLECLICKED,message),ptr);
      }


      // Lassoed object(s)
      long Viewer::onLassoed(FXObject*,FXSelector,void* ptr)
      {
         FXEvent* event=(FXEvent*)ptr;

         // Notify target of lasso
         if(target && target->tryHandle(this,FXSEL(SEL_LASSOED,message),ptr)) return 1;

         // Grab all objects in lasso
         std::vector<ObjectPtr> selected_objects;
         if (lasso(event->click_x,event->click_y,event->win_x,event->win_y, selected_objects))
         {
            // Add m_selected_objects mode
            if(event->state&SHIFTMASK)
            {
               handle(this,FXSEL(SEL_SELECTED,0),&selected_objects);
            }

            // Toggle m_selected_objects mode
            else if(event->state&CONTROLMASK)
            {
               handle(this,FXSEL(SEL_DESELECTED,0),&selected_objects);
            }
         }

         return 1;
      }


      // Selected object(s)
      long Viewer::onSelected(FXObject*,FXSelector,void* ptr)
      {
         return target && target->tryHandle(this,FXSEL(SEL_SELECTED,message),ptr);
      }


      // Deselected object(s)
      long Viewer::onDeselected(FXObject*,FXSelector,void* ptr)
      {
         return target && target->tryHandle(this,FXSEL(SEL_DESELECTED,message),ptr);
      }


      // Inserted object(s)
      long Viewer::onInserted(FXObject*,FXSelector,void* ptr){
         return target && target->tryHandle(this,FXSEL(SEL_INSERTED,message),ptr);
      }


      // Deleted object(s)
      long Viewer::onDeleted(FXObject*,FXSelector,void* ptr)
      {
         return target && target->tryHandle(this,FXSEL(SEL_DELETED,message),ptr);
      }


      // Change operation
      void Viewer::setOp(uint32 o){
         if(mode!=o){
            switch(o){
      case HOVERING:
         setDragCursor(getDefaultCursor());
         FXTRACE((100,"HOVERING\n"));
         if(doesturbo) update();
         doesturbo=false;
         break;
      case PICKING:
         FXTRACE((100,"PICKING\n"));
         setDragCursor(getDefaultCursor());
         break;
      case ROTATING:
         FXTRACE((100,"ROTATING\n"));
         doesturbo=turbomode;
         setDragCursor(getApp()->getDefaultCursor(DEF_ROTATE_CURSOR));
         break;
      case POSTING:
         FXTRACE((100,"POSTING\n"));
         setDragCursor(getDefaultCursor());
         break;
      case TRANSLATING:
         FXTRACE((100,"TRANSLATING\n"));
         doesturbo=turbomode;
         setDragCursor(getApp()->getDefaultCursor(DEF_MOVE_CURSOR));
         break;
      case ZOOMING:
         FXTRACE((100,"ZOOMING\n"));
         doesturbo=turbomode;
         setDragCursor(getApp()->getDefaultCursor(DEF_DRAGH_CURSOR));
         break;
      case FOVING:
         FXTRACE((100,"FOVING\n"));
         doesturbo=turbomode;
         setDragCursor(getApp()->getDefaultCursor(DEF_DRAGH_CURSOR));
         break;
      case DRAGGING:
         FXTRACE((100,"DRAGGING\n"));
         doesturbo=turbomode;
         setDragCursor(getApp()->getDefaultCursor(DEF_MOVE_CURSOR));
         break;
      case TRUCKING:
         FXTRACE((100,"TRUCKING\n"));
         doesturbo=turbomode;
         setDragCursor(getApp()->getDefaultCursor(DEF_DRAGH_CURSOR));
         break;
      case GYRATING:
         FXTRACE((100,"GYRATING\n"));
         doesturbo=turbomode;
         setDragCursor(getApp()->getDefaultCursor(DEF_ROTATE_CURSOR));
         break;
      case DO_LASSOSELECT:
         if(mode==LASSOSELECT) return;
         FXTRACE((100,"LASSOSELECT\n"));
         setDefaultCursor(getApp()->getDefaultCursor(DEF_CORNERNW_CURSOR));
         /// FIXME grab
         break;
      case LASSOSELECT:
         FXTRACE((100,"LASSOSELECT\n"));
         setDefaultCursor(getDragCursor());
         setDragCursor(getApp()->getDefaultCursor(DEF_CORNERNW_CURSOR));
         break;
      case DO_LASSOZOOM:
         if(mode==LASSOZOOM) return;
         FXTRACE((100,"LASSOZOOM\n"));
         setDefaultCursor(getApp()->getDefaultCursor(DEF_CORNERNW_CURSOR));
         /// FIXME grab
         break;
      case LASSOZOOM:
         FXTRACE((100,"LASSOZOOM\n"));
         setDefaultCursor(getDragCursor());
         setDragCursor(getApp()->getDefaultCursor(DEF_CORNERNW_CURSOR));
         break;
            }
            mode=o;
         }
      }


      // Perform the usual mouse manipulation
      long Viewer::onLeftBtnPress(FXObject*,FXSelector,void* ptr){
         FXEvent* event=(FXEvent*)ptr;
         flags&=~FLAG_TIP;
         FXTRACE((100,"onLeftBtnPress Mask=%08x\n",event->state));
         handle(this,FXSEL(SEL_FOCUS_SELF,0),ptr);
         if(isEnabled()){
            grab();
            flags&=~FLAG_UPDATE;
            if(target && target->tryHandle(this,FXSEL(SEL_LEFTBUTTONPRESS,message),ptr)) return 1;
            if(event->state&RIGHTBUTTONMASK){
               if(event->state&SHIFTMASK)
                  setOp(TRUCKING);
               else
                  setOp(ZOOMING);
            }
            else if(event->state&MIDDLEBUTTONMASK){
               setOp(ROTATING);
            }
            else if(mode==DO_LASSOZOOM){
               if(0<=event->click_x && 0<=event->click_y && event->click_x<width && event->click_y<height){
                  drawLasso(event->click_x,event->click_y,event->win_x,event->win_y);
                  setOp(LASSOZOOM);
               }
               else{
                  getApp()->beep();
               }
            }
            else if(mode==DO_LASSOSELECT){
               if(0<=event->click_x && 0<=event->click_y && event->click_x<width && event->click_y<height){
                  drawLasso(event->click_x,event->click_y,event->win_x,event->win_y);
                  setOp(LASSOSELECT);
               }
               else{
                  getApp()->beep();
               }
            }
            else if(event->state&(SHIFTMASK|CONTROLMASK)){
               setOp(PICKING);
            }
            else if(m_selected_objects.size())
            {
               bool dragable = true;
               for (ObjectPtrVector::iterator it=m_selected_objects.begin(); it!=m_selected_objects.end(); ++it)
               {
                  dragable &= (*it)->IsDragAble();
               }
               if (dragable)
               {
                  ObjectPtr obj = pick(event->click_x,event->click_y);
                  if (std::find(m_selected_objects.begin(), m_selected_objects.end(), obj) != m_selected_objects.end())
                  {
                     setOp(DRAGGING);
                  }
                  else
                  {
                     setOp(PICKING);
                  }
               }
               else
               {
                  setOp(PICKING);
               }
            }
            else
            {
               setOp(PICKING);
            }
         }
         return 1;
      }


      // Left mouse button released
      long Viewer::onLeftBtnRelease(FXObject*,FXSelector,void* ptr){
         FXEvent* event=(FXEvent*)ptr;
         sint32 new_x,new_y,cx,cy,xl,xh,yl,yh;
         Vertex3D vec;
         FXTRACE((100,"onLeftBtnRelease Mask=%08x\n",event->state));
         if(isEnabled()){
            ungrab();
            flags|=FLAG_UPDATE;
            if(target && target->tryHandle(this,FXSEL(SEL_LEFTBUTTONRELEASE,message),ptr)) return 1;
            if(event->state&RIGHTBUTTONMASK){
               if(event->state&SHIFTMASK){
                  setOp(GYRATING);
               }
               else if(event->state&CONTROLMASK){
                  setOp(FOVING);
               }
               else{
                  setOp(TRANSLATING);
               }
               grab();
            }
            else if(event->state&MIDDLEBUTTONMASK){
               if(event->state&SHIFTMASK){
                  setOp(TRUCKING);
               }
               else{
                  setOp(ZOOMING);
               }
               grab();
            }
            else if(mode==LASSOZOOM){
               new_x=FXCLAMP(0,event->win_x,(width-1));
               new_y=FXCLAMP(0,event->win_y,(height-1));
               drawLasso(event->click_x,event->click_y,new_x,new_y);
               xl=FXMIN(new_x,event->click_x);
               xh=FXMAX(new_x,event->click_x);
               yl=FXMIN(new_y,event->click_y);
               yh=FXMAX(new_y,event->click_y);
               if(xh>xl && yh>yl){
                  cx=(getWidth()-(xl+xh))/2;
                  cy=(getHeight()-(yl+yh))/2;
                  vec=worldVector(0,0,cx,cy);
                  translate(-vec);
                  setZoom(zoom*getWidth()/(xh-xl));
               }
               setOp(HOVERING);
            }
            else if(mode==LASSOSELECT){           // FIXME interpret control and shift for m_selected_objects changes
               new_x=FXCLAMP(0,event->win_x,(width-1));
               new_y=FXCLAMP(0,event->win_y,(height-1));
               drawLasso(event->click_x,event->click_y,new_x,new_y);
               handle(this,FXSEL(SEL_LASSOED,0),ptr);
               setOp(HOVERING);
            }
            else if(mode==PICKING)
            {               // FIXME interpret control and shift for m_selected_objects changes
               setOp(HOVERING);
               if(!handle(this,FXSEL(SEL_PICKED,0),ptr))
               {
                  ObjectPtrVector objects;
                  ObjectPtr obj = pick(event->click_x,event->click_y);
                  if (obj)
                  {
                     if (event->state&SHIFTMASK)
                     {
                        objects = m_selected_objects;
                        objects.push_back(obj);
                     }
                     else if (event->state&CONTROLMASK)
                     {
                        objects = m_selected_objects;
                        ObjectPtrVector::iterator it_obj = std::find(objects.begin(), objects.end(), obj);
                        if (it_obj != objects.end())
                        {
                           objects.erase(it_obj);
                        }
                        else
                        {
                           objects.push_back(obj);
                        }
                     }
                     else
                     {
                        objects.push_back(obj);
                     }
                  }
                  handle(this,FXSEL(SEL_CHANGED,0), &objects);
                  handle(this,FXSEL(SEL_SELECTED,0), &objects);
               }
            }
            else if(mode==DRAGGING)
            {
               if(target) target->tryHandle(this, FXSEL(SEL_DRAGGED,message), &m_selected_objects);
               setOp(HOVERING);
            }
            else{
               setOp(HOVERING);
            }
            if(event->click_count==1)
            {
               handle(this, FXSEL(SEL_CLICKED,0), &m_selected_objects);
            }
            else if(event->click_count==2)
            {
               handle(this, FXSEL(SEL_DOUBLECLICKED,0), &m_selected_objects);
            }
            else if(event->click_count==3)
            {
               handle(this, FXSEL(SEL_TRIPLECLICKED,0), &m_selected_objects);
            }
         }
         return 1;
      }


      // Pressed middle mouse button
      long Viewer::onMiddleBtnPress(FXObject*,FXSelector,void* ptr){
         FXEvent* event=(FXEvent*)ptr;
         flags&=~FLAG_TIP;
         FXTRACE((100,"onMiddleBtnPress Mask=%08x\n",event->state));
         handle(this,FXSEL(SEL_FOCUS_SELF,0),ptr);
         if(isEnabled()){
            grab();
            flags&=~FLAG_UPDATE;
            if(target && target->tryHandle(this,FXSEL(SEL_MIDDLEBUTTONPRESS,message),ptr)) return 1;
            if(event->state&SHIFTMASK){
               setOp(TRUCKING);
            }
            else{
               setOp(ZOOMING);
            }
         }
         return 1;
      }


      // Released middle mouse button
      long Viewer::onMiddleBtnRelease(FXObject*,FXSelector,void* ptr){
         FXEvent* event=(FXEvent*)ptr;
         FXTRACE((100,"onMiddleBtnRelease Mask=%08x\n",event->state));
         if(isEnabled()){
            ungrab();
            flags|=FLAG_UPDATE;
            if(target && target->tryHandle(this,FXSEL(SEL_MIDDLEBUTTONRELEASE,message),ptr)) return 1;
            if(event->state&LEFTBUTTONMASK){
               setOp(ROTATING);
               grab();
            }
            else if(event->state&RIGHTBUTTONMASK){
               if(event->state&SHIFTMASK){
                  setOp(GYRATING);
               }
               else if(event->state&CONTROLMASK){
                  setOp(FOVING);
               }
               else{
                  setOp(TRANSLATING);
               }
               grab();
            }
            else{
               setOp(HOVERING);
            }
         }
         return 1;
      }


      // Pressed right button
      long Viewer::onRightBtnPress(FXObject*,FXSelector,void* ptr){
         FXEvent* event=(FXEvent*)ptr;
         flags&=~FLAG_TIP;
         FXTRACE((100,"onRightBtnPress Mask=%08x\n",event->state));
         handle(this,FXSEL(SEL_FOCUS_SELF,0),ptr);
         if(isEnabled()){
            grab();
            flags&=~FLAG_UPDATE;
            if(target && target->tryHandle(this,FXSEL(SEL_RIGHTBUTTONPRESS,message),ptr)) return 1;
            if(event->state&LEFTBUTTONMASK){
               if(event->state&SHIFTMASK){
                  setOp(TRUCKING);
               }
               else{
                  setOp(ZOOMING);
               }
            }
            else if(event->state&MIDDLEBUTTONMASK){
               if(event->state&SHIFTMASK){
                  setOp(GYRATING);
               }
               else if(event->state&CONTROLMASK){
                  setOp(FOVING);
               }
               else{
                  setOp(TRANSLATING);
               }
            }
            else{
               if(event->state&SHIFTMASK){
                  setOp(GYRATING);
               }
               else if(event->state&CONTROLMASK){
                  setOp(FOVING);
               }
               else{
                  setOp(POSTING);
               }
            }
         }
         return 1;
      }


      // Released right button
      long Viewer::onRightBtnRelease(FXObject*,FXSelector,void* ptr){
         FXEvent* event=(FXEvent*)ptr;
         ObjectPtr hit;
         FXTRACE((100,"onRightBtnRelease Mask=%08x\n",event->state));
         if(isEnabled()){
            ungrab();
            flags|=FLAG_UPDATE;
            if(target && target->tryHandle(this,FXSEL(SEL_RIGHTBUTTONRELEASE,message),ptr)) return 1;
            if(event->state&LEFTBUTTONMASK){
               setOp(ROTATING);
               grab();
            }
            else if(event->state&MIDDLEBUTTONMASK){
               if(event->state&SHIFTMASK){
                  setOp(TRUCKING);
               }
               else{
                  setOp(ZOOMING);
               }
               grab();
            }
            else{
               if(mode==POSTING)
               {
                  setOp(HOVERING);
                  hit=pick(event->click_x,event->click_y);
                  if(hit && hit->handle(this,FXSEL(SEL_COMMAND,ID_QUERY_MENU),ptr))
                     ;
                  else if(target && target->tryHandle(this,FXSEL(SEL_COMMAND,ID_QUERY_MENU),ptr))
                     ;
               }
               setOp(HOVERING);
            }
         }
         return 1;
      }


      // Mouse moved
      long Viewer::onMotion(FXObject*,FXSelector,void* ptr){
         FXEvent* event=(FXEvent*)ptr;
         sint32 new_x,new_y,old_x,old_y;
         long changed=(flags&FLAG_TIP)!=0;
         double delta;
         float tmp;
         Vertex3D vec;
         FXQuatf q;
         flags&=~FLAG_TIP;
         if(isEnabled()){
            if(target && target->tryHandle(this,FXSEL(SEL_MOTION,message),ptr)) return 1;
            getApp()->removeTimeout(this,ID_TIPTIMER);
            switch(mode){
      case HOVERING:            // Reset the timer each time we moved the cursor
         getApp()->addTimeout(this,ID_TIPTIMER,getApp()->getMenuPause());
         break;
      case PICKING:             // Picking
         if(!event->moved){                              // Keep picking mode for now
            break;
         }
         if(event->state&(SHIFTMASK|CONTROLMASK)){       // Lasso mode if modifier held down
            drawLasso(event->click_x,event->click_y,event->win_x,event->win_y);
            setOp(LASSOSELECT);
            break;
         }
         setOp(ROTATING);                                // Go into rotation mode
      case ROTATING:            // Rotating camera around target
         q=turn(event->last_x,event->last_y,event->win_x,event->win_y) * getOrientation();
         setOrientation(q);
         changed=1;
         break;
      case POSTING:             // Posting right-mouse menu; if moving more than delta, we go to translate mode
         if(!event->moved) break;
         setOp(TRANSLATING);
      case TRANSLATING:         // Translating camera
         vec=worldVector(event->last_x,event->last_y,event->win_x,event->win_y);
         translate(-vec);
         changed=1;
         break;
      case ZOOMING:             // Zooming camera
         delta=0.005*(event->win_y-event->last_y);
         setZoom(getZoom()*pow(2.0,delta));
         changed=1;
         break;
      case FOVING:              // Change FOV
         setFieldOfView(getFieldOfView()+90.0*(event->win_y-event->last_y)/(double)m_view_port.h);
         changed=1;
         break;
      case DRAGGING:            // Dragging a shape
         if(m_selected_objects.size())
         {
            for (ObjectPtrVector::iterator it=m_selected_objects.begin(); it!=m_selected_objects.end(); ++it)
            {
               (*it)->Drag(*this, event->last_x,event->last_y,event->win_x,event->win_y);
            }
            //// Perhaps callback here for the target to be notified of the new object position
            update();
         }
         changed=1;
         break;
      case TRUCKING:            // Trucking camera forward or backward
         tmp=(float)(worldpx*(event->win_y-event->last_y));
         vec=math::Normalize(getEyeVector());
         translate(tmp*vec);
         changed=1;
         break;
      case GYRATING:            // Rotating camera around eye
         {
            FXMat4f mm;
            FXQuatf qq;
            qq=turn(event->win_x,event->win_y,event->last_x,event->last_y);
            mm.eye();
            mm.trans(0.0f,0.0f,(float)-distance); // FIXME This aint it yet...
            mm.rot(qq);
            mm.trans(0.0f,0.0f,(float)distance);
            center=FXVec3f2Vertex(Vertex2FXVec3f(center)*mm);
            q=qq * getOrientation();
            setOrientation(q);
            update();
            changed=1;
         }
         break;
      case LASSOSELECT:         // Dragging a lasso
      case LASSOZOOM:
         old_x=FXCLAMP(0,event->last_x,(width-1));
         old_y=FXCLAMP(0,event->last_y,(height-1));
         new_x=FXCLAMP(0,event->win_x,(width-1));
         new_y=FXCLAMP(0,event->win_y,(height-1));
         drawLasso(event->click_x,event->click_y,old_x,old_y);
         drawLasso(event->click_x,event->click_y,new_x,new_y);
         if(new_x>event->click_x){
            if(new_y>event->click_y)
               setDragCursor(getApp()->getDefaultCursor(DEF_CORNERSE_CURSOR));
            else
               setDragCursor(getApp()->getDefaultCursor(DEF_CORNERNE_CURSOR));
         }
         else{
            if(new_y>event->click_y)
               setDragCursor(getApp()->getDefaultCursor(DEF_CORNERSW_CURSOR));
            else
               setDragCursor(getApp()->getDefaultCursor(DEF_CORNERNW_CURSOR));
         }
         changed=1;
         break;
            }
         }
         return changed;
      }


      // Mouse wheel
      long Viewer::onMouseWheel(FXObject*,FXSelector,void* ptr){
         FXEvent* event=(FXEvent*)ptr;
         if(isEnabled()){
            if(target && target->tryHandle(this,FXSEL(SEL_MOUSEWHEEL,message),ptr)) return 1;
            setZoom(getZoom()*pow(2.0,-0.1*event->code/120.0));
            return 1;
         }
         return 0;
      }


      // Handle keyboard press/release
      long Viewer::onKeyPress(FXObject*,FXSelector,void* ptr){
         FXEvent* event=(FXEvent*)ptr;
         flags&=~FLAG_TIP;
         if(isEnabled()){
            if(target && target->tryHandle(this,FXSEL(SEL_KEYPRESS,message),ptr)) return 1;
            switch(event->code){
      case KEY_Shift_L:
      case KEY_Shift_R:

         // We do not switch modes unless something was going on already
         if(mode!=HOVERING){
            if((event->state&MIDDLEBUTTONMASK) || ((event->state&LEFTBUTTONMASK) && (event->state&RIGHTBUTTONMASK))){
               setOp(TRUCKING);
            }
            else if(event->state&RIGHTBUTTONMASK){
               setOp(GYRATING);
            }
         }
         return 1;
      case KEY_Control_L:
      case KEY_Control_R:

         // We do not switch modes unless something was going on already
         if(mode!=HOVERING){
            if(event->state&RIGHTBUTTONMASK){
               setOp(FOVING);
            }
         }
         return 1;
            }
         }
         return 0;
      }


      // Key release
      long Viewer::onKeyRelease(FXObject*,FXSelector,void* ptr){
         FXEvent* event=(FXEvent*)ptr;
         if(isEnabled()){
            if(target && target->tryHandle(this,FXSEL(SEL_KEYRELEASE,message),ptr)) return 1;
            switch(event->code){
      case KEY_Shift_L:
      case KEY_Shift_R:

         // We do not switch modes unless something was going on already
         if(mode!=HOVERING){
            if((event->state&MIDDLEBUTTONMASK) || ((event->state&LEFTBUTTONMASK) && (event->state&RIGHTBUTTONMASK))){
               setOp(ZOOMING);
            }
            else if(event->state&RIGHTBUTTONMASK){
               setOp(TRANSLATING);
            }
         }
         return 1;
      case KEY_Control_L:
      case KEY_Control_R:

         // We do not switch modes unless something was going on already
         if(mode!=HOVERING){
            if(event->state&RIGHTBUTTONMASK){
               setOp(TRANSLATING);
            }
         }
         return 1;
            }
         }
         return 0;
      }


      // The widget lost the grab for some reason
      long Viewer::onUngrabbed(FXObject* sender,FXSelector sel,void* ptr){
         FXGLCanvas::onUngrabbed(sender,sel,ptr);
         flags&=~FLAG_PRESSED;
         flags&=~FLAG_CHANGED;
         flags|=FLAG_UPDATE;
         setOp(HOVERING);
         doesturbo=false;
         return 1;
      }


      // We timed out, i.e. the user didn't move for a while
      long Viewer::onTipTimer(FXObject*,FXSelector,void*){
         FXTRACE((250,"%s::onTipTimer %p\n",getClassName(),this));
         flags|=FLAG_TIP;
         return 1;
      }


      // We were asked about status text
      long Viewer::onQueryHelp(FXObject* sender,FXSelector sel,void* ptr){
         if(FXWindow::onQueryHelp(sender,sel,ptr)) return 1;
         if((flags&FLAG_HELP) && !help.empty()){
            sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&help);
            return 1;
         }
         return 0;
      }


      // We were asked about tip text
      long Viewer::onQueryTip(FXObject* sender,FXSelector sel,void* ptr){
         if(FXWindow::onQueryTip(sender,sel,ptr)) return 1;
         if(flags&FLAG_TIP){
            sint32 x,y; uint32 state;
            getCursorPosition(x,y,state);
            ObjectPtr hit=pick(x,y);
            if(hit && hit->handle(sender,sel,ptr)) return 1;
            if(!tip.empty()){
               sender->handle(this,FXSEL(SEL_COMMAND,ID_SETSTRINGVALUE),(void*)&tip);
               return 1;
            }
         }
         return 0;
      }


      /*****************************  Switch Projection  *****************************/


      // Switch to perspective mode
      long Viewer::OnCmdPerspective(FXObject*,FXSelector,void*){
         setProjection(PERSPECTIVE);
         return 1;
      }


      // Update sender
      long Viewer::OnUpdPerspective(FXObject* sender,FXSelector,void*){
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SHOW),0);
         sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
         sender->handle(this,(projection==PERSPECTIVE)?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),0);
         return 1;
      }


      // Switch to parallel mode
      long Viewer::OnCmdParallel(FXObject*,FXSelector,void*){
         setProjection(PARALLEL);
         return 1;
      }


      // Update sender
      long Viewer::OnUpdParallel(FXObject* sender,FXSelector,void*){
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SHOW),0);
         sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
         sender->handle(this,(projection==PARALLEL)?FXSEL(SEL_COMMAND,ID_CHECK):FXSEL(SEL_COMMAND,ID_UNCHECK),0);
         return 1;
      }


      /*****************************  Switch Viewpoints  *****************************/

      // View front
      long Viewer::OnCmdFront(FXObject*,FXSelector,void*){
         rotation=FXQuatf(0.0f,0.0f,0.0f,1.0f);
         updateTransform();
         update();
         return 1;
      }


      // Update sender
      long Viewer::OnUpdFront(FXObject* sender,FXSelector,void*){
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SHOW),0);
         sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
         sender->handle(this,(EPS>fabs(rotation[0]) && EPS>fabs(rotation[1]) && EPS>fabs(rotation[2]) && EPS>fabs(rotation[3]-1.0)) ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),0);
         return 1;
      }

      // View back
      long Viewer::OnCmdBack(FXObject*,FXSelector,void*){
         rotation=FXQuatf(0.0f,-1.0f,0.0f,0.0f);
         updateTransform();
         update();
         return 1;
      }


      // Update sender
      long Viewer::OnUpdBack(FXObject* sender,FXSelector,void*){
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SHOW),0);
         sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
         sender->handle(this,(EPS>fabs(rotation[0]) && EPS>fabs(rotation[1]+1.0) && EPS>fabs(rotation[2]) && EPS>fabs(rotation[3])) ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),0);
         return 1;
      }

      // View left
      long Viewer::OnCmdLeft(FXObject*,FXSelector,void*){
         rotation=FXQuatf(0.0f,0.7071067811865f,0.0f,0.7071067811865f);
         updateTransform();
         update();
         return 1;
      }


      // Update sender
      long Viewer::OnUpdLeft(FXObject* sender,FXSelector,void*){
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SHOW),0);
         sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
         sender->handle(this,(EPS>fabs(rotation[0]) && EPS>fabs(rotation[1]-0.7071067811865) && EPS>fabs(rotation[2]) && EPS>fabs(rotation[3]-0.7071067811865)) ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),0);
         return 1;
      }

      // View right
      long Viewer::OnCmdRight(FXObject*,FXSelector,void*){
         rotation=FXQuatf(0.0f,-0.7071067811865f,0.0f,0.7071067811865f);
         updateTransform();
         update();
         return 1;
      }


      // Update sender
      long Viewer::OnUpdRight(FXObject* sender,FXSelector,void*){
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SHOW),0);
         sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
         sender->handle(this,(EPS>fabs(rotation[0]) && EPS>fabs(rotation[1]+0.7071067811865) && EPS>fabs(rotation[2]) && EPS>fabs(rotation[3]-0.7071067811865)) ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),0);
         return 1;
      }

      // View top
      long Viewer::OnCmdTop(FXObject*,FXSelector,void*){
         rotation=FXQuatf(0.7071067811865f,0.0f,0.0f,0.7071067811865f);
         updateTransform();
         update();
         return 1;
      }


      // Update sender
      long Viewer::OnUpdTop(FXObject* sender,FXSelector,void*){
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SHOW),0);
         sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
         sender->handle(this,(EPS>fabs(rotation[0]-0.7071067811865) && EPS>fabs(rotation[1]) && EPS>fabs(rotation[2]) && EPS>fabs(rotation[3]-0.7071067811865)) ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),0);
         return 1;
      }

      // View bottom
      long Viewer::OnCmdBottom(FXObject*,FXSelector,void*){
         rotation=FXQuatf(-0.7071067811865f,0.0f,0.0f,0.7071067811865f);
         updateTransform();
         update();
         return 1;
      }


      // Update sender
      long Viewer::OnUpdBottom(FXObject* sender,FXSelector,void*){
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SHOW),0);
         sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
         sender->handle(this,(EPS>fabs(rotation[0]+0.7071067811865) && EPS>fabs(rotation[1]) && EPS>fabs(rotation[2]) && EPS>fabs(rotation[3]-0.7071067811865)) ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),0);
         return 1;
      }


      // Reset view
      long Viewer::OnCmdResetView(FXObject*,FXSelector,void*)
      {
         rotation=FXQuatf(0.0f,0.0f,0.0f,1.0f);
         zoom=1.0;
         scale=Vertex3D(1.0f,1.0f,1.0f);
         BoundingBox3D box;
         if(scene) scene->GetBoundingBox(box);
         setBounds(box);
         updateProjection();
         updateTransform();
         update();
         return 1;
      }


      // Fit view
      long Viewer::OnCmdFitView(FXObject*,FXSelector,void*)
      {
         BoundingBox3D box;
         if(scene) scene->GetBoundingBox(box);
         setBounds(box);
         update();
         return 1;
      }


      // Update zoom
      long Viewer::OnUpdZoom(FXObject* sender,FXSelector,void*){
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SETREALVALUE),(void*)&zoom);
         return 1;
      }


      // Change zoom
      long Viewer::OnCmdZoom(FXObject* sender,FXSelector sel,void*){
         double z=zoom;
         sender->handle(this,FXSEL(SEL_COMMAND,ID_GETREALVALUE),(void*)&z);
         doesturbo=(FXSELTYPE(sel)==SEL_CHANGED)?turbomode:false;
         setZoom(z);
         return 1;
      }


      // Update field of view
      long Viewer::OnUpdFov(FXObject* sender,FXSelector,void*){
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SETREALVALUE),(void*)&fov);
         return 1;
      }


      // Change field of view
      long Viewer::OnCmdFov(FXObject* sender,FXSelector sel,void*){
         double f=fov;
         sender->handle(this,FXSEL(SEL_COMMAND,ID_GETREALVALUE),(void*)&f);
         doesturbo=(FXSELTYPE(sel)==SEL_CHANGED)?turbomode:false;
         setFieldOfView(f);
         return 1;
      }


      // Scale model
      long Viewer::OnCmdXYZScale(FXObject* sender,FXSelector sel,void*){
         Vertex3D s=scale;
         double value;
         sender->handle(this,FXSEL(SEL_COMMAND,ID_GETREALVALUE),&value);
         s[FXSELID(sel)-ID_SCALE_X]=(float)value;
         doesturbo=(FXSELTYPE(sel)==SEL_CHANGED)?turbomode:false;
         setScale(s);
         return 1;
      }


      // Update scale value
      long Viewer::OnUpdXYZScale(FXObject* sender,FXSelector sel,void*){
         double value=scale[FXSELID(sel)-ID_SCALE_X];
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SETREALVALUE),(void*)&value);
         return 1;
      }


      // Rotate camera about model by means of dials
      long Viewer::OnCmdXYZDial(FXObject*,FXSelector sel,void* ptr)
      {
         const Vertex3D xaxis(1.0f,0.0f,0.0f);
         const Vertex3D yaxis(0.0f,1.0f,0.0f);
         const Vertex3D zaxis(0.0f,0.0f,1.0f);
         sint32 dialnew=(sint32)(FXival)ptr;
         float ang;
         FXQuatf q;
         if(FXSELTYPE(sel)==SEL_CHANGED){
            doesturbo=turbomode;
            FXASSERT(ID_DIAL_X<=FXSELID(sel) && FXSELID(sel)<=ID_DIAL_Z);
            switch(FXSELID(sel)){
      case ID_DIAL_X:
         ang=(float)(DTOR*(dialnew-dial[0]));
         q.setAxisAngle(Vertex2FXVec3f(xaxis),-ang);
         dial[0]=dialnew;
         break;
      case ID_DIAL_Y:
         ang=(float)(DTOR*(dialnew-dial[1]));
         q.setAxisAngle(Vertex2FXVec3f(yaxis), ang);
         dial[1]=dialnew;
         break;
      case ID_DIAL_Z:
         ang=(float)(DTOR*(dialnew-dial[2]));
         q.setAxisAngle(Vertex2FXVec3f(zaxis), ang);
         dial[2]=dialnew;
         break;
            }
            setOrientation(q*getOrientation());
         }
         else if(doesturbo){
            doesturbo=false;
            update();
         }
         return 1;
      }


      // Update dial value
      long Viewer::OnUpdXYZDial(FXObject* sender,FXSelector sel,void*){
         FXASSERT(ID_DIAL_X<=FXSELID(sel) && FXSELID(sel)<=ID_DIAL_Z);
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SETINTVALUE),(void*)&dial[FXSELID(sel)-ID_DIAL_X]);
         return 1;
      }


      // Update roll pitch yaw
      long Viewer::OnCmdRollPitchYaw(FXObject* sender,FXSelector sel,void*){
         FXASSERT(ID_ROLL<=FXSELID(sel) && FXSELID(sel)<=ID_YAW);
         float rpy[3];
         double ang;
         rotation.getRollPitchYaw(rpy[0],rpy[1],rpy[2]);
         sender->handle(this,FXSEL(SEL_COMMAND,ID_GETREALVALUE),(void*)&ang);
         rpy[FXSELID(sel)-ID_ROLL]=(float)(DTOR*ang);
         doesturbo=(FXSELTYPE(sel)==SEL_CHANGED)?turbomode:false;
         setOrientation(FXQuatf(rpy[0],rpy[1],rpy[2]));
         update();
         return 1;
      }


      // Update roll pitch yaw
      long Viewer::OnUpdRollPitchYaw(FXObject* sender,FXSelector sel,void*){
         FXASSERT(ID_ROLL<=FXSELID(sel) && FXSELID(sel)<=ID_YAW);
         float rpy[3];
         rotation.getRollPitchYaw(rpy[0],rpy[1],rpy[2]);
         double ang=RTOD*rpy[FXSELID(sel)-ID_ROLL];
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SETREALVALUE),(void*)&ang);
         return 1;
      }


      /******************************  Printing Support  *****************************/


      // Read back pixels
      // Derived from code contributed by <sancelot@crosswinds.net>
      bool Viewer::readPixels(FXColor*& buffer,sint32 x,sint32 y,sint32 w,sint32 h){

         if(1<=w && 1<=h){
            GLint swapbytes,lsbfirst,rowlength,skiprows,skippixels,alignment,oldbuf;
            register FXColor *p,*q,*pp,*qq,t;

            // Try allocate buffer
            if(FXMALLOC(&buffer,FXColor,w*h)){

               // Make context current
               makeCurrent();

               // Save old pixel formats
               glGetIntegerv(GL_PACK_SWAP_BYTES,&swapbytes);
               glGetIntegerv(GL_PACK_LSB_FIRST,&lsbfirst);
               glGetIntegerv(GL_PACK_ROW_LENGTH,&rowlength);
               glGetIntegerv(GL_PACK_SKIP_ROWS,&skiprows);
               glGetIntegerv(GL_PACK_SKIP_PIXELS,&skippixels);
               glGetIntegerv(GL_PACK_ALIGNMENT,&alignment);
               glGetIntegerv(GL_READ_BUFFER,&oldbuf);

               // Set pixel readback formats
               glPixelStorei(GL_PACK_SWAP_BYTES,GL_FALSE);
               glPixelStorei(GL_PACK_LSB_FIRST,GL_FALSE);
               glPixelStorei(GL_PACK_ROW_LENGTH,0);
               glPixelStorei(GL_PACK_SKIP_ROWS,0);
               glPixelStorei(GL_PACK_SKIP_PIXELS,0);
               glPixelStorei(GL_PACK_ALIGNMENT,1);

               // Read from the right buffer
               glReadBuffer((GLenum)GL_FRONT);

               // Read the pixels
               glReadPixels(x,y,w,h,GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)buffer);

               // Flip image upside down
               pp=buffer;
               qq=buffer+(h-1)*w;
               do{
                  p=pp; pp+=w;
                  q=qq; qq-=w;
                  do{
                     FXSWAP(*p,*q,t);
                     p++;
                     q++;
                  }
                  while(p<pp);
               }
               while(pp<qq);

               // Restore old formats
               glPixelStorei(GL_PACK_SWAP_BYTES,swapbytes);
               glPixelStorei(GL_PACK_LSB_FIRST,lsbfirst);
               glPixelStorei(GL_PACK_ROW_LENGTH,rowlength);
               glPixelStorei(GL_PACK_SKIP_ROWS,skiprows);
               glPixelStorei(GL_PACK_SKIP_PIXELS,skippixels);
               glPixelStorei(GL_PACK_ALIGNMENT,alignment);
               glReadBuffer((GLenum)oldbuf);

               // Make context non-current
               makeNonCurrent();
               return true;
            }
         }
         return false;
      }


      // Print the window by grabbing pixels
      long Viewer::OnCmdPrintImage(FXObject*,FXSelector,void*){
         FXColor *buffer;

         // First, ensure window is fully painted
         repaint();
         getApp()->flush(true);

         // Then try grab the pixels
         if(readPixels(buffer,0,0,width,height)){
            //    FXFileStream outfile;
            //    if(outfile.open("testje.bmp",FXStreamSave)){
            //      fxsaveBMP(outfile,buffer,width,height);
            //      outfile.close();
            //      }

            // Open print dialog
            FXPrintDialog dlg(this,tr("Print Scene"));

            // Run dialog
            if(dlg.execute()){
               FXPrinter printer;

               // Get the printer
               dlg.getPrinter(printer);

               // Printer device context
               FXDCPrint pdc(getApp());

               // Try open printer
               if(!pdc.beginPrint(printer)){
                  FXMessageBox::error(this,MBOX_OK,tr("Printer Error"),tr("Unable to print."));
                  return 1;
               }

               // Page header
               pdc.beginPage(1);

               // This is very ad-hoc; please don't look
               pdc.outf("/picstr %d string def\n",width*3);
               pdc.outf("%d %d translate\n",50,50);
               pdc.outf("%d %d scale\n",width,height);
               pdc.outf("%d %d %d\n",width,height,8);
               pdc.outf("[%d 0 0 -%d 0 %d]\n",width,height,height);
               pdc.outf("{currentfile picstr readhexstring pop}\n");
               pdc.outf("false %d\n",3);
               pdc.outf("colorimage\n");
               for(int i=0; i<width*height; i++){
                  pdc.outhex(FXREDVAL(buffer[i]));
                  pdc.outhex(FXGREENVAL(buffer[i]));
                  pdc.outhex(FXBLUEVAL(buffer[i]));
               }
               pdc.outf("\n");

               // Page trailer
               pdc.endPage();
               pdc.endPrint();
            }

            // Free the pixels
            FXFREE(&buffer);
         }
         return 1;
      }


      // Render
      sint32 Viewer::renderFeedback(float *buffer,sint32,sint32,sint32,sint32,sint32 maxbuffer)
      {

         sint32 used;
         makeCurrent();
         glFeedbackBuffer(maxbuffer,GL_3D_COLOR,buffer);
         glRenderMode(GL_FEEDBACK);
         DrawScene(m_view_port);
         used=glRenderMode(GL_RENDER);
         makeNonCurrent();
         return used;
      }


      // Read feedback buffer
      bool Viewer::readFeedback(float*& buffer,sint32& used,sint32& size,sint32 x,sint32 y,sint32 w,sint32 h)
      {
         bool ok=false;
         buffer=0;
         used=0;
         size=10000;
         while(!ok)
         {

            // Allocate buffer
            FXMALLOC(&buffer,float,size);

            // It got too big, give up
            if(!buffer) break;

            // Try to render scene into it
            used=renderFeedback(buffer,x,y,w,h,size);

            // No errors, got our stuff
            if(0<used)
            {
               ok=true;
               continue;
            }

            // It didn't fit, lets double the buffer and try again
            FXFREE(&buffer);
            size*=2;
         }
         return ok;
      }


      // Draw feedback buffer into dc
      void Viewer::drawFeedback(FXDCPrint& pdc,const float* buffer,sint32 used){

         sint32 nvertices,smooth,token,i,p;

         // Draw background
         pdc.outf("%g %g %g C\n",m_background[0][0],m_background[0][1],m_background[0][2]);
         pdc.outf("newpath\n");
         pdc.outf("%g %g moveto\n",0.0,0.0);
         pdc.outf("%g %g lineto\n",0.0,(double)height);
         pdc.outf("%g %g lineto\n",(double)width,(double)height);
         pdc.outf("%g %g lineto\n",(double)width,0.0);
         pdc.outf("closepath fill\n");

         pdc.outf("1 setlinewidth\n");

         // Crank out primitives
         p=0;
         while(p<used){
            token=(sint32)buffer[p++];
            switch(token){

               // Point primitive
      case GL_POINT_TOKEN:
         pdc.outf("%g %g %g %g %g P\n",buffer[p+0],buffer[p+1],buffer[p+3],buffer[p+4],buffer[p+5]);
         p+=7;             // Each vertex element in the feedback buffer is 7 floats
         break;

         // Line primitive
      case GL_LINE_RESET_TOKEN:
      case GL_LINE_TOKEN:
         if(fabs(buffer[p+3]-buffer[p+7+3])<1E-4 || fabs(buffer[p+4]-buffer[p+7+4])<1E-4 || fabs(buffer[p+5]-buffer[p+7+5])<1E-4){
            pdc.outf("%g %g %g %g %g %g %g %g %g %g SL\n",buffer[p+0],buffer[p+1],buffer[p+3],buffer[p+4],buffer[p+5], buffer[p+7+0],buffer[p+7+1],buffer[p+7+3],buffer[p+7+4],buffer[p+7+5]);
         }
         else{
            pdc.outf("%g %g %g %g %g %g %g L\n",buffer[p+0],buffer[p+1],buffer[p+7+0],buffer[p+7+1],buffer[p+3],buffer[p+4],buffer[p+5]);
         }
         p+=14;            // Each vertex element in the feedback buffer is 7 GLfloats
         break;

         // Polygon primitive
      case GL_POLYGON_TOKEN:
         nvertices = (sint32)buffer[p++];
         if(nvertices==3){ // We assume polybusting has taken place already!
            smooth=0;
            for(i=1; i<nvertices; i++){
               if(fabs(buffer[p+3]-buffer[p+i*7+3])<1E-4 || fabs(buffer[p+4]-buffer[p+i*7+4])<1E-4 || fabs(buffer[p+5]-buffer[p+i*7+5])<1E-4){ smooth=1; break; }
            }
            if(smooth){
               pdc.outf("%g %g %g %g %g %g %g %g %g %g %g %g %g %g %g ST\n",buffer[p+0],buffer[p+1],buffer[p+3],buffer[p+4],buffer[p+5], buffer[p+7+0],buffer[p+7+1],buffer[p+7+3],buffer[p+7+4],buffer[p+7+5], buffer[p+14+0],buffer[p+14+1],buffer[p+14+3],buffer[p+14+4],buffer[p+14+5]);
            }
            else{
               pdc.outf("%g %g %g %g %g %g %g %g %g T\n",buffer[p+0],buffer[p+1], buffer[p+7+0],buffer[p+7+1], buffer[p+14+0],buffer[p+14+1], buffer[p+3],buffer[p+4],buffer[p+5]);
            }
         }
         p+=nvertices*7;   // Each vertex element in the feedback buffer is 7 GLfloats
         break;

         // Skip these, don't deal with it here
      case GL_BITMAP_TOKEN:
      case GL_DRAW_PIXEL_TOKEN:
      case GL_COPY_PIXEL_TOKEN:
         p+=7;
         break;

         // Skip passthrough tokens
      case GL_PASS_THROUGH_TOKEN:
         p++;
         break;

         // Bad token, this is the end
      default:
         return;
            }
         }
      }


      // Print the window by means of feedback buffer
      long Viewer::OnCmdPrintVector(FXObject*,FXSelector,void*){
         FXPrintDialog dlg(this,tr("Print Scene"));
         FXPrinter printer;
         float *buffer;
         sint32 used,size;

         // Run dialog
         if(dlg.execute()){
            dlg.getPrinter(printer);
            FXDCPrint pdc(getApp());
            if(!pdc.beginPrint(printer)){
               FXMessageBox::error(this,MBOX_OK,tr("Printer Error"),tr("Unable to print."));
               return 1;
            }

            // Repaint now
            repaint();

            // Flush commands
            getApp()->flush(true);

            // Page header
            pdc.beginPage(1);

            // Read feedback
            if(readFeedback(buffer,used,size,0,0,width,height)){
               if(zsortfunc) (*zsortfunc)(buffer,used,size);   // FIXME:- may throw exception
               drawFeedback(pdc,buffer,used);
            }

            // Page trailer
            pdc.endPage();
            pdc.endPrint();
         }
         return 1;
      }


      // Zoom into lasso rectangle
      long Viewer::OnCmdLassoZoom(FXObject*,FXSelector,void*){
         setOp(DO_LASSOZOOM);
         return 1;
      }

      // Select objects in lasso rectangle
      long Viewer::OnCmdLassoSelect(FXObject*,FXSelector,void*){
         setOp(DO_LASSOSELECT);
         return 1;
      }

      /*****************************  Selection Support  *****************************/

      // We now really do have the m_selected_objects
      long Viewer::onClipboardGained(FXObject* sender,FXSelector sel,void* ptr){
         FXGLCanvas::onClipboardGained(sender,sel,ptr);
         return 1;
      }


      // We lost the m_selected_objects somehow
      long Viewer::onClipboardLost(FXObject* sender,FXSelector sel,void* ptr){
         FXGLCanvas::onClipboardLost(sender,sel,ptr);
         return 1;
      }


      // Somebody wants our m_selected_objects
      long Viewer::onClipboardRequest(FXObject* sender,FXSelector sel,void* ptr){
         FXEvent *event=(FXEvent*)ptr;
         //  FXuchar *data; FXuint len;

         // Try handling it in base class first
         if(FXGLCanvas::onClipboardRequest(sender,sel,ptr)) return 1;

         // Requested data from clipboard
         if(event->target==objectType){
            FXTRACE((100,"requested objectType\n"));
            //    FXMemoryStream stream;
            //    stream.open(0,0,FXStreamSave);
            //    stream.takeBuffer(data,len);
            //    stream.close();
            //    setDNDData(FROM_CLIPBOARD,objectType,data,len);
            return 1;
         }

         return 0;
      }

      // Update for current object
      long Viewer::OnUpdCurrent(FXObject* sender,FXSelector,void*){
         if(m_selected_objects.size())
         {
            sender->handle(this,FXSEL(SEL_COMMAND,ID_SHOW),0);
            sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
            return 1;
         }
         return 0;
      }


      // Set background color
      long Viewer::OnCmdBackColor(FXObject*,FXSelector sel,void* ptr)
      {
         FXColor color=(FXColor)(FXuval)ptr;
         m_background[0].FromUnsignedInt(color);
         m_background[1].FromUnsignedInt(color);
         m_background[2].FromUnsignedInt(color);
         m_background[3].FromUnsignedInt(color);
         if(FXSELTYPE(sel)==SEL_COMMAND || !turbomode)
         {
            update();
         }
         return 1;
      }


      // Update background color
      long Viewer::OnUpdBackColor(FXObject* sender,FXSelector,void*)
      {
         FXColor clr=m_background[0].ToUnsignedInt();
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SETVALUE),(void*)(FXuval)clr);
         return 1;
      }


      // Set gradient background color
      long Viewer::OnCmdGradientBackColor(FXObject*,FXSelector sel,void* ptr)
      {
         FXColor color=(FXColor)(FXuval)ptr;
         m_background[FXSELID(sel)-ID_TOP_LEFT_COLOR].FromUnsignedInt(color);
         if(FXSELTYPE(sel)==SEL_COMMAND || !turbomode)
         {
            update();
         }
         return 1;
      }


      // Update gradient background color
      long Viewer::OnUpdGradientBackColor(FXObject* sender,FXSelector sel,void*)
      {
         FXColor clr=m_background[FXSELID(sel)-ID_TOP_LEFT_COLOR].ToUnsignedInt();
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SETVALUE),(void*)(FXuval)clr);
         return 1;
      }


      // Set ambient light color
      long Viewer::OnCmdAmbientColor(FXObject*,FXSelector sel,void* ptr){
         FXColor color=(FXColor)(FXuval)ptr;
         ambient.FromUnsignedInt(color);
         if(FXSELTYPE(sel)==SEL_COMMAND || !turbomode){
            update();
         }
         return 1;
      }


      // Update ambient light color
      long Viewer::OnUpdAmbientColor(FXObject* sender,FXSelector,void*)
      {
         FXColor clr=ambient.ToUnsignedInt();
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SETVALUE),(void*)(FXuval)clr);
         return 1;
      }


      // Set ambient light color
      long Viewer::OnCmdLightAmbient(FXObject*,FXSelector sel,void* ptr)
      {
         FXColor color=(FXColor)(FXuval)ptr;
         light.ambient.FromUnsignedInt(color);
         if(FXSELTYPE(sel)==SEL_COMMAND || !turbomode){
            update();
         }
         return 1;
      }


      // Update ambient light color
      long Viewer::OnUpdLightAmbient(FXObject* sender,FXSelector,void*){
         FXColor clr=light.ambient.ToUnsignedInt();
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SETVALUE),(void*)(FXuval)clr);
         return 1;
      }


      // Set diffuse light color
      long Viewer::OnCmdLightDiffuse(FXObject*,FXSelector sel,void* ptr){
         FXColor color=(FXColor)(FXuval)ptr;
         light.diffuse.FromUnsignedInt(color);
         if(FXSELTYPE(sel)==SEL_COMMAND || !turbomode){
            update();
         }
         return 1;
      }


      // Update diffuse light color
      long Viewer::OnUpdLightDiffuse(FXObject* sender,FXSelector,void*){
         FXColor clr=light.diffuse.ToUnsignedInt();
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SETVALUE),(void*)(FXuval)clr);
         return 1;
      }


      // Set specular light color
      long Viewer::OnCmdLightSpecular(FXObject*,FXSelector sel,void* ptr){
         FXColor color=(FXColor)(FXuval)ptr;
         light.specular.FromUnsignedInt(color);
         if(FXSELTYPE(sel)==SEL_COMMAND || !turbomode){
            update();
         }
         return 1;
      }


      // Update specular light color
      long Viewer::OnUpdLightSpecular(FXObject* sender,FXSelector,void*){
         FXColor clr=light.specular.ToUnsignedInt();
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SETVALUE),(void*)(FXuval)clr);
         return 1;
      }


      // Toggle Turbo Mode
      long Viewer::OnCmdTurbo(FXObject*,FXSelector,void*){
         setTurboMode(!getTurboMode());
         return 1;
      }


      // Update Turbo Mode
      long Viewer::OnUpdTurbo(FXObject* sender,FXSelector,void*){
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SHOW),0);
         sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
         sender->handle(this,getTurboMode() ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),0);
         return 1;
      }


      // Toggle lighting
      long Viewer::OnCmdLighting(FXObject*,FXSelector,void*){
         options^=VIEWER_LIGHTING;
         update();
         return 1;
      }


      // Update lighting
      long Viewer::OnUpdLighting(FXObject* sender,FXSelector,void*){
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SHOW),0);
         sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
         sender->handle(this,(options&VIEWER_LIGHTING) ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),0);
         return 1;
      }


      // Toggle fog
      long Viewer::OnCmdFog(FXObject*,FXSelector,void*){
         options^=VIEWER_FOG;
         update();
         return 1;
      }


      // Update fog
      long Viewer::OnUpdFog(FXObject* sender,FXSelector,void*){
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SHOW),0);
         sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
         sender->handle(this,(options&VIEWER_FOG) ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),0);
         return 1;
      }


      // Toggle dithering
      long Viewer::OnCmdDither(FXObject*,FXSelector,void*)
      {
         options^=VIEWER_DITHER;
         update();
         return 1;
      }


      // Update dithering
      long Viewer::OnUpdDither(FXObject* sender,FXSelector,void*)
      {
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SHOW),0);
         sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
         sender->handle(this,(options&VIEWER_DITHER) ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),0);
         return 1;
      }

      // Toggle cull face
      long Viewer::OnCmdCullFace(FXObject*,FXSelector,void*)
      {
         options^=VIEWER_CULLFACE;
         update();
         return 1;
      }

      // Update cull face
      long Viewer::OnUpdCullFace(FXObject* sender,FXSelector,void*)
      {
         sender->handle(this,FXSEL(SEL_COMMAND,ID_SHOW),0);
         sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
         sender->handle(this,(options&VIEWER_CULLFACE) ? FXSEL(SEL_COMMAND,ID_CHECK) : FXSEL(SEL_COMMAND,ID_UNCHECK),0);
         return 1;
      }

      /*******************************  Drag and Drop  *******************************/


      // Handle drag-and-drop enter
      long Viewer::onDNDEnter(FXObject* sender,FXSelector sel,void* ptr){
         if(FXGLCanvas::onDNDEnter(sender,sel,ptr)) return 1;
         dropped=ObjectPtr();
         return 1;
      }

      // Handle drag-and-drop leave
      long Viewer::onDNDLeave(FXObject* sender,FXSelector sel,void* ptr){
         if(FXGLCanvas::onDNDLeave(sender,sel,ptr)) return 1;
         dropped=ObjectPtr();
         return 1;
      }


      // Handle drag-and-drop motion
      long Viewer::onDNDMotion(FXObject* sender,FXSelector sel,void* ptr){
         FXEvent* event=(FXEvent*)ptr;

         // Handled elsewhere
         if(FXGLCanvas::onDNDMotion(sender,sel,ptr)) return 1;

         // Dropped on some object
         if((dropped=pick(event->win_x,event->win_y))!=0){

            // Object agrees with drop type
            if(dropped->handle(this,sel,ptr)){
               acceptDrop(DRAG_COPY);
               return 1;
            }

            // Forget about the whole thing
            dropped=ObjectPtr();
            return 0;
         }

         // Dropped in viewer background; hope its a color
         if(offeredDNDType(FROM_DRAGNDROP,colorType)){
            acceptDrop(DRAG_COPY);
            return 1;
         }

         // Won't accept drop, dont know what it is
         return 0;
      }


      // Handle drag-and-drop drop
      long Viewer::onDNDDrop(FXObject* sender,FXSelector sel,void* ptr)
      {
         // Try base class first
         if(FXGLCanvas::onDNDDrop(sender,sel,ptr)) return 1;

         // Dropped on object?
         if(dropped){

            // Object handled drop; so probably want to repaint
            if(dropped->handle(this,sel,ptr)){
               update();
               return 1;
            }

            // We're done
            return 0;
         }

         // Dropped on viewer
         FX::FXuchar* tmp;
         uint32 len;
         if(getDNDData(FROM_DRAGNDROP,Viewer::colorType,tmp,len))
         {
            const FX::FXushort* clr=reinterpret_cast<const FX::FXushort*>(tmp);
            setBackgroundColor(Color(clr[0]/65535.0f,clr[1]/65535.0f,clr[2]/65535.0f,1.0f), -1);
            FXFREE(tmp);
            update();
            return 1;
         }
         return 0;
      }


      // Change projection
      void Viewer::setProjection(uint32 proj)
      {
         projection=proj;
         updateProjection();
         update();
      }


      // Set background
      void Viewer::setBackgroundColor(const Color& clr, sint32 idx)
      {
         if(idx==-1)
         {
            m_background[0] = clr;
            m_background[1] = clr;
            m_background[2] = clr;
            m_background[3] = clr;
         }
         else
         {
            m_background[idx]=clr;
         }
         update();
      }


      // Set ambient color
      void Viewer::setAmbientColor(const Color& clr)
      {
         ambient = clr;
         update();
      }


      // Delegate all other messages to the GL Object
      long Viewer::onDefault(FXObject* sender,FXSelector sel,void* ptr)
      {
         if (m_selected_objects.size())
         {
            ForeachHandle fh(sender, sel, ptr);
            fh = std::for_each(m_selected_objects.begin(), m_selected_objects.end(), fh);
            return fh.WasHandled() ? 1 : 0;
         }
         return 0;
      }


      // Change turbo mode
      void Viewer::setTurboMode(bool turbo){
         if(!turbo) doesturbo=false;
         turbomode=turbo;
      }


      // Return light settings
      void Viewer::getLight(Light& lite) const {
         lite=light;
      }


      // Change light settings
      void Viewer::setLight(const Light& lite) {
         light=lite;
         update();
      }

      // Close and release any resources
      Viewer::~Viewer()
      {
         getApp()->removeTimeout(this,ID_TIPTIMER);
      }

   }
}

