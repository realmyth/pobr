#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QDebug>

/**
brest:
((red >= 2 && red < 125) &&
					(green >= 28 && green < 140) &&
					(blue >= 37 && blue < 170))
M1 =  0.45242
M2 =  0.0701238
M7 =  0.03364
minrgb:  2   28   37
maxrgb:  116   131   160
pmax:  16   12
pmin:  3   3

  sword: r 0:81; g 5:142; b 43:223\
M1 =  0.341356
M2 =  0.0456053
M7 =  0.0177296
pmax:  42   14
pmin:  1   6

lhand:
minrgb:  7   22   24
maxrgb:  108   127   143
M1 =  0.651288
M2 =  0.226032
M7 =  0.049536
pmax:  11   22
pmin:  3   12
hue: 203 +- 4

head:
M1 =  0.353779
M2 =  0.0631976
M7 =  0.0154905
minrgb:  12   1   0
maxrgb:  255   250   202
pmax:  4   27
pmin:  3   26
hue: 40+-5; 60+-1;

shield:
M1 =  0.185475
M2 =  0.000151462
M7 =  0.00856237
minrgb:  37   0   0
maxrgb:  231   112   108
pmax:  20   33
pmin:  2   2
hue: 2 +- 2; 5+-5; 44+-1; 341+-2; 349+-2

tail:
M1 =  0.391901
M2 =  0.0269255
M7 =  0.0316652
minrgb:  0   14   32
maxrgb:  194   255   255
pmax:  37   76
pmin:  4   18
hue: 203 +- 2; 180;

*/

bool inRange(int testValue, int templateValue, unsigned delta)
{
	int d = (int)delta;
	return (testValue >= templateValue - d && testValue <= templateValue + d);
}

void analyzeImg(QImage *img)
{
	if(img == 0 || img->isNull())
	{
		qDebug() << "Null image";
		return;
	}
	float _big_m[6] = {0,0,0,0,0,0};
	int _small_m[6] = {0,0,0,0,0,0};
	QPoint max(0, 0), min(99999999, 99999999);
	int vmax=0, vmin=255;
	QMap<int, int> colors, hues;
	for (int x = 0; x < img->width(); x++)
	{
		for(int y=0; y < img->height(); ++y)
		{
			QRgb tmp = img->pixel(x,y);
			QColor color = QColor::fromRgb(tmp);
			if(!colors.contains(color.value()))
				colors.insert(color.value(), 1);
			else
				colors[color.value()] += 1;
			if(color.value() != 0 && color.hue() != -1)
			{
				if(!hues.contains(color.hue()))
					hues.insert(color.hue(), 1);
				else
					hues[color.hue()] += 1;
				if (inRange(color.hue(), 40, 5) || inRange(color.hue(), 60, 1))
				{
					_small_m[_00] += 1;
					_small_m[_10] += x;
					_small_m[_01] += y;
					_small_m[_11] += x * y;
					_small_m[_20] += x * x;
					_small_m[_02] += y * y;
				}
			}
		 }
	}
	_big_m[_b20] = (float)_small_m[_20] -
			((float)_small_m[_10] * (float)_small_m[_10]) /
			(float)_small_m[_00];
	_big_m[_b02] = (float)_small_m[_02] -
			((float)_small_m[_01] * (float)_small_m[_01]) /
			(float)_small_m[_00];
	_big_m[_b11] = (float)_small_m[_11] -
			((float)_small_m[_10] *(float) _small_m[_01]) /
			(float)_small_m[_00];
	_big_m[_1] = (_big_m[_b20] + _big_m[_b02]) / (float)((float)_small_m[_00] * (float)_small_m[_00]);
	_big_m[_2] = ((_big_m[_b20] - _big_m[_b02])* (_big_m[_b20] - _big_m[_b02]) +
				 4*_big_m[_11]*_big_m[_11])/ (float)((float)_small_m[_00]*(float)_small_m[_00]*
																		   (float)_small_m[_00]*
																		   (float)_small_m[_00]);
	_big_m[_7] = (_big_m[_b20] * _big_m[_b02] -
				  _big_m[_11] * _big_m[_11]) / (float)((float)_small_m[_00] * (float)_small_m[_00] *
													   (float) _small_m[_00] * (float)_small_m[_00]);
	qDebug() << "M1 = " << _big_m[_1];
	qDebug() << "M2 = " << _big_m[_2];
	qDebug() << "M7 = " << _big_m[_7];
	qDebug() << "pmax: " << max.x() << " " << max.y();
	qDebug() << "pmin: " << min.x() << " " << min.y();
//	for(QMap<int, int>::Iterator ii = colors.begin(); ii != colors.end(); ++ii)
//	{
//		if(ii.value() > 5)
//			qDebug() << ii.key() << " " << ii.value();
//	}
	for(QMap<int, int>::Iterator ii = hues.begin(); ii != hues.end(); ++ii)
	{
		if(ii.value() > 5)
			qDebug() << ii.key() << " " << ii.value();
	}
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//	QImage *i = new QImage("C:\\Users\\zby\\Desktop\\DOCS\\POBR\\head.png");
//	analyzeImg(i);
	MainWindow w;
	w.show();

	return a.exec();
//	return 0;
}
