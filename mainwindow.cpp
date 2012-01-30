#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QImage>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMutex>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	this->ui->mdiArea->setViewMode(QMdiArea::SubWindowView);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setNewPixmap(QImage *img)
{
	QLabel* l = _labels.value(img);
	QMdiSubWindow* msw = _subWindowsToImages.key(img);
	ImageController* ic = _controllers.value(img);
	QImage* newone = ic->image();
	_labels.remove(img);
	_labels.insert(newone, l);
	_subWindowsToImages.insert(msw, newone);
	_controllers.remove(img);
	_controllers.insert(newone, ic);
	delete img;
	l->setPixmap(QPixmap::fromImage(*newone));
}

void MainWindow::openImage(QImage* img)
{
	ImageController* ic = new ImageController();
	ic->setImage(img);
	ic->start();
	if(QApplication::instance()->thread() == ic->thread())
		ic->moveToThread(ic);
	connect(this, SIGNAL(sharpen(QImage*)), ic, SLOT(sharpenImage(QImage*)));
	connect(this, SIGNAL(highlite(QImage*)), ic, SLOT(brightenImage(QImage*)));
	connect(this, SIGNAL(segment(QImage*)), ic, SLOT(segmentImage(QImage*)));
	connect(ic, SIGNAL(resultImage(QImage*)), this, SLOT(setNewPixmap(QImage*)));
	connect(ic, SIGNAL(newImage(QImage*)), this, SLOT(openImage(QImage*)));
	connect(this, SIGNAL(findObject(QImage*)), ic, SLOT(segmentImage(QImage*)));
	connect(this, SIGNAL(detectEdges(QImage*)), ic, SLOT(detectEdges(QImage*)));
	_controllers.insert(img, ic);
//	QWidget *pic = new QWidget(this, Qt::Dialog);
	QLabel* l = new QLabel();
	l->setPixmap(QPixmap::fromImage(*img));
	_labels.insert(img, l);
	QMdiSubWindow *msw = this->ui->mdiArea->addSubWindow(l);
	if(msw == 0)
		qDebug() << "dupa";
	_subWindowsToImages.insert(msw, img);;
	l->activateWindow();
	l->raise();
	l->setGeometry(10, 10, img->width(), img->height());
	l->show();

}

void MainWindow::on_actionOtw_rz_obraz_triggered()
{
	QString file_name = QFileDialog::getOpenFileName(this, "Wybierz", QString(),
													 tr("Image Files (*.png *.jpg *.gif"));
	if(file_name == QString())
		return;
	QImage* img = new QImage(file_name);
	this->openImage(img);
}

void MainWindow::on_actionWyostrz_triggered()
{
	QMdiSubWindow *msw = this->ui->mdiArea->activeSubWindow();
	QImage* img = this->_subWindowsToImages.value(msw);
	emit sharpen(img);
}

void MainWindow::on_actionRozja_nij_triggered()
{
	QMdiSubWindow *msw = this->ui->mdiArea->activeSubWindow();
	QImage* img = this->_subWindowsToImages.value(msw);
	emit highlite(img);
}

void MainWindow::on_actionWykrywaj_triggered()
{
	QMdiSubWindow *msw = this->ui->mdiArea->activeSubWindow();
	QImage* img = this->_subWindowsToImages.value(msw);
	emit findObject(img);
}

void MainWindow::on_actionWykrywaj_kraw_dzie_triggered()
{
	QMdiSubWindow *msw = this->ui->mdiArea->activeSubWindow();
	QImage* img = this->_subWindowsToImages.value(msw);
	emit detectEdges(img);
}
