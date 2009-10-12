/****************************************************************************
 *      qtoutput.cc: a Qt color output for yafray
 *      This is part of the yafray package
 *      Copyright (C) 2008 Gustavo Pichorim Boiko
 *
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2.1 of the License, or (at your option) any later version.
 *
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 *
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library; if not, write to the Free Software
 *      Foundation,Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "qtoutput.h"
#include "events.h"
#include <QtCore/QCoreApplication>
#include <iostream>
#include <cstdlib>

QtOutput::QtOutput(RenderWidget *render): widgy(render)
{
}

void QtOutput::setRenderSize(const QSize &s)
{
	img = QImage(s, QImage::Format_ARGB32);
	img.fill(0);
	// calling the alphaChannel() function will return a 8-bit indexed 
	// greyscale image
	widgy->alphaChannel = img.alphaChannel();
	widgy->resize(s);

	QPalette palette;
	palette.setColor(QPalette::Background, QColor(0, 0, 0));
	widgy->setPalette(palette);
	// force pixmap creation
	flush();
}

void QtOutput::clear()
{
	// clear image...
	img = QImage();
	widgy->alphaChannel = QImage();
	flush();
}

/*====================================
/ colorOutput_t implementations
=====================================*/

bool QtOutput::putPixel(int x, int y, const float *c, int channels)
{
	int r = std::max(0,std::min(255, (int)(c[0] * 255)));
	int g = std::max(0,std::min(255, (int)(c[1] * 255)));	
	int b = std::max(0,std::min(255, (int)(c[2] * 255)));
	int a = 255;

	QRgb rgb = qRgb(r, g, b);
	
	if (channels > 3) a = std::max(0,std::min(255, (int)(c[3] * 255)));

	img.setPixel(x + widgy->borderStart.x(),y + widgy->borderStart.y(), rgb);
	widgy->alphaChannel.bits()[widgy->alphaChannel.bytesPerLine() * (y + widgy->borderStart.y()) + x + widgy->borderStart.x()] = a;

	return true;
}

void QtOutput::flush()
{
	QImage tmp = img.copy();
	QCoreApplication::postEvent(widgy, new GuiUpdateEvent(QRect(), tmp, true));
}

void QtOutput::flushArea(int x0, int y0, int x1, int y1)
{
	// as the tile is finished, it looks like it is safe to use the image instead of
	// copying the tiles for updating
	QCoreApplication::postEvent(widgy, new GuiUpdateEvent(QRect(x0,y0,x1-x0,y1-y0), img));
}

void QtOutput::highliteArea(int x0, int y0, int x1, int y1)
{
	// as the tile is finished, it looks like it is safe to use the image instead of
	// copying the tiles for updating
	QCoreApplication::postEvent(widgy, new GuiAreaHighliteEvent(QRect(x0,y0,x1-x0,y1-y0), img));
}
