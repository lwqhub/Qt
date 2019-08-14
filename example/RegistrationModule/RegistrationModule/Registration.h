#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Registration.h"
#include <pcl/point_types.h>
#include <pcl/registration/icp.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <vector>
#include <QString>
#include "Visualization3D.h"

typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

class Registration : public QMainWindow
{
	Q_OBJECT

public:
	Registration(QWidget *parent = Q_NULLPTR);
	bool importPLYFile();
	void setDefaultParameters();
	void updateParameters();
	void print4x4Matrix(const Eigen::Matrix4d & matrix);
	void importCloud();
	void startICP();
	PointCloudT::Ptr ICP();
	void nextITR();
	void showResult(double time);
	void show3DResult();
	void stop3DResult();

signals:
	void sendICPInformation(PointCloudT::Ptr, PointCloudT::Ptr, PointCloudT::Ptr, int);
	void update3DVisualization();

private:
	Ui::RegistrationClass ui;
	std::map<QString, PointCloudT::Ptr> clouds;  // Original point clouds
	std::map<QString, QString> settings; //ICP parameter settings
	PointCloudT::Ptr icpCloud;
	pcl::IterativeClosestPoint<PointT, PointT> icp;
	Visualization3D *viewerThread;
};
