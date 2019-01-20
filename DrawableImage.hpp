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
 * file: DrawableImage.hpp
 *
 * This class implements a wxWidget window with an OpenGL sub window 
 * nested in it. The purpose of this software is to implement a GUI for 
 * viewing and manipulating image data and performing active contour model
 * segmentation of the images within.
 *
 ****************************************************************************
 */

#ifndef DRAWABLE_IMAGE_HPP
#define DRAWABLE_IMAGE_HPP

// Include Eigen first to make it happy
#define EIGEN_MPL2_ONLY
#include <Eigen/Eigen>

// include OpenGL
#ifdef __WXMAC__
    #include "OpenGL/glu.h"
    #include "OpenGL/gl.h"
#else
    #include <GL/glu.h>
    #include <GL/gl.h>
#endif

#include <wx/wx.h>

#include <iostream>
#include <cstdint>
#include <vector>

#include "Timer.hpp"

const size_t kBytesPerPixel = 3;

struct snaxel {
    size_t x;
    size_t y;
};

class DrawableImage {
    private:
        float                   m_xScale;
        float                   m_yScale;
       
        size_t                  m_xPos;
        size_t                  m_yPos;
        
        size_t                  m_hotSpotX;
        size_t                  m_hotSpotY;
        
        size_t                  m_angle;

        size_t                  m_width;
        size_t                  m_height;

        bool                    m_xFlip;
        bool                    m_yFlip;
        
        std::vector<uint8_t>    m_rawImage;
        std::vector<uint8_t>    m_processedImage;
        
        Eigen::MatrixXf         m_edgeMap;

        GLuint                  m_rawTextureId;
        GLuint                  m_processedTextureId;

    public:
        DrawableImage(const char* fileName);
        ~DrawableImage();

        void renderRawData();
        void renderProcessedData();
       
        void rotate(const size_t angle);
        void setFlip(const bool x, const bool y);
        void move(const size_t x, const size_t y);
        void setHotspot(const size_t x, const size_t y);
        void scale(const float x, const float y);
        void scale(const float k);

        size_t  width();
        size_t  height();
};

std::vector<uint8_t> loadImage(wxString path, size_t* imageWidth, size_t* imageHeight);
Eigen::MatrixXf rgbToGray(const std::vector<uint8_t>& rawImage, const size_t width, const size_t height);
std::vector<uint8_t> matToImage(const Eigen::MatrixXf& matrix);
Eigen::MatrixXf computeEdgeMap(const Eigen::MatrixXf& grayImage, const bool useConvoltion = true);

template <typename Derived, typename Derived2>
Derived conv2d(const Eigen::MatrixBase<Derived>& I, const Eigen::MatrixBase<Derived2> &kernel);

#endif
