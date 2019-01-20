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
 * file: DrawableImage.cpp
 *
 * This class implements a wxWidget window with an OpenGL sub window 
 * nested in it. The purpose of this software is to implement a GUI for 
 * viewing and manipulating image data and performing active contour model
 * segmentation of the images within.
 *
 ****************************************************************************
 */

#include "DrawableImage.hpp"

/*
 * This is a simple class built on top of OpenGL that manages drawing images in a higher-level and quicker way.
 */

DrawableImage::DrawableImage(const char* fileName) {
    m_xScale    = 1.0;
    m_yScale    = 1.0;

    m_xPos      = 0;
    m_yPos      = 0;

    m_hotSpotX  = 0;
    m_hotSpotY  = 0;

    m_angle     = 0;

    m_xFlip     = false;
    m_yFlip     = false;

    Timer timer;

    if (fileName) {
        m_rawImage = loadImage(fileName, &m_width, &m_height);
        m_edgeMap = rgbToGray(m_rawImage, m_width, m_height);
        timer.tick();
        m_edgeMap = computeEdgeMap(m_edgeMap);
        const double latency = timer.tock();

        std::cout << "DrawableImage::DrawableImage(): edge map time: " << latency << " ms." << std::endl;

        m_processedImage = matToImage(m_edgeMap);
    }
    else {
        // TODO: gracefully handle the image not being loaded.
    }

    if (m_rawImage.size() > 0) {
        // Generate a texture for the raw image data.
        glGenTextures(1, &m_rawTextureId);
        std::cout << "DrawableImage::DrawableImage(): generated texture id: " << m_rawTextureId << std::endl;

        glBindTexture(GL_TEXTURE_2D, m_rawTextureId);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB,  GL_UNSIGNED_BYTE, m_rawImage.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_LINEAR
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_LINEAR

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


        // Generate a texture for the processed image data.
        glGenTextures(1, &m_processedTextureId);
        std::cout << "DrawableImage::DrawableImage(): generated texture id: " << m_processedTextureId << std::endl;

        glBindTexture(GL_TEXTURE_2D, m_processedTextureId);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB,  GL_UNSIGNED_BYTE, m_processedImage.data());
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_width, m_height, 0, GL_R8,  GL_UNSIGNED_BYTE, m_processedImage.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_LINEAR
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_LINEAR

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
}

DrawableImage::~DrawableImage() {
    glDeleteTextures (1, &m_rawTextureId);
    glDeleteTextures (1, &m_processedTextureId);
}

void DrawableImage::setFlip(const bool x, const bool y) {
    m_xFlip = x;
    m_yFlip = y;
}

void DrawableImage::setHotspot(const size_t x, const size_t y) {
    m_hotSpotX = x;
    m_hotSpotY = y;
}

void DrawableImage::move(const size_t x, const size_t y) {
    m_xPos = x;
    m_yPos = y;
}

void DrawableImage::scale(const float x, const float y) {
    m_xScale = x;
    m_yScale = y;
}

void DrawableImage::scale(const float k) {
    m_xScale = k;
    m_yScale = k;
}

void DrawableImage::rotate(size_t m_angle) {
    m_angle = m_angle;
}

void DrawableImage::renderRawData() {
    assert(m_rawImage.size() > 0);

    glLoadIdentity();
    glTranslatef(m_xPos, m_yPos, 0);

    if (m_xScale != 1.0 || m_yScale != 1.0) {
        glScalef(m_xScale, m_yScale, 1.0);
    }

    if (m_angle != 0) {
        glRotatef(m_angle, 0, 0, 1);   
    }

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glBindTexture(GL_TEXTURE_2D, m_rawTextureId);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    // top left
    glTexCoord2i(0, 0); 
    glVertex2i(0, 0);

    // bottom left
    glTexCoord2i(0, 1);
    glVertex2i(0, m_height);

    // bottom right
    glTexCoord2i(1, 1);
    glVertex2i(m_width, m_height);

    // top right
    glTexCoord2i(1, 0);
    glVertex2i(m_width, 0);
    glEnd();

    //glPopMatrix();
}

void DrawableImage::renderProcessedData() {
    assert(m_processedImage.size() > 0);

    glLoadIdentity();
    glTranslatef(m_xPos, m_yPos, 0);

    if (m_xScale != 1.0 || m_yScale != 1.0) {
        glScalef(m_xScale, m_yScale, 1.0);
    }

    if (m_angle != 0) {
        glRotatef(m_angle, 0, 0, 1);   
    }

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glBindTexture(GL_TEXTURE_2D, m_processedTextureId);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
        // top left
        glTexCoord2i(0, 0); 
        glVertex2i(0, 0);

        // bottom left
        glTexCoord2i(0, 1);
        glVertex2i(0, m_height);

        // bottom right
        glTexCoord2i(1, 1);
        glVertex2i(m_width, m_height);

        // top right
        glTexCoord2i(1, 0);
        glVertex2i(m_width, 0);
    glEnd();
}

size_t DrawableImage::width() {
    return m_width;
}

size_t DrawableImage::height() {
    return m_height;
}

Eigen::MatrixXf rgbToGray(const std::vector<uint8_t>& rawImage, const size_t width, const size_t height) {
    Eigen::MatrixXf I(height, width);

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            const size_t idx = (i * width + j) * kBytesPerPixel;

            const float red = rawImage[idx + 0];
            const float grn = rawImage[idx + 1];
            const float blu = rawImage[idx + 2];

            const float gray = (0.2126f * red + 0.7512f * grn + 0.0722 * blu);

            I(i, j) = gray;
        }
    }

    return I;
}

std::vector<uint8_t> loadImage(wxString path, size_t* imageWidth, size_t* imageHeight) {
    // the first time, init image handlers (remove this part if you do it somewhere else in your app)
    static bool is_first_time = true;

    if(is_first_time) {
        wxInitAllImageHandlers();
        is_first_time = false;
    }

    // check the file exists
    if(!wxFileExists(path)) {
        wxMessageBox( _("Failed to load resource image") );
        exit(1);    
    }

    wxImage* img = new wxImage(path);

    std::cout << "\nwxImageLoader::loadImage(): now loading: " << path << "." << std::endl;

    (*imageWidth)   = (size_t) img->GetWidth();
    (*imageHeight)  = (size_t) img->GetHeight();

    std::cout << "wxImageLoader::loadImage(): width, height: " << *imageWidth << ", " << *imageHeight << ".\n" << std::endl;

    size_t imageSize = img->GetWidth() * img->GetHeight() * kBytesPerPixel;

    std::vector<uint8_t> imageData;
    imageData.resize(imageSize);

    memset(imageData.data(), 0, imageSize);
    memcpy(imageData.data(), img->GetData(), imageSize);

    delete img;

    return imageData;
}

std::vector<uint8_t> matToImage(const Eigen::MatrixXf& matrix) {
    std::vector<uint8_t> image;
    image.resize(matrix.rows() * matrix.cols() * kBytesPerPixel);

    const float oldMin = matrix.minCoeff();
    const float oldMax = matrix.maxCoeff();
    const float newMin = 0.0;
    const float newMax = 255.0;

    std::cout << "oldMin, oldMax: " << oldMin << ", " << oldMax << std::endl;
    const float valueRange = (newMax - newMin) / (oldMax - oldMin);

    for (size_t i = 0; i < matrix.rows(); ++i) {
        for (size_t j = 0; j < matrix.cols(); ++j) {
            const size_t idx = (i * matrix.cols()  + j) * kBytesPerPixel;


            const float newVal = (matrix(i, j) - oldMin) * valueRange + newMin;

            image[idx + 0] = static_cast<uint8_t>(newVal);
            image[idx + 1] = static_cast<uint8_t>(newVal);
            image[idx + 2] = static_cast<uint8_t>(newVal);
        }
    }

    return image;
}


Eigen::MatrixXf computeEdgeMap(const Eigen::MatrixXf& I, const bool useConvolution) {
    Eigen::MatrixXf edgeMap(I.rows(), I.cols());

    Eigen::MatrixXf dx(3, 3);
    Eigen::MatrixXf dy(3, 3);

    dx(0, 0) = 1.0;     dx(0, 1) = 0.0;     dx(0, 2) = -1.0;
    dx(1, 0) = 2.0;     dx(1, 1) = 0.0;     dx(1, 2) = -2.0;
    dx(2, 0) = 1.0;     dx(2, 1) = 0.0;     dx(2, 2) = -1.0;

    dy(0, 0) = 1.0;     dy(0, 1) = 2.0;     dy(0, 2) = 1.0;
    dy(1, 0) = 0.0;     dy(1, 1) = 0.0;     dy(1, 2) = 0.0;
    dy(2, 0) = -1.0;    dy(2, 1) = -2.0;    dy(2, 2) = -1.0;

    Eigen::MatrixXf Gx;
    Eigen::MatrixXf Gy;
    
    if (useConvolution) {
        Gx = conv2d(I, dx);
        Gy = conv2d(I, dy);
    }
    else {
        Gx = I.rightCols(I.cols() - 1) - I.leftCols(I.cols() - 1);
        Gy = I.bottomRows(I.rows() - 1) - I.topRows(I.rows() - 1);
        Gx.conservativeResizeLike(Eigen::MatrixXf(I.rows(), I.cols()));
        Gy.conservativeResizeLike(Eigen::MatrixXf(I.rows(), I.cols()));
    }

    Gx = Gx.array().square();
    Gy = Gy.array().square();

    std::cout << "Gx rows, cols: " << Gx.rows() << ", " << Gx.cols() << std::endl;
    std::cout << "Gy rows, cols: " << Gy.rows() << ", " << Gy.cols() << std::endl;

    edgeMap = Gx + Gy;
    edgeMap = edgeMap.array().sqrt();
    
    return edgeMap;
}

template <typename Derived, typename Derived2>
Derived conv2d(const Eigen::MatrixBase<Derived>& I, const Eigen::MatrixBase<Derived2> &kernel) {
    Derived O = Derived::Zero(I.rows(), I.cols());


    typedef typename Derived::Scalar Scalar;

    Derived2 block;
    Scalar normalization = kernel.sum();
    
    if (normalization < 1E-6) {
        normalization = 1;
    } 

    Derived2 tempBlock1;
    Derived2 tempBlock2;
    
    const size_t kernelRows = kernel.rows();
    const size_t kernelCols = kernel.cols();

    const size_t rowUpperBound = I.rows() - kernelRows;
    const size_t colUpperBound = I.cols() - kernelCols;

    for (size_t i = kernelRows; i < rowUpperBound; i++) {
        for (size_t j = kernelCols; j < colUpperBound; j++ ) {
            tempBlock1 = (I.block(i, j, kernelRows, kernelCols));
            tempBlock2 = tempBlock1.cwiseProduct(kernel);
            Scalar b = tempBlock2.sum();
            //Scalar b = (static_cast<Derived2>(I.block(i, j, kernelRows, kernelCols)).cwiseProduct(kernel)).sum();
            O.coeffRef(i, j) = b;
        }
    }

    return O / normalization;
}
