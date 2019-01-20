#****************************************************************************
# * HabiSoft, LLC
# ****************************************************************************
# * 
# * (c) [2018] - [present]
# * All Rights Reserved.
# * 
# * Limited License: Under no circumstance is commercial use, reproduction, or
# * distribution permitted. Use, reproduction, and distribution are permitted
# * solely for educational purposes.
# *
# * Any reproduction or distribution of source code must retain the above
# * copyright notice and the full text of this license including the Disclaimer,
# * below. 
# *
# * Any reproduction or distribution in binary form must reproduce the above
# * copyright notice and the full text of this license including the Disclaimer
# * below in the documentation and/or other materials provided with the Distribution.
# *
# * DISCLAIMER
# *
# * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
# * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
# * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
# * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
# * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# ****************************************************************************
# *
# * file: Makefile
# *
# * This class implements a wxWidget window with an OpenGL sub window 
# * nested in it. The purpose of this software is to implement a GUI for 
# * viewing and manipulating image data and performing active contour model
# * segmentation of the images within.
# *
# ****************************************************************************

C++ = g++

CPPFLAGS = `wx-config --cppflags` -I../Eigen/ -std=c++11 -O3
LIBS = -lGL -lGLU `wx-config --gl-libs` `wx-config --libs`

OBJS = DrawableImage.o ImageViewer.o

all: ImageViewer

ImageViewer: $(OBJS)
	$(C++) $(OBJS) -o ImageViewer $(CPPFLAGS) $(LIBS)

#Image.o: Image.cpp
#	$(C++) $(CPPFLAGS) -c Image.cpp

DrawableImage.o: DrawableImage.cpp
	$(C++) $(CPPFLAGS) -c DrawableImage.cpp

ImageViewer.o: ImageViewer.cpp
	$(C++) $(CPPFLAGS) -c ImageViewer.cpp

run:
	./ImageViewer

clean:
	rm -rf *.o ImageViewer
