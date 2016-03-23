#pragma once

#include <QList>
#include <QPointF>
#include <qmath.h>


/* Qt implementation of downsampling algorithm by Sveinn Steinarsson
   http://skemman.is/stream/get/1946/15343/37285/3/SS_MSthesis.pdf
*/
void downsample(const QList<QPointF> & in, QList<QPointF> & out , int threshold)
{

	int dataSize = in.size();
	if ( ( dataSize <= threshold) || (threshold == 0 ) )
	{
		out = in;
		return;
	}

	qreal bucketSize = (qreal) (in.size() - 2) / (qreal)(threshold - 2);

	int a = 0, next_a = 0;

	out.append(in.first());

	for (int i = 0; i < threshold - 2; ++i)
	{


		int avgRangeStart = (int) ( qFloor((i + 1) * bucketSize) + 1);
		int avgRangeEnd   = (int) ( qFloor((i + 2) * bucketSize) + 1);

		avgRangeEnd = avgRangeEnd < dataSize ? avgRangeEnd : dataSize;

		int avgRangeLength   = (int)( avgRangeEnd - avgRangeStart );

		qreal avgX = 0., avgY = 0.;

		while (avgRangeStart < avgRangeEnd)
		{
			avgX += in.at(avgRangeStart).x();
			avgY += in.at(avgRangeStart).y();
			++avgRangeStart;
		}

		avgX /= avgRangeLength;
		avgY /= avgRangeLength;

		int rangeOffs  = (int) ( qFloor( (i + 0) * bucketSize) + 1);
		int rangeTo    = (int) ( qFloor( (i + 1) * bucketSize) + 1);

		qreal pointAx = in.at(a).x();
		qreal pointAy = in.at(a).y();


		qreal maxArea = -1;

		QPointF maxAreaPoint;
		while (rangeOffs < rangeTo)
		{
			qreal area = qAbs ( (pointAx - avgX) * ( in.at(rangeOffs).y() - pointAy )  - ( pointAx - in.at(rangeOffs).x() ) * (avgY - pointAy) ) * 0.5;

			if (area > maxArea)
			{
				maxArea = area;
				maxAreaPoint = in.at(rangeOffs) ;
				next_a = rangeOffs;
			}
			++rangeOffs;
		}

		out.append( maxAreaPoint  );
		a = next_a;
	}

	out.append (in.last());
}