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

#include "TCOpenGLFace.h"

#include "TCOutput.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace open_gl
   {
      Face::Face(const std::string& name, GLenum patch_type)
         :Object3D(name)
         ,m_material()
         ,m_renderer(patch_type)
      {
         SetOption(OP_SURFACE, true);
         SetOption(OP_SHADING_SMOOTH, true);

         m_material[0].ambient   = Color(0.2f,0.2f,0.2f);
         m_material[0].diffuse   = Color(0.2f,0.7f,0.85f);
         m_material[0].specular  = Color(1.0f,1.0f,1.0f);
         m_material[0].emission  = Color(0.0f,0.0f,0.0f);
         m_material[0].shininess = 30;
         m_material[1].ambient   = Color(0.2f,0.2f,0.2f);
         m_material[1].diffuse   = Color(0.8f,0.8f,0.8f);
         m_material[1].specular  = Color(1.0f,1.0f,1.0f);
         m_material[1].emission  = Color(0.0f,0.0f,0.0f);
         m_material[1].shininess = 30.0;
      }

      Face::Face()
         :m_renderer(0)
      {
      }

      Face::~Face()
      {
      }

      void Face::SetMaterials()
      {
         if (GetOption(OP_SHADING_SMOOTH) || 
             GetOption(OP_SHADING_FLAT))
         {
            if (GetOption(OP_SURFACE_DUALSIDED))
            {
               m_renderer.SetFrontMaterial(m_material[0]);
               m_renderer.SetBackMaterial(m_material[1]);
            }
            else
            {
               m_renderer.SetFrontMaterial(m_material[0]);
               m_renderer.SetBackMaterial(m_material[0]);
            }
         }
      }

      void Face::DrawSelf(open_gl::Viewer&) 
      {
         SetMaterials();

         if ((GetOption(OP_WIREFRAME) || GetOption(OP_POINTS)) && GetOption(OP_SURFACE))
         {
            m_renderer.EnableLineOffset();
         }

         RenderWireFrame();
         RenderPoints();
         RenderSurface();

         if ((GetOption(OP_WIREFRAME) || GetOption(OP_POINTS)) && GetOption(OP_SURFACE))
         {
            m_renderer.DisableLineOffset();
         }

      }

      void Face::DrawForHitSelf(open_gl::Viewer&)
      {
         m_renderer.DrawArraysFilled();
      }

      void Face::RenderWireFrame()
      {
         if (GetOption(OP_WIREFRAME))
         {
            if (GetOption(OP_WIREFRAME_COLORED))
            {
               m_renderer.EnableColorArray();
            }
            else
            {
               glColor3fv(open_gl::Color(0, 0, 0));
            }

            m_renderer.DrawArraysOutlined();
         }
      }

      void Face::RenderPoints()
      {
         if (GetOption(OP_POINTS))
         {
            if (GetOption(OP_POINTS_COLORED))
            {
               m_renderer.EnableColorArray();
            }
            else
            {
               glColor3fv(open_gl::Color(0, 0, 0));
            }

            m_renderer.DrawArraysPoints();
         }
      }

      void Face::RenderSurface()
      {
         if (GetOption(OP_SURFACE))
         {
            if (GetOption(OP_SURFACE_COLORED) && HasColors())
            {
               m_renderer.EnableColorArray();
            }

            if (GetOption(OP_SHADING_SMOOTH) || GetOption(OP_SHADING_FLAT))
            {
               if (GetOption(OP_SHADING_SMOOTH))
               {
                  ::glShadeModel(GL_SMOOTH);
               }
               else
               {
                  ::glShadeModel(GL_FLAT);
               }

               m_renderer.EnableNormalArray();

               if (GetOption(OP_SURFACE_COLORED) && HasColors())
               {
                  m_renderer.EnableColoredShading();
               }
               else
               {
                  m_renderer.EnableShading();
               }
            }
            else
            {
               if (!GetOption(OP_SURFACE_COLORED))
               {
                  glColor3fv(open_gl::Color(100, 100, 100));
               }
            }

            m_renderer.DrawArraysFilled();
            m_renderer.DisableShading();
         }
      }

      PatchRenderer& Face::GetRenderer()
      {
         return m_renderer;
      }

      // ----------------------------------------------------------
      // FOX ID mapping and coresponding methodes
      // ----------------------------------------------------------
      FXDEFMAP(Face) s_face_map[]=
      {
         FXMAPFUNC(FX::SEL_DND_DROP, 0, Face::OnDNDDrop),
         FXMAPFUNC(FX::SEL_DND_MOTION, 0, Face::OnDNDMotion),
         FXMAPFUNC(FX::SEL_QUERY_TIP, 0, Face::OnQueryTip),
         FXMAPFUNC(FX::SEL_COMMAND, Face::ID_FRONT_MATERIAL, Face::OnCmdFrontMaterial),
         FXMAPFUNC(FX::SEL_UPDATE, Face::ID_FRONT_MATERIAL, Face::OnUpdFrontMaterial),
         FXMAPFUNC(FX::SEL_COMMAND, Face::ID_BACK_MATERIAL, Face::OnCmdBackMaterial),
         FXMAPFUNC(FX::SEL_UPDATE, Face::ID_BACK_MATERIAL, Face::OnUpdBackMaterial),
         FXMAPFUNCS(FX::SEL_COMMAND, Face::FXID_SURFACE, Face::FXID_POINTS_COLORED, Face::OnCmdDrawingStyle),
         FXMAPFUNCS(FX::SEL_UPDATE, Face::FXID_SURFACE, Face::FXID_POINTS_COLORED, Face::OnUpdDrawingStyle)
      };
      FXIMPLEMENT_ABSTRACT(Face, Object3D, s_face_map, util::ArraySize(s_face_map))

      long Face::OnDNDDrop(FX::FXObject* sender,FX::FXSelector,void*)
      {
         uint32_t len; 
         FX::FXuchar* tmp;
         if(((FX::FXWindow*)sender)->getDNDData(FX::FROM_DRAGNDROP, FX::FXWindow::colorType,tmp,len))
         {
            Color color;
            const uint16_t* clr = reinterpret_cast<const uint16_t*>(tmp);
            color[0]=clr[0]/65535.0f;
            color[1]=clr[1]/65535.0f;
            color[2]=clr[2]/65535.0f;
            color[3]=clr[3]/65535.0f;
            FXFREE(&clr);

            m_material[0].diffuse=color;
            return 1;
         }
         return 0;
      }

      // Cursor got dragged over here.
      long Face::OnDNDMotion(FX::FXObject*,FX::FXSelector,void*)
      {
         return 1;
      }

      // We were asked about tip text
      long Face::OnQueryTip(FX::FXObject* sender,FX::FXSelector,void*)
      {
         FX::FXString tip(GetName().c_str());
         sender->handle(this,FXSEL(FX::SEL_COMMAND, FX::FXWindow::ID_SETSTRINGVALUE), &tip);
         return 1;
      }

      // Command from material editor
      long Face::OnCmdFrontMaterial(FX::FXObject*,FX::FXSelector,void *ptr)
      {
         m_material[0] = *reinterpret_cast<Material*>(ptr);
         return 1;
      }


      // Update material editor
      long Face::OnUpdFrontMaterial(FX::FXObject *sender,FX::FXSelector,void*)
      {
         sender->handle(this, FXSEL(FX::SEL_COMMAND,FX::FXWindow::ID_SETVALUE),&m_material[0]);
         return 1;
      }


      // Command from material editor
      long Face::OnCmdBackMaterial(FX::FXObject*,FX::FXSelector,void *ptr)
      {
         m_material[1] = *reinterpret_cast<Material*>(ptr);
         return 1;
      }


      // Update material editor
      long Face::OnUpdBackMaterial(FX::FXObject *sender,FX::FXSelector,void*)
      {
         sender->handle(this,FXSEL(FX::SEL_COMMAND,FX::FXWindow::ID_SETVALUE),&m_material[1]);
         return 1;
      }

      // Drawing style toggles
      long Face::OnCmdDrawingStyle(FX::FXObject*,FX::FXSelector sel,void*)
      {
         switch(FXSELID(sel))
         {
         case FXID_SURFACE:         
            SetOption(OP_SURFACE, !GetOption(OP_SURFACE)); 
            break;
         case FXID_SURFACE_COLORED: 
            SetOption(OP_SURFACE_COLORED, !GetOption(OP_SURFACE_COLORED)); 
            break;
         case FXID_SHADE_SMOOTH:    
            SetOption(OP_SHADING_SMOOTH, true); 
            SetOption(OP_SHADING_FLAT, false); 
            break;
         case FXID_SHADE_FLAT:
            SetOption(OP_SHADING_SMOOTH, false); 
            SetOption(OP_SHADING_FLAT, true); 
            break;
         case FXID_SHADE_OFF:
            SetOption(OP_SHADING_SMOOTH, false); 
            SetOption(OP_SHADING_FLAT, false); 
            break;
         case FXID_WIREFRAME:   
            SetOption(OP_WIREFRAME, !GetOption(OP_WIREFRAME)); 
            break;
         case FXID_WIREFRAME_COLORED: 
            SetOption(OP_WIREFRAME_COLORED, !GetOption(OP_WIREFRAME_COLORED)); 
            break;
         case FXID_POINTS:      
            SetOption(OP_POINTS, !GetOption(OP_POINTS)); 
            break;
         case FXID_POINTS_COLORED:     
            SetOption(OP_POINTS_COLORED, !GetOption(OP_POINTS_COLORED)); 
            break;
         }
         return 1;
      }


      // Update drawing style toggles
      long Face::OnUpdDrawingStyle(FX::FXObject *sender,FX::FXSelector sel,void*)
      {
         bool is_active = false;
         switch(FXSELID(sel))
         {
         case FXID_SURFACE:
            is_active = GetOption(OP_SURFACE);
            break;
         case FXID_SURFACE_COLORED: 
            is_active = GetOption(OP_SURFACE_COLORED);
            break;
         case FXID_SHADE_SMOOTH:    
            is_active = GetOption(OP_SHADING_SMOOTH);
            break;
         case FXID_SHADE_FLAT:
            is_active = GetOption(OP_SHADING_FLAT);
            break;
         case FXID_SHADE_OFF:
            is_active = !GetOption(OP_SHADING_FLAT) && !GetOption(OP_SHADING_SMOOTH);
            break;
         case FXID_WIREFRAME:   
            is_active = GetOption(OP_WIREFRAME);
            break;
         case FXID_WIREFRAME_COLORED: 
            is_active = GetOption(OP_WIREFRAME_COLORED);
            break;
         case FXID_POINTS:      
            is_active = GetOption(OP_POINTS);
            break;
         case FXID_POINTS_COLORED:     
            is_active = GetOption(OP_POINTS_COLORED);
            break;
         default:
            return 0;
         }

         FX::FXSelector msg=  is_active ? (FXSEL(FX::SEL_COMMAND,FX::FXWindow::ID_CHECK)) :
                                          (FXSEL(FX::SEL_COMMAND,FX::FXWindow::ID_UNCHECK));
         sender->handle(this,msg,0);
         sender->handle(this,FXSEL(FX::SEL_COMMAND,FX::FXWindow::ID_ENABLE), 0);
         return 1;
      }
   }
}

