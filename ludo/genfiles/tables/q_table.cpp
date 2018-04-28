#include <opencv2/core.hpp>
#include <iostream>
#include <string>
#include <fstream>
using namespace cv;
using namespace std;


int main( int argc, char** argv )
{

	FileStorage fs("/home/charlie/workspace/AI/LUDO-ToAI/ludo/genfiles/Q_Table.xml", FileStorage::WRITE);
	Mat table = Mat::ones(7, 11, CV_64FC1);
	fs << "Q_table" << table;
	fs.release();

	cout << "Q_table rows: " << table.rows << endl;
	cout << "Q_table columns: " << table.cols << endl;


	// Load and initialize Q-table.
	Mat Q_table;
	FileStorage fj("/home/charlie/workspace/AI/LUDO-ToAI/ludo/genfiles/Q_Table.xml", FileStorage::READ);
	fj["Q_table"] >> Q_table;
	fj.release();
	cout << Q_table << endl;
	// Display Q-table.
	return 0;

} // main()
