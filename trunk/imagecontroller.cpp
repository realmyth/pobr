#include "imagecontroller.h"
#include "libs/convolutionfilter.h"
#include "libs/qtimagefilterfactory.h"
#include <QDebug>
#include <QColor>

const int small_m_size = 6;
const int big_m_size=6;
int ImageController::HISTOGRAM = 15000;

/**

sword:
M1 =  0.341356
M2 =  0.0456053
M7 =  0.0177296
pmax:  42   14
pmin:  1   6

lhand:
M1 =  0.580131
M2 =  0.213463
M7 =  0.0307723
pmax:  11   22
pmin:  3   12

head:
M1 =  0.353779
M2 =  0.0631976
M7 =  0.0154905
hue: 40+-5; 60+-1;

shield:
M1 =  0.185475
M2 =  0.000151462
M7 =  0.00856237
hue: 2 +- 3; 5+-5; 44+-1; 341+-2; 349+-2

tail:
M1 =  0.391901
M2 =  0.0269255
M7 =  0.0316652
hue: 203 +- 2; 180; 300


*/

ImageController::ImageController(QObject *parent) :
    QThread(parent)
{
	this->_img = 0;

	for(int i=0; i < small_m_size; ++i)
		_small_m[i] = 0;
	for(int i=0; i < big_m_size; ++i)
		_big_m[i] = 0;
	_templateM1[SWORD] = 0.341356;
	_templateM1[LHAND] = 0.580131;
	_templateM1[HEAD] = 0.353779;
	_templateM1[SHIELD] = 0.185475;
	_templateM1[TAIL] = 0.391901;
	_templateM2[SWORD] = 0.316748;
	_templateM2[LHAND] = 0.213463;
	_templateM2[HEAD] = 0.0631976;
	_templateM2[SHIELD] = 0.000151462;
	_templateM2[TAIL] = 0.0269255;
	_templateM7[SWORD] = 0.0116079;
	_templateM7[LHAND] = 0.0307723;
	_templateM7[HEAD] = 0.0154905;
	_templateM7[SHIELD] = 0.00856237;
	_templateM7[TAIL] = 0.0316652;
}

void ImageController::setImage(QImage* img)
{
	this->_img = img;
}

void ImageController::sharpenImage(QImage* img)
{
	if(img != _img)
		return;
	QtImageFilter *img_f = createSharpenFilter();
	QRect rect = this->_img->rect();
	QImage* app_img = new QImage(img_f->apply(*this->_img, rect));
	_old.push_back(_img);
	if(app_img == img)
		qDebug() << "sa takie same";
	else
		qDebug() << " sa rozne";
	_img = app_img;
	emit resultImage(_old.at(_old.size()-1));
}

void ImageController::detectEdges(QImage* img)
{
	if(img != _img)
		return;
	QtImageFilter *img_f = createEdgeDetectFilter();
	QRect rect = this->_img->rect();
	QImage* app_img = new QImage(img_f->apply(*this->_img, rect));
	_old.push_back(_img);
	if(app_img == img)
		qDebug() << "sa takie same";
	else
		qDebug() << " sa rozne";
	_img = app_img;
	emit resultImage(_old.at(_old.size()-1));
}

void ImageController::brightenImage(QImage* img)
{
	if(img != _img)
		return;
	QtImageFilter *img_f = createHighlightFilter();
	QRect rect = this->_img->rect();
	QImage* app_img = new QImage(img_f->apply(*this->_img, rect));
	_old.push_back(_img);
	if(app_img == img)
		qDebug() << "sa takie same";
	else
		qDebug() << " sa rozne";
	_img = app_img;
	emit resultImage(_old.at(_old.size()-1));
}

void ImageController::segmentImage(QImage* img)
{
	if(img == _img)
		this->computeFactors();
}

void ImageController::run()
{
	if(this->_img == 0)
		return;
	this->exec();
}

void ImageController::getStep()
{
	int* histogram = new int[256];
	int* redHistogram = new int[256];
	int* greenHistogram = new int[256];
	int* blueHistogram = new int[256];
	QRgb tmp;
	int amount = 0;
	for(int i =0; i < 256; ++i)
	{
		histogram[i] = 0;
		redHistogram[i] = 0;
		greenHistogram[i] = 0;
		blueHistogram[i] = 0;
	}
	for (int x = 0; x < _img->width(); x++)
	{
		for (int y = 0; y < _img->height(); y++)
		{
			tmp = _img->pixel(x, y);
			int red = qRed(tmp);
			int green = qGreen(tmp);
			int blue = qBlue(tmp);
			int gray = red + green + blue;
			gray /= 3;
			if (gray < 222 && gray > 20)
			{
				amount++;
				histogram[gray]++;
				redHistogram[red]++;
				greenHistogram[green]++;
				blueHistogram[blue]++;
			}
		}
	}
//	logHistogram(histogram, "histogram_raw");
	for (int i = 0; i < 256; i++)
	{
		double t = ((histogram[i]*HISTOGRAM) / (amount));
		histogram[i] = (int)t;
		t = ((redHistogram[i]*HISTOGRAM) / (amount));
		redHistogram[i] = (int)t;
		t = ((greenHistogram[i]*HISTOGRAM) / (amount));
		greenHistogram[i] = (int)t;
		t = ((blueHistogram[i]*HISTOGRAM) / (amount));
		blueHistogram[i] = (int)t;
	}
}

void ImageController::setUpBorders(int width, int height)
{
	QRgb tmp = qRgb(255, 255, 255);
	for (int x = 0; x < width; x++)
	{
		_img->setPixel(x, 0, tmp);
		_img->setPixel(x, height - 1, tmp);
	}
	for (int y = 0; y < height; y++)
	{
		_img->setPixel(0, y, tmp);
		_img->setPixel(width - 1, y, tmp);
	}
}

void ImageController::lookupObjects(int width, int height)
{
	QRgb tmp = qRgb(255, 255, 255);
	for (int x = 1; x < width - 1; x++)
	{
		for (int y = 1; y < height - 1; y++)
		{
			tmp = _img->pixel(x, y);
			QColor color(tmp);
			if (color_test(color.hue(), color.value(), color.saturation()) != NONE)
				seed(x, y);
//			else
//			{
//				qDebug() << x << ", " << y << "; " << color.hue() << ", "
//						 << color.saturation() << ", " << color.value();
//			}
		}
		if (x % 50 == 0)
			joinObjects();
	}
}

void ImageController::computeFactors()
{
	_objects.clear();
	int width = _img->width();
	int height = _img->height();
	this->setUpBorders(width, height);
	this->lookupObjects(width, height);
	this->joinObjects();
	this->drawObjects();
	qDebug() << "object.size: " << _objects.size() << endl;
	for (int j = 0; j < _objects.size(); j++)
	{
		 if (_objects.at(j).size()> 10)
		 {
				int A = _objects.at(j).size();
				_small_m[_00] = A;
				for (int i = 0; i < A; i++)
				{
					_small_m[_10] += (int)_objects.at(j).at(i).x();
					_small_m[_01] += (int)_objects.at(j).at(i).y();
					_small_m[_11] += (int)_objects.at(j).at(i).x() * (int)_objects.at(j).at(i).y();
					_small_m[_20] += (int)_objects.at(j).at(i).x() * (int)_objects.at(j).at(i).x();
					_small_m[_02] += (int)_objects.at(j).at(i).y() * (int)_objects.at(j).at(i).y();
				}
				_big_m[_b20] = _small_m[_20] -
						(_small_m[_10] * _small_m[_10]) /
						_small_m[_00];
				_big_m[_b02] = _small_m[_02] -
						(_small_m[_01] * _small_m[_01]) /
						_small_m[_00];
				_big_m[_b11] = _small_m[_11] -
						(_small_m[_10] * _small_m[_01]) /
						_small_m[_00];
				_big_m[_1] = (_big_m[_b20] + _big_m[_b02]) /
						(_small_m[_00] * _small_m[_00]);
				_big_m[_2] = ((_big_m[_b20] - _big_m[_b02])*
							  (_big_m[_b20] - _big_m[_b02]) +
							  4*_big_m[_11]*_big_m[_11])/
						(_small_m[_00]*_small_m[_00]*
						 _small_m[_00]*_small_m[_00]);
				_big_m[_7] = (_big_m[_b20] * _big_m[_b02] -
							  _big_m[_11] * _big_m[_11]) /
						(_small_m[_00] * _small_m[_00] *
						 _small_m[_00] * _small_m[_00]);
				qDebug() << "M1 = " << _big_m[_1];
				qDebug() << "M2 = " << _big_m[_2];
				qDebug() << "M7 = " << _big_m[_7];
				if (((abs(_big_m[_1] - _templateM1[HEAD]) / _templateM1[HEAD])
						* 100 <= 20) &&
						((abs(_big_m[_2] - _templateM2[HEAD]) / _templateM2[HEAD])
												* 100 <= 20) &&
						((abs(_big_m[_7] - _templateM7[HEAD]) / _templateM7[HEAD])
												* 100 <= 20))
				{
					heads.push_back(_objects.at(j));
				}
				else if (((abs(_big_m[_1] - _templateM1[TAIL]) / _templateM1[TAIL])
						  * 100 <= 20) &&
						  ((abs(_big_m[_2] - _templateM2[TAIL]) / _templateM2[TAIL])
												  * 100 <= 20) &&
						  ((abs(_big_m[_7] - _templateM7[TAIL]) / _templateM7[TAIL])
												  * 100 <= 20))
				{
					tails.push_back(_objects.at(j));
				}
				else if (((abs(_big_m[_1] - _templateM1[SHIELD]) / _templateM1[SHIELD])
						  * 100 <= 20) &&
						  ((abs(_big_m[_2] - _templateM2[SHIELD]) / _templateM2[SHIELD])
												  * 100 <= 20) &&
						  ((abs(_big_m[_7] - _templateM7[SHIELD]) / _templateM7[SHIELD])
												  * 100 <= 20))
				{
					shields.push_back(_objects.at(j));
				}
		 }
	}

	this->drawObjects();

}


int ImageController::getMinX(QList<QPoint> object)
{
	int coord = 999999999;
	for (int i = 0; i < object.size(); i++)
	{
		if (coord > (int)object.at(i).x())
			coord = (int)object.at(i).x();
	}
	return coord;
}

int ImageController::getMinY(QList<QPoint> object)
{
	int coord = 999999999;
	for (int i = 0; i < object.size(); i++)
	{
		if (coord > (int)object.at(i).y())
			coord = (int)object.at(i).y();
	}
	return coord;
}

int ImageController::getMaxX(QList<QPoint> object)
{
	int coord = 0;
	for (int i = 0; i < object.size(); i++)
	{
		if (coord < (int)object.at(i).x())
			coord = (int)object.at(i).x();
	}
	return coord;
}

int ImageController::getMaxY(QList<QPoint> object)
{
	int coord = 0;
	for (int i = 0; i < object.size(); i++)
	{
		if (coord < (int)object.at(i).y())
			coord = (int)object.at(i).y();
	}
	return coord;
}

void ImageController::joinObjects()
{
	for(int i = 0; i < _objects.size() - 1; i ++)
	{
		for(int j = i+1; j < _objects.size(); j++)
		{
			for (int k = 0; k < _objects.at(i).size(); k++)
			{
				int x =(int)_objects.at(i).at(k).x();
				int y = (int)_objects.at(i).at(k).y();
				if(_objects.at(j).contains(QPoint(x-1, y-1)) ||
						_objects.at(j).contains(QPoint(x, y-1)) ||
						_objects.at(j).contains(QPoint(x-1, y)) ||
						_objects.at(j).contains(QPoint(x+1, y+1)) ||
						_objects.at(j).contains(QPoint(x, y+1)) ||
						_objects.at(j).contains(QPoint(x+1, y)))
				{ // neighbour found
					for (int l = 0; l < _objects.at(j).size(); l++)
					{
						if (!_objects.at(i).contains(_objects.at(j).at(l)))
							_objects[i].push_back(_objects.at(j).at(l));
					}
					_objects.removeAt(j);
					j--;
					break;
				}
			}
		}
	}

}

//void ImageController::joinObjects()
//{
//	for(int i = 0; i < _objects[o].size() - 1; i ++)
//	{
//		for(int j = i+1; j < _objects[o].size(); j++)
//		{
//			for (int k = 0; k < _objects[o].at(i).size(); k++)
//			{
//				int x =(int)_objects[o].at(i).at(k).x();
//				int y = (int)_objects[o].at(i).at(k).y();
//				if(_objects[o].at(j).contains(QPoint(x-1, y-1)))
//				{ // neighbour found
//					for (int l = 0; l < _objects[o].at(j).size(); l++)
//					{
//						if (!_objects[o].at(i).contains(_objects[o].at(j).at(l)))
//						{
//							QList<QPoint> ltmp = _objects[o].at(i);
//							ltmp.push_back(_objects[o].at(j).at(l));
//						}
//					}
//					_objects[o].removeAt(j);
//					j--;
//					break;
//				}
//			}
//		}
//	}

//}

void ImageController::seed(int x, int y)
{
	QColor tmp;
	tmp.setHsv(0, 255, 128);
	QRgb fromTmp;
	fromTmp = qRgb(tmp.red(), tmp.green(), tmp.blue());
	_img->setPixel(x, y, fromTmp);
	if (_objects.size() != 0)
	{
		int i = 0;
		while (i < _objects.size())
		{
			if (_objects.at(i).contains(QPoint(x-1,y-1)) ||
					_objects.at(i).contains(QPoint(x,y-1))
					|| _objects.at(i).contains(QPoint(x-1,y)) ||
					_objects.at(i).contains(QPoint(x-1,y+1)))
			{
				_objects[i].push_back(QPoint(x,y));
				break;
			}
			i++;
		}
		if (i == _objects.size()) // new shape found
		{
			QList<QPoint> newObject;
			newObject.push_back(QPoint(x,y));
			_objects.push_back(newObject);
		}
	}
	else
	{
		QList<QPoint> newObject;
		newObject.push_back(QPoint(x,y));
		_objects.push_back(newObject);
	}
}

void ImageController::drawImage(QList<QPoint> object)
{
	QImage* d = new QImage(_img->width(), _img->height(), QImage::Format_RGB32);
	QRgb tmp = qRgb(0,0,0);
	foreach(QPoint p, object)
		d->setPixel(p, tmp);
	emit newImage(d);
}

void ImageController::drawObjects()
{
	QImage* d = new QImage(_img->width(), _img->height(), QImage::Format_RGB32);
	QRgb tmp = qRgb(200,0,0);
	QRgb bck = qRgb(2,2,2);
	for(int x=0; x<d->width(); ++x)
	{
		for(int y=0; y<d->height(); ++y)
			d->setPixel(x, y, bck);

	}
	foreach(QList<QPoint> list, _objects)
	{
		foreach(QPoint p, list)
			d->setPixel(p, tmp);
	}
	tmp = qRgb(0,0,200);
	foreach(QList<QPoint> list, tails)
	{
		foreach(QPoint p, list)
			d->setPixel(p, tmp);
	}
	tmp = qRgb(0,200,0);
	foreach(QList<QPoint> list, heads)
	{
		foreach(QPoint p, list)
			d->setPixel(p, tmp);
	}
	tmp = qRgb(255,255,0);
	foreach(QList<QPoint> list, shields)
	{
		foreach(QPoint p, list)
			d->setPixel(p, tmp);
	}

	emit newImage(d);
}

//ObjectType ImageController::color_test(int red, int green, int blue)
//{
//	if ((red >= 0 && red < 90) &&
//		(green >= 5 && green < 145) &&
//		(blue > 40 && blue < 230))
//		return SWORD;
//	if ((red >= 7 && red < 115) &&
//			(green >= 22 && green < 135) &&
//			(blue > 24 && blue < 150))
//		return LHAND;
//	if ((red >= 12 && red <= 255) &&
//			(green >= 1 && green < 250) &&
//			(blue > 0 && blue < 202))
//		return HEAD;
//	if ((red >= 35 && red <= 240) &&
//			(green >= 0 && green < 125) &&
//			(blue >= 0 && blue < 115))
//		return SHIELD;
//	if ((red >= 0 && red <= 200) &&
//			(green >= 14 && green <= 255) &&
//			(blue >= 32 && blue <= 255))
//		return TAIL;
//	if ((red >= 2 && red < 125) &&
//			(green >= 28 && green < 140) &&
//			(blue >= 37 && blue < 170))
//		return BREST;
//	return NONE;
//}
/**

head:
M1 =  0.353779
M2 =  0.0631976
M7 =  0.0154905
hue: 40+-5; 60+-1;

shield:
M1 =  0.185475
M2 =  0.000151462
M7 =  0.00856237
hue: 5+-5; 44+-1; 341+-2; 349+-2

tail:
M1 =  0.391901
M2 =  0.0269255
M7 =  0.0316652
hue: 203 +- 2; 180; 300


  */

ObjectType ImageController::color_test(int hue, int value, int saturation)
{
	if (hue >=190 && hue <=210 && saturation > 125)
		return TAIL;
	else if((hue >=350 || (hue >= 0 && hue <= 10)) && saturation > 125)
		return SHIELD;
	else if((hue >= 30 && hue <= 60) && saturation > 125)
		return HEAD;
	else
		return NONE;
}

