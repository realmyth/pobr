#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QHash>
#include <QMdiSubWindow>
#include <QLabel>
#include "imagecontroller.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

	void sharpen(QImage* img);
	void highlite(QImage* img);
	void segment(QImage* img);
	void findObject(QImage* img);
	void detectEdges(QImage* img);

public slots:

	void setNewPixmap(QImage *img);

private:
    Ui::MainWindow *ui;

private slots:

	void openImage(QImage* img);
	void on_actionOtw_rz_obraz_triggered();

	void on_actionWyostrz_triggered();

	void on_actionRozja_nij_triggered();

	void on_actionWykrywaj_triggered();

	void on_actionWykrywaj_kraw_dzie_triggered();

protected:

	QHash<QImage* , ImageController*> _controllers;
	QHash<QMdiSubWindow*, QImage*> _subWindowsToImages;
	QHash<QImage*, QLabel* > _labels;
};

#endif // MAINWINDOW_H
