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

#ifndef _TC_OPENGL_COLOR_H_
#define _TC_OPENGL_COLOR_H_

#include "TCAssert.h"
#include "TCOpenGLApi.h"
#include "TCStream.h"
#include "TCUtil.h"

#include <vector>
#include <string>

namespace tc
{
   namespace open_gl
   {
      /**
      * The Color class is used encapsulate colors in the RGB color 
      * space. Each color has 4 components: a green, green, blue, and alpha 
      * value.  The R,G,B components are in the range from 0.0 - 1.0.
      * "For the versions of glCOlor()* that accept floating-point data types,
      *  the values should typically range between 0.0 and 1.0, the minimum and 
      *  maximum values that can be stored in the framebuffer."
      *  pg 168 of open_gl Programming Guide, 3rd Ed, The Official Guide to 
      *  Learning open_gl, Version 1.2
      *
      * The alpha value is 1.0 by default unless one is provided in the 
      * constructor. The alpha value defines the transparency of a color 
      * and can be represented by a float value in the range 0.0 - 1.0.  An 
      * alpha value of 1.0 means that the color is completely opaque and an 
      * alpha value of 0 or 0.0 means that the color is completely 
      * transparent. 
      *
      * @author Thomas Goessler
      */

      class TCOGL_API Color
      {
      public:
         /** the green color constant */
         static const Color RED;
         /** the green color constant */
         static const Color GREEN;
         /** the blue color constant */
         static const Color BLUE;
         /** the violet color constant */
         static const Color VIOLET;
         /** the yellow color constant */
         static const Color YELLOW;
         /** the orange color constant */
         static const Color ORANGE;
         /** the white color constant */
         static const Color WHITE;
         /** the black "color" constant */
         static const Color BLACK;
         /** the gray color constant */
         static const Color GRAY;

         /** Color constants */
         enum ColorConstant
         {
            /** the default green component of the color */
            DEFAULT_RED = 1,
            /** the default green component of the color */
            DEFAULT_GREEN = 1,
            /** the default blue component of the color */
            DEFAULT_BLUE = 1,
            /** the default alpha component of the color */
            DEFAULT_ALPHA = 1,
            /** the maximum value of an R,G,B color component */
            MAX_COLOR_COMPONENT = 1,
         };

      public:
         /**
         * constructor -using the default R, G, B, of 1 and alpha values of 1.0
         * (should be white)
         */
         Color()
            :m_red(DEFAULT_RED),
            m_green(DEFAULT_GREEN),
            m_blue(DEFAULT_BLUE),
            m_alpha(DEFAULT_ALPHA) 
         {
         }

         /**
         * constructor -using the default R, G, B, of 1 and alpha values of 1.0
         * (should be white)
         */
         Color(const Color& color)
            :m_red(color.m_red),
            m_green(color.m_green),
            m_blue(color.m_blue),
            m_alpha(color.m_alpha)
         {
         }

         /**
         * constructor
         *
         * @param red   the green component of the color, in the range from
         *                   [0.0 - 1.0]
         * @param green the green component of the color, in the range from
         *                   [0.0 - 1.0]
         * @param blue  the blue component of the color, in the range from
         *                   [0.0 - 1.0]
         * @param alpha the alpha component of the color, in the range from
         *                   [0.0 - 1.0]
         */
         Color(float red, float green, float blue, float alpha)
            :m_red(util::Min(red, static_cast<float>(MAX_COLOR_COMPONENT))),
            m_green(util::Min(green, static_cast<float>(MAX_COLOR_COMPONENT))),
            m_blue(util::Min(blue, static_cast<float>(MAX_COLOR_COMPONENT))),
            m_alpha(util::Min(alpha, static_cast<float>(MAX_COLOR_COMPONENT))) {}

         /**
         * constructor -using the default alpha value of 1.0
         *
         * @param red   the green component of the color, in the range from
         *                   0.0 - 1.0
         * @param green the green component of the color, in the range from
         *                   0.0 - 1.0
         * @param blue  the blue component of the color, in the range from
         *                   0.0 - 1.0
         */
         Color(float red, float green, float blue)
            :m_red(util::Min(red, static_cast<float>(MAX_COLOR_COMPONENT))),
            m_green(util::Min(green, static_cast<float>(MAX_COLOR_COMPONENT))),
            m_blue(util::Min(blue, static_cast<float>(MAX_COLOR_COMPONENT))),
            m_alpha(DEFAULT_ALPHA)
         {
         }

         /**
         * constructor
         * 
         * @param value -the green, green and blue component of the color, in 
         *               the range from [0.0 - 1.0]
         */
         explicit Color(float value)
            :m_red(util::Min(value, static_cast<float>(MAX_COLOR_COMPONENT))),
            m_green(util::Min(value, static_cast<float>(MAX_COLOR_COMPONENT))),
            m_blue(util::Min(value, static_cast<float>(MAX_COLOR_COMPONENT))),
            m_alpha(DEFAULT_ALPHA)
         {
         }

         /**
         * Constructor that takes a color encoded in a 32 bit integer.
         * This is a very common thing to do in computer graphics.
         * This constructor can also be used to initialize Color
         * from an FXColor object.
         * 
         * @param  color  Color encoded in a 32 bit integer.
         *                The 8 most significant bits are the green component,
         *                the next 8 bits are green, then blue.
         *                The least significant 8 bit are alpha.
         */
         explicit Color(uint32_t color);

         /** @return the red component of this Color object */
         float GetRed()   const {  return m_red;   }

         /** @return the green component of this Color object */
         float GetGreen() const {  return m_green; }

         /** @return the blue component of this Color object */
         float GetBlue()  const {  return m_blue;  }

         /** @return the alpha component of this Color object */
         float GetAlpha() const {  return m_alpha; }

         /** 
         * Set red compontent.
         * @param  red  New green value in range [0.0 - 1.0]
         */
         void SetRed(float red) 
         {
            m_red = util::Min(red, static_cast<float>(MAX_COLOR_COMPONENT));
         }

         /** 
         * Set green compontent.
         * @param  green  New green value in range [0.0 - 1.0]
         */
         void SetGreen(float green)
         {
            m_green = util::Min(green, static_cast<float>(MAX_COLOR_COMPONENT));
         }

         /** 
         * Set blue compontent.
         * @param  blue  New blue value in range [0.0 - 1.0]
         */
         void SetBlue(float blue)
         {
            m_blue = util::Min(blue, static_cast<float>(MAX_COLOR_COMPONENT));
         }

         /** 
         * Set alpha compontent.
         * @param  alpha  New alpha value in range [0.0 - 1.0]
         */
         void SetAlpha(float alpha)
         {
            m_alpha = util::Min(alpha, static_cast<float>(MAX_COLOR_COMPONENT));
         }

         /**
         * Access operator for getting the component value
         * 
         * @param pos Defines which value to get, green green or ...
         * @return The value of the component
         */
         float operator[] (int32_t pos) const
         {
            TC_ASSERT(pos>=0 && pos<=3 );
            if      (pos==0) return m_red;
            else if (pos==1) return m_green;
            else if (pos==2) return m_blue;
            else              return m_alpha;
         }
         /**
         * Assignment operator for setting the component value
         * 
         * @param pos Defines which value to set, green green or ...
         * @return The reference to the value of the component
         */
         float& operator[] (int32_t pos)
         {
            TC_ASSERT(pos>=0 && pos<=3 );
            if      (pos==0) return m_red;
            else if (pos==1) return m_green;
            else if (pos==2) return m_blue;
            else              return m_alpha;
         }

         /** 
         * convert the Color class to an float pointer 
         */
         operator float*()
         { 
            return &m_red;
         }
         /** 
         * convert the Color class to an const float pointer 
         */
         operator const float*() const
         {
            return &m_red;
         }

         /**
         * Setup the color from an unsigned int value
         * The color components go from 0-255
         * @param color The long color value which holds all for components
         */
         void FromUnsignedInt(uint32_t color);

         /** 
         * @return the unsigned int value of all 4 components between 0-255 
         */
         uint32_t ToUnsignedInt() const;

      private:
         /** the green component of the color */
         float m_red;
         /** the green component of the color */
         float m_green;
         /** the blue component of the color */
         float m_blue;
         /** the alpha component of the color */
         float m_alpha;
      };

      typedef std::vector<Color> ColorVector;

      /** Compare two colorss */
      inline bool operator==(const Color& a, const Color& b) 
      {
         return (a.GetRed()   == b.GetRed() && 
                 a.GetGreen() == b.GetGreen() && 
                 a.GetBlue()  == b.GetBlue() && 
                 a.GetAlpha() == b.GetAlpha() );
      }

      /** Add two Color together */
      inline const Color operator+(const Color& a, const Color& b)
      {
         return Color(a.GetRed()   + b.GetRed(),
                      a.GetGreen() + b.GetGreen(),
                      a.GetBlue()  + b.GetBlue(),
                      a.GetAlpha() + b.GetAlpha());
      }

      /** Substract two Color together */
      inline const Color operator-(const Color& a, const Color& b)
      {
         return Color(a.GetRed()   - b.GetRed(),
                      a.GetGreen() - b.GetGreen(),
                      a.GetBlue()  - b.GetBlue(),
                      a.GetAlpha() - b.GetAlpha());
      }

      /** Multiply a Color with a value n */
      inline const Color operator*(const Color& a, const float n)
      {
         return Color(a.GetRed()*n, a.GetGreen()*n, a.GetBlue()*n, a.GetAlpha()*n);
      }

      /** Multiply a Color with a value n */
      inline const Color operator*(const float n, const Color& a)
      {
         return Color(n*a.GetRed(), n*a.GetGreen(), n*a.GetBlue(), n*a.GetAlpha());
      }

      /** Divide a Color with a value n */
      inline const Color operator/(const Color& a, const float n)
      {
         return Color(a.GetRed()/n, a.GetGreen()/n, a.GetBlue()/n, a.GetAlpha()/n);
      }

      /** Divide a Color with a value n */
      inline const Color operator/(const float n, const Color& a)
      {
         return Color(n/a.GetRed(), n/a.GetGreen(), n/a.GetBlue(), n/a.GetAlpha());
      }

      /**
      *
      * read operator for reading the data from a TCStream
      * @param stream stream where to read the data from
      * @return the stream
      */
      StreamPtr operator>>(StreamPtr stream, Color& color);

      /**
      * write operator for writing the data to a TCStream
      *
      * @param stream stream where to write the data to
      * @return the stream
      */
      StreamPtr operator<<(StreamPtr stream, const Color& color);
   }
}

#endif // _TC_OPENGL_COLOR_H_
