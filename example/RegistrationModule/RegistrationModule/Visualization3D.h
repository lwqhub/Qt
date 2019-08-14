#pragma once
#include <QThread>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

class Visualization3D : public QThread
{
	Q_OBJECT
public:
	Visualization3D();
	~Visualization3D();

protected:
	void run();

public slots:
	void getICPInformation(PointCloudT::Ptr, PointCloudT::Ptr, PointCloudT::Ptr, int);
	void updateVisualization();
private:
	PointCloudT::Ptr cloud_in, cloud_tr, cloud_icp;
	int iterations;
	bool next_iteration;
};

