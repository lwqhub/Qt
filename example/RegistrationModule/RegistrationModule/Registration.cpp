#include "Registration.h"
#include <QFileDialog>
#include <QMessageBox>
#include <pcl/io/ply_io.h>
#include <pcl/console/time.h>

Registration::Registration(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton_import, &QPushButton::clicked, [=] { this->importCloud(); });
	connect(ui.pushButton_ICPStart, &QPushButton::clicked, [=] { this->startICP(); });
	connect(ui.pushButton_nextITR, &QPushButton::clicked, [=] { this->nextITR(); });
	viewerThread = nullptr;
}

bool Registration::importPLYFile()
{
	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(tr("Choose a .ply FIle"));
	fileDialog->setDirectory(".");
	fileDialog->setNameFilter(tr("Model(*.ply)"));
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setViewMode(QFileDialog::Detail);
	QStringList fileName;
	if (fileDialog->exec())
	{
		fileName = fileDialog->selectedFiles();
	}
	if (fileName.count() == 1)
	{
		PointCloudT::Ptr cloud(new PointCloudT);
		std::string str = fileName[0].toStdString();
		if (pcl::io::loadPLYFile(str, *cloud) < 0)
		{
			QMessageBox::information(this, "Message", "Fail to import a .ply File");
			return false;
		}
		else
		{
			QFileInfo fileinfo;
			QString file_name;
			fileinfo = QFileInfo(fileName[0]);
			file_name = fileinfo.fileName();
			clouds.insert(std::pair<QString, PointCloudT::Ptr>(file_name,cloud));
			QMessageBox::information(this, "Message", "Having imported a .ply File Successfully");
			return true;
		}
	}
	else
	{
		QMessageBox::information(this, "Message", "Choose a .ply File");
		return false;
	}
}

void Registration::setDefaultParameters()
{
	settings.insert(std::pair<QString, QString>(ui.label_fixedCloud->text(), ui.comboBox_fixedCloud->currentText()));
	settings.insert(std::pair<QString, QString>(ui.label_movingCloud->text(), ui.comboBox_movingCloud->currentText()));
	settings.insert(std::pair<QString, QString>(ui.label_InitialITR->text(), ui.spinBox_initialIterations->text()));
	settings.insert(std::pair<QString, QString>(ui.label_NextITR->text(), ui.spinBox_nextIteration->text()));
}

void Registration::updateParameters()
{
	settings[ui.label_fixedCloud->text()] = ui.comboBox_fixedCloud->currentText();
	settings[ui.label_movingCloud->text()] = ui.comboBox_movingCloud->currentText();
	settings[ui.label_InitialITR->text()] = ui.spinBox_initialIterations->text();
	settings[ui.label_NextITR->text()] = ui.spinBox_nextIteration->text();
}

void Registration::print4x4Matrix(const Eigen::Matrix4d & matrix)
{
	ui.textBrowser_R->clear();
	QString str;
	str = QString("    | %1 %2 %3 |").arg(matrix(0, 0), 0, 'e', 2).arg(matrix(1, 0), 0, 'e', 2).arg(matrix(2, 0), 0, 'e', 2);
	ui.textBrowser_R->append(str);
	str = QString("R = | %1 %2 %3 |").arg(matrix(0, 1), 0, 'e', 2).arg(matrix(1, 1), 0, 'e', 2).arg(matrix(2, 1), 0, 'e', 2);
	ui.textBrowser_R->append(str);
	str = QString("    | %1 %2 %3 |").arg(matrix(0, 2), 0, 'e', 2).arg(matrix(1, 2), 0, 'e', 2).arg(matrix(2, 2), 0, 'e', 2);
	ui.textBrowser_R->append(str);
	str = QString("T = | %1 %2 %3 |").arg(matrix(0, 3), 0, 'e', 2).arg(matrix(1, 3), 0, 'e', 2).arg(matrix(2, 3), 0, 'e', 2);
	ui.textBrowser_T->setText(str);
}

void Registration::importCloud()
{
	if (importPLYFile())
	{
		ui.label_accountOfClouds->setText("amount:" + QString::number(clouds.size()));
		ui.comboBox_fixedCloud->clear();
		ui.comboBox_movingCloud->clear();
		std::map<QString, PointCloudT::Ptr>::iterator iter;
		for (iter = clouds.begin(); iter != clouds.end(); iter++)
		{
			ui.comboBox_fixedCloud->addItem(iter->first);
			ui.comboBox_movingCloud->addItem(iter->first);
		}
	}
}

void Registration::startICP()
{
	if (settings.empty())
		setDefaultParameters();
	else
		updateParameters();
	pcl::console::TicToc time;
	time.tic();
	icpCloud = ICP();
	showResult(time.toc());
}

PointCloudT::Ptr Registration::ICP()
{
	icp.setMaximumIterations(settings[ui.label_InitialITR->text()].toInt());
	icp.setInputSource(clouds[settings[ui.label_movingCloud->text()]]);
	icp.setInputTarget(clouds[settings[ui.label_fixedCloud->text()]]);
	PointCloudT::Ptr cloud(new PointCloudT);
	icp.align(*cloud);
	return cloud;
}

void Registration::nextITR()
{
	updateParameters();
	pcl::console::TicToc time;
	time.tic();
	icp.setMaximumIterations(settings[ui.label_NextITR->text()].toInt());
	icp.setInputSource(icpCloud);
	icp.align(*icpCloud);
	showResult(time.toc());
}

void Registration::showResult(double time)
{
	ui.label_fitnessScore->setText("Fitness score : " + QString::number(icp.getFitnessScore()));
	ui.label_time->setText("Last iterration time : " + QString::number(time) + "ms");
	Eigen::Matrix4d transformation_matrix = icp.getFinalTransformation().cast<double>();
	print4x4Matrix(transformation_matrix);
	if (ui.checkBox_3D->isChecked())
		show3DResult();
	else
		stop3DResult();
}

void Registration::show3DResult()
{
	if (viewerThread == nullptr)
	{
		viewerThread = new Visualization3D;
		connect(this, SIGNAL(sendICPInformation(PointCloudT::Ptr, PointCloudT::Ptr, PointCloudT::Ptr, int)), viewerThread, SLOT(getICPInformation(PointCloudT::Ptr, PointCloudT::Ptr, PointCloudT::Ptr, int)) );
		connect(this, &Registration::update3DVisualization, viewerThread, &Visualization3D::updateVisualization);
		emit sendICPInformation(clouds[settings[ui.label_fixedCloud->text()]], clouds[settings[ui.label_movingCloud->text()]], icpCloud, icp.max_iterations_);
		viewerThread->start();
	}
	else
	{
		emit sendICPInformation(clouds[settings[ui.label_fixedCloud->text()]], clouds[settings[ui.label_movingCloud->text()]], icpCloud, icp.max_iterations_);
		emit update3DVisualization();
	}
}

void Registration::stop3DResult()
{
	if (viewerThread != nullptr)
	{
		viewerThread->terminate();
		disconnect(this, SIGNAL(sendICPInformation(PointCloudT::Ptr, PointCloudT::Ptr, PointCloudT::Ptr, int)), viewerThread, SLOT(getICPInformation(PointCloudT::Ptr, PointCloudT::Ptr, PointCloudT::Ptr, int)));
		disconnect(this, &Registration::update3DVisualization, viewerThread, &Visualization3D::updateVisualization);
		viewerThread->deleteLater();
		viewerThread = nullptr;
	}
}







