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
 * file: Timer.hpp
 *
 * The following implements a timer class for simple profiling.
 *
 ****************************************************************************
 */

#ifndef TIMER_HPP
#define TIMER_HPP

#include <iostream>
#include <sstream>
#include <sys/time.h>

class Timer {
private:
   struct timeval m_startTime;
   struct timeval  m_endTime;
   
public:
   Timer() { }
   ~Timer() { }

   void tick() {
       gettimeofday(&m_startTime, NULL);
   }
   
   double tock(){
      gettimeofday(&m_endTime, NULL);
      m_endTime.tv_sec -= m_startTime.tv_sec;
      m_endTime.tv_usec -= m_startTime.tv_usec;

      return (m_endTime.tv_sec + (m_endTime.tv_usec / 1000000.0)) * 1000.0;
   }
};

#endif
