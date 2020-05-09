/*
Copyright (c) 2014, Yasuhiro Hasegawa
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
  3. The name of the author may not be used to endorse or promote products derived
     from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/


//
// GuiOp.h
//

#ifndef GUIOP_H__
#define GUIOP_H__

#include "Util.h"

class GuiOp {
public:
	class CursorPattern {
	public:
		CursorPattern() {}
		CursorPattern(const CursorPattern* that);

		quint16 data[16];
	};

	class Rect {
	public:
		Rect() : x(0), y(0), width(0), height(0) {}
		Rect(int x_, int y_, int width_, int height_) : x(x_), y(y_), width(width_), height(height_) {}
		Rect(const Rect& that) : x(that.x), y(that.y), width(that.width), height(that.height) {}

		Rect& operator=(const Rect& that) {
			this->x      = that.x;
			this->y      = that.y;
			this->width  = that.width;
			this->height = that.height;

			return *this;
		}

		int x, y, width, height;
	};

	static void setCursorPattern(CursorPattern* data);

	static void updateDisplay(Rect* rect);

	static void setMP(quint16 newValue);

	static void setContext(GuiOp *newValue) {
		guiOp = newValue;
	}

protected:
	virtual void setCursorPatternImpl(CursorPattern* data) = 0;
	virtual void updateDisplayImpl   (Rect* rect) = 0;
	virtual void setMPImpl           (quint16 newValue) = 0;
	//
	virtual ~GuiOp() {}

private:
	static GuiOp *guiOp;
};

class NullGuiOp : public GuiOp {
	void setCursorPatternImpl(CursorPattern*) {}
	void updateDisplayImpl   (Rect*) {}
	void setMPImpl           (quint16) {}
};

#endif
