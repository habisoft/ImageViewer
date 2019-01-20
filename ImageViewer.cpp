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
 * file: ImageViewer.cpp
 *
 * This class implements a wxWidget window with an OpenGL sub window 
 * nested in it. The purpose of this software is to implement a GUI for 
 * viewing and manipulating image data and performing active contour model
 * segmentation of the images within.
 *
 ****************************************************************************
 */

#include "ImageViewer.hpp"

class MyApp: public wxApp {
    virtual bool OnInit();

    wxFrame*        frame;
    BasicGLPane*    glPane;
    
    public:

};

IMPLEMENT_APP(MyApp)


bool MyApp::OnInit() {
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    frame = new wxFrame((wxFrame *) NULL, -1,  wxT("Snake Viewer"), wxPoint(50, 50), wxSize(kDefaultWindowWidth, kDefaultWindowHeight));
    
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    
    glPane  = new BasicGLPane((wxFrame *) frame, "ferret.jpg", args);
    frame->Show();
    
    return true;
}

BasicGLPane::BasicGLPane(wxFrame* parent, const char* fileName, int* args) :
    wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {

    m_context = new wxGLContext(this);
    
    // TODO: introduce some logic around the filename to test it before attempting to load
    m_imageFileName = std::string(fileName);
    m_drawableImage = NULL;

    // To avoid flashing on MSW
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    m_showProcessed = false;
}

BasicGLPane::~BasicGLPane() {
    if (m_context) {
        delete m_context;
    }
    
    if (m_drawableImage) {
        delete m_drawableImage;
    }
}

void BasicGLPane::resized(wxSizeEvent& evt) {
    //wxGLCanvas::OnSize(evt);

    Refresh();
}

/** Inits the OpenGL viewport for drawing in 2D. */
void BasicGLPane::prepare2DViewport(const int minX, const int minY, const int maxX, const int maxY) {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Red Background
    glEnable(GL_TEXTURE_2D);   // textures
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glViewport(minX, minY, maxX - minX, maxY - minY);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(minX, maxX, maxY, minY);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void BasicGLPane::render(wxPaintEvent& evt) {
    if (!IsShown()) {
        return;
    }

    wxGLCanvas::SetCurrent(*m_context);

    // If the image has not been loaded yet, go ahead and load it.
    if (m_drawableImage == NULL) {
        m_drawableImage = new DrawableImage(m_imageFileName.c_str());
    }

    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ------------- draw some 2D ----------------
    prepare2DViewport(0, 0, getWidth(), getHeight());
   
    const float scaleX = (float) getWidth() / (float) m_drawableImage->width();
    const float scaleY = (float) getHeight() / (float) m_drawableImage->height();;

    m_drawableImage->scale(scaleX, scaleY);
    
    if (m_showProcessed) {
        m_drawableImage->renderProcessedData();
    }
    else {
        m_drawableImage->renderRawData();
    }
    
    glFlush();
    SwapBuffers();
}


// some useful events to use
void BasicGLPane::mouseMoved(wxMouseEvent& event) {

}

void BasicGLPane::mouseDown(wxMouseEvent& event) {
    const int xPos = event.GetPosition().x;
    const int yPos = event.GetPosition().y;

    std::cout << "BasicGLPane::mouseDown(): x,y: " << xPos << ", " << yPos << "." << std::endl;    
}

void BasicGLPane::mouseWheelMoved(wxMouseEvent& event) {
    
}

void BasicGLPane::mouseReleased(wxMouseEvent& event) {
    
}

void BasicGLPane::rightClick(wxMouseEvent& event) {
    
}

void BasicGLPane::mouseLeftWindow(wxMouseEvent& event) {
    
}

void BasicGLPane::keyPressed(wxKeyEvent& event) {
    if (event.GetKeyCode() == WXK_F1) {
        std::cout << "\nImageViewer::keyPressed(): F1 key was pressed" << std::endl;
        m_showProcessed = (!m_showProcessed);

        if (m_showProcessed) {
            std::cout << "ImageViewer::keyPressed(): the processed image will be displayed" << std::endl;
        }
        else {
            std::cout << "ImageViewer::keyPressed(): the raw image will be displayed" << std::endl;
        }
        
        wxPaintEvent paintEvent;
        render(paintEvent);
    }
}

void BasicGLPane::keyReleased(wxKeyEvent& event) {
    
}

BEGIN_EVENT_TABLE(BasicGLPane, wxGLCanvas)
    EVT_MOTION(BasicGLPane::mouseMoved)
    EVT_LEFT_DOWN(BasicGLPane::mouseDown)
    EVT_LEFT_UP(BasicGLPane::mouseReleased)
    EVT_RIGHT_DOWN(BasicGLPane::rightClick)
    EVT_LEAVE_WINDOW(BasicGLPane::mouseLeftWindow)
    EVT_SIZE(BasicGLPane::resized)
    EVT_KEY_DOWN(BasicGLPane::keyPressed)
    EVT_KEY_UP(BasicGLPane::keyReleased)
    EVT_MOUSEWHEEL(BasicGLPane::mouseWheelMoved)
    EVT_PAINT(BasicGLPane::render)
END_EVENT_TABLE()

int BasicGLPane::getWidth() {
    return GetSize().x;
}
    
int BasicGLPane::getHeight() {
    return GetSize().y;
}

