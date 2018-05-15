#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <vector>
#include <string>
#include "Segment.h"

using namespace cv;
using namespace std;

class ImageToSegment
{
public:
	vector<Segment> segs;
	int h, m;

	ImageToSegment()
	{
	}

	vector<Segment> imageToSeg(string imageName)
	{
		
		vector<Segment> s;

		Mat image = imread(imageName);

		// Check if any image was actually passed in.
		//if (!image.data)
		//	cout << "Check you have input the correct image!" << endl;
		//	return;

		// Show source image
		// Change the background from white to black, since that will help later to extract
		// better results during the use of Distance Transform
		for (int x = 0; x < image.rows; x++) {
			for (int y = 0; y < image.cols; y++) {
				if (image.at<Vec3b>(x, y) == Vec3b(255, 255, 255)) {
					image.at<Vec3b>(x, y)[0] = 0;
					image.at<Vec3b>(x, y)[1] = 0;
					image.at<Vec3b>(x, y)[2] = 0;
				}
			}
		}

		//imshow("Black Background Image", image);
		//Create a kernel that we will use for accuting/sharpening our image
		Mat kernel = (Mat_<float>(3, 3) <<
			1, 1, 1,
			1, -8, 1,
			1, 1, 1); 
		Mat imgLaplacian;
		Mat sharp = image; // copy source image to another temporary one
		filter2D(sharp, imgLaplacian, CV_32F, kernel);
		image.convertTo(sharp, CV_32F);
		Mat imgResult = sharp - imgLaplacian;
		// convert back to 8bits gray scale
		imgResult.convertTo(imgResult, CV_8UC3);
		imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
		//imshow( "Laplace Filtered Image", imgLaplacian );
		imshow("Step 1: New Sharped Image", imgResult);
		image = imgResult; // copy back
						   // Create binary image from source image
		Mat bw;
		cvtColor(image, bw, CV_BGR2GRAY);
		threshold(bw, bw, 1, 30, CV_THRESH_BINARY | CV_THRESH_OTSU);

		// Perform the distance transform algorithm
		Mat dist;
		distanceTransform(bw, dist, CV_DIST_L2, 3);
		// Normalize the distance image for range = {0.0, 1.0}
		// so we can visualize and threshold it
		normalize(dist, dist, 0, 1., NORM_MINMAX);
		imshow("Step 2: Distance Transform Image", dist);

		// Threshold to obtain the peaks
		// This will be the markers for the foreground objects
		threshold(dist, dist, .4, 1., CV_THRESH_BINARY);
		// Dilate a bit the dist image
		Mat kernel1 = Mat::ones(3, 3, CV_8UC1);
		dilate(dist, dist, kernel1);
		imshow("Step 3: Peaks", dist);

		// It is needed for findContours()
		Mat dist_8u;
		dist.convertTo(dist_8u, CV_8U);
		// Find total markers
		vector<vector<Point> > contours;
		findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		// Create the marker image for the watershed algorithm
		Mat markers = Mat::zeros(dist.size(), CV_32SC1);
		// Draw the foreground markers
		for (size_t i = 0; i < contours.size(); i++)
			drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i) + 1), -1);
		// Draw the background marker
		circle(markers, Point(5, 5), 3, CV_RGB(255, 255, 255), -1);

		// Perform the watershed algorithm
		watershed(image, markers);
		Mat mark = Mat::zeros(markers.size(), CV_8UC1);
		markers.convertTo(mark, CV_8UC1);
		bitwise_not(mark, mark);

		vector<Vec3b> colors;
		for (size_t i = 0; i < contours.size(); i++)
		{
			int b = theRNG().uniform(0, 255);
			int g = theRNG().uniform(0, 255);
			int r = theRNG().uniform(0, 255);
			colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
			Segment d = Segment();
			s.push_back(d);
		}

		Mat dst = Mat::zeros(markers.size(), CV_8UC3);
		for (int i = 0; i < markers.rows; i++)
		{
			for (int j = 0; j < markers.cols; j++)
			{
				int index = markers.at<int>(i, j);
				if (index > 0 && index <= static_cast<int>(contours.size()))
				{
					dst.at<Vec3b>(i, j) = colors[index - 1];
					s[index - 1].x += j;
					s[index - 1].y += i;
					s[index - 1].count++;
					//col[0] = R etc
					Vec3b col = image.at<Vec3b>(i, j);
					s[index - 1].r += col[0];
					s[index - 1].g += col[1];
					s[index - 1].b += col[2];
					s[index - 1].segNumber = index - 1;
				}
				else
					dst.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			}
		}

		for each (Segment seg in s)
		{
			h = image.rows;
			m = image.cols;
		}

		imshow("Step 4: Final Result", dst);


			//Algorithm3(image, 20, 100);
		
		return s;
	}

	/*
	Algorithm 3 takes columns of pixels and represents them as harmonics.

	*/
	void Algorithm3(Mat img, int xAmount, int yAmount)
	{
		//Create an empty audio file to fill up.
		string empty = "synth silence algo3.wav 44100 1 1";
		system(empty.c_str());

		//For every pixel column:
		for (int i = 0; i <= xAmount; i++)
		{
			string xamoun = "synth silence "+ std::to_string(i)+ ".wav 44100 1 0.125";
			system(xamoun.c_str());
			//Create empty called xamount
			int column = (img.rows / xAmount) * i;

			//for every pixel in column
			for (int y = 0; y <= yAmount; y++)
			{
				//Get which pixel is being used.
				int pixel = (img.cols / yAmount) * y;
				if (pixel == 0)
				{
					pixel = 1;
				}

				//Luminosity of pixel, for gain later.
				float pixelLumin = ((0.2126*img.at<Vec3b>(column, pixel)[0]) + 
									(0.7152*img.at<Vec3b>(column, pixel)[1]) + 
									(0.0722*img.at<Vec3b>(column, pixel)[2]));

				//Make sure luminosity does not bug out, a value of less than 1 is not allowed.
				if (pixelLumin <= 1)
				{
					pixelLumin = 3;
				}

				//Build sine for each, add to xamount
				string audio = "synth wave 1 " + std::to_string(column) 
							 + "_" + std::to_string(pixel) + ".wav 44100 1 0.125 "+ std::to_string(pixel);
				system(audio.c_str());

				//Change the sine waves gain to luminosty.
				string vol = "modify loudness 4 " + std::to_string(column) 
						   + "_" + std::to_string(pixel) + ".wav" + 
					        (" vol" + std::to_string(column) + "_" + std::to_string(pixel) + ".wav") 
					       + " -l" + to_string(pixelLumin/256);
				system(vol.c_str());

				//cleanup
				string del = "del " + std::to_string(column) + "_" + std::to_string(pixel) + ".wav";
				system(del.c_str());

				//add to sound
				string add = "nmix " + std::to_string(i) + ".wav " + 
					(" vol" + std::to_string(column) + "_" + std::to_string(pixel) + ".wav") + " temp.wav";
				system(add.c_str());

				//cleaup
				del = "del vol" + std::to_string(column) + "_" + std::to_string(pixel) + ".wav";
				system(del.c_str());

				//cleaup
				del = "del "+ std::to_string(i) + ".wav";
				system(del.c_str());

				//rename to main file
				string ren = "ren temp.wav " + std::to_string(i) + ".wav";
				system(ren.c_str());
			}
			//pan column
			float amount = (2 * (i / (float)xAmount)) - 1;
			string pan = "modify space 1 " + (to_string(i) + ".wav") + (" p" + (to_string(i) + ".wav")) + " " + to_string(amount);
			system(pan.c_str());

			//Add to list of things to concantenate
			string echo = "echo p" + (to_string(i) + ".wav")+">> algo3.txt";
			system(echo.c_str());

			//Cleaup
			string del = "del " + (to_string(i) + ".wav");
			system(del.c_str());
		}
		//Concantenate the columns
		string conc = "njoin algo3.txt Algorithm3.wav";
		system(conc.c_str());

		for (int i = 0; i < xAmount; i++)
		{
			//Cleaup
			string del = "del p" + (to_string(i) + ".wav");
			system(del.c_str());
		}
	}
};