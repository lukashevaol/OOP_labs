#include "stdafx.h"
#include <iostream>
#include "Errors.h"
#include "Wav.h"

using namespace std;

int main(int argc, char *argv[])
{
	cout << "************** | WavCore | **************" << endl;


	// ################  Tests for WavCore  ################

	const char* input_fname = "D:\\oop\\lab3\\0.wav";
	const char* output_fname = "D:\\oop\\lab3\\out.wav";

	Wav wav;

	// #### Opening WAV file, checking header.
	try {
		wav.ExtractData16bit(input_fname);
		wav.GetDescription();
	}
	catch (Errors::IO_ERROR error) {
		cout << "[File reading error] : " << error.what() << endl;
		getchar();
		return 0;
	}
	catch (Errors::HeaderError error) {
		cout << "[Error in checking header] : " << error.what() << endl;
		getchar();
		return 0;
	}
	cout << endl << "********************" << endl;
	// # Making signal mono from stereo.
	wav.ConvertStereoToMono();
	wav.SaveToFile(output_fname, 44100);

	system("pause");
	return 0;
}