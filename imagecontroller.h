#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H

#include <QThread>
#include <QImage>
#include <QList>
#include <utility>

enum SMALL_M
{
	_00=0,
	_01,
	_10,
	_11,
	_20,
	_02
};

enum BIG_M
{
	_b20=0,
	_b02,
	_b11,
	_1,
	_2,
	_7
};

enum ObjectType
{
	NONE=-1,
	SWORD=0,
	LHAND=1,
	HEAD=2,
	SHIELD=3,
	TAIL=4,
	BREST=5
};

enum ObjectTypeFlag
{
	NONEF=0,
	SWORDF=1,
	LHANDF=2,
	HEADF=4,
	SHIELDF=8,
	TAILF=16,
	BRESTF=32
};

extern const int small_m_size;
extern const int big_m_size;

/**
  sword: r 0:81; g 5:142; b 43:223\
((red >= 0 && red < 90) &&
					(green >= 5 && green < 145) &&
					(blue > 40 && blue < 230))
M1 =  0.602643
M2 =  0.316748
M7 =  0.0116079
pmax:  42   14
pmin:  1   6

lhand:
((red >= 7 && red < 115) &&
					(green >= 22 && green < 135) &&
					(blue > 24 && blue < 150))
minrgb:  7   22   24
maxrgb:  108   127   143
M1 =  0.580131
M2 =  0.213463
M7 =  0.0307723
pmax:  11   22
pmin:  3   12

head:
((red >= 12 && red <= 255) &&
					(green >= 1 && green < 250) &&
					(blue > 0 && blue < 202))
M1 =  0.317735
M2 =  0.0548318
M7 =  0.0115309
minrgb:  12   1   0
maxrgb:  255   250   202
pmax:  4   27
pmin:  3   26

shield:
((red >= 35 && red <= 240) &&
					(green >= 0 && green < 125) &&
					(blue >= 0 && blue < 115))
M1 =  0.167314
M2 =  0.0000819674
M7 =  0.00697801
minrgb:  37   0   0
maxrgb:  231   112   108
pmax:  20   33
pmin:  2   2

tail:
((red >= 0 && red <= 200) &&
					(green >= 14 && green <= 255) &&
					(blue >= 32 && blue <= 255))
M1 =  0.324467
M2 =  0.0267208
M7 =  0.0196395
minrgb:  0   14   32
maxrgb:  194   255   255
pmax:  37   76
pmin:  4   18

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


*/

/**
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
hue: 39+-2; 60;
minrgb:  12   1   0
maxrgb:  255   250   202
pmax:  4   27
pmin:  3   26

shield:
M1 =  0.185475
M2 =  0.000151462
M7 =  0.00856237
hue: 2 +- 2;
minrgb:  37   0   0
maxrgb:  231   112   108
pmax:  20   33
pmin:  2   2


tail:
M1 =  0.391901
M2 =  0.0269255
M7 =  0.0316652
hue: 203 +- 2; 180; 300
minrgb:  0   14   32
maxrgb:  194   255   255
pmax:  37   76
pmin:  4   18

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


*/

class ImageController : public QThread
{
    Q_OBJECT
public:
    explicit ImageController(QObject *parent = 0);

	inline QImage* image() { return _img; }

	static bool inRange(int testValue, int templateValue, unsigned delta)
	{
		int d = (int)delta;
		int lower = templateValue - d < 0 ? (templateValue - d)%361 : (templateValue - d);
		int upper = templateValue + d > 360 ? (templateValue + d)%361 : (templateValue + d);
		return (testValue >= lower && testValue <= upper);
	}

signals:

	void resultImage(QImage* img);
	void newImage(QImage* img);

public slots:

	void setImage(QImage* img);
	void sharpenImage(QImage* img);
	void detectEdges(QImage* img);
	void brightenImage(QImage* img);
	void segmentImage(QImage* img);
	void computeFactors();

protected:

	QImage* _img;
	QList<QList<QPoint> > _objects;
	QList<QImage* > _old;
	static int HISTOGRAM;
	int _small_m[6];
	float _big_m[6];
	float _m1[6];
	float _m2[6];
	float _m7[6];

	float _templateM1[6];
	float _templateM2[6];
	float _templateM7[6];

	QList<QList<QPoint> > tails;
	QList<QList<QPoint> > heads;
	QList<QList<QPoint> > shields;


	virtual void run();

	void getStep();
	void setUpBorders(int width, int height);
	void lookupObjects(int width, int height);
	int getMinX(QList<QPoint> object);
	int getMinY(QList<QPoint> object);
	int getMaxX(QList<QPoint> object);
	int getMaxY(QList<QPoint> object);
	void joinObjects();
	void seed(int x, int y);
	void drawImage(QList<QPoint> object);
	void drawObjects();
	static ObjectType color_test(int hue, int value, int saturation);

};

#endif // IMAGECONTROLLER_H
