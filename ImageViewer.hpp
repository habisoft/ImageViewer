/****************************************************************************
 * HabiSoft, LLC
 ****************************************************************************
 * 
 * (c) [2018] - [present]
 * All Rights Reserved.
 * 
 * Limited License: Under no circumstance is commercial use, reproduction, or
 * distribution permitted. Use, reproduction, and distribution are permitted
 * solely for educational purposes.
 *
 * Any reproduction or distribution of source code must retain the above
 * copyright notice and the full text of this license including the Disclaimer,
 * below. 
 *
 * Any reproduction or distribution in binary form must reproduce the above
 * copyright notice and the full text of this license including the Disclaimer
 * below in the documentation and/or other materials provided with the Distribution.
 *
 * DISCLAIMER
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************
 *
 * file: ImageViewer.hpp
 *
 * This class implements a wxWidget window with an OpenGL sub window 
 * nested in it. The purpose of this software is to implement a GUI for 
 * viewing and manipulating image data and performing active contour model
 * segmentation of the images within.
 *
 ****************************************************************************
 */

#ifndef SNAKE_VIEWER_HPP
#define SNAKE_VIEWER_HPP

// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#include <string>

#include "DrawableImage.hpp"

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/notebook.h>
#include <wx/glcanvas.h>

const size_t kDefaultWindowWidth    = 1024;
const size_t kDefaultWindowHeight   = 768;

//const size_t kDefaultWindowWidth    = 2048;
//const size_t kDefaultWindowHeight   = 1536;

// Here trying to automatically detect the screen resolution
//const size_t kDefaultWindowWidth  = 0.75 * wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
//const size_t kDefaultWindowHeight = 0.75 * wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

class BasicGLPane : public wxGLCanvas {
    private:
        wxGLContext*    m_context;
        std::string     m_imageFileName;
        
        DrawableImage*  m_drawableImage;

        bool            m_showProcessed;

    public:
        BasicGLPane(wxFrame* parent, const char* fileName, int* args);
        virtual ~BasicGLPane();

        void resized(wxSizeEvent& evt);

        int getWidth();
        int getHeight();

        void render(wxPaintEvent& evt);
        void prepare2DViewport(const int minX, const int minY, const int maxX, const int maxY);

        // events
        void mouseMoved(wxMouseEvent& event);
        void mouseDown(wxMouseEvent& event);
        void mouseWheelMoved(wxMouseEvent& event);
        void mouseReleased(wxMouseEvent& event);
        void rightClick(wxMouseEvent& event);
        void mouseLeftWindow(wxMouseEvent& event);
        void keyPressed(wxKeyEvent& event);
        void keyReleased(wxKeyEvent& event);

        DECLARE_EVENT_TABLE()
};

#endif
