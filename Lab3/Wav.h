#include "WavHeader.h"

#include <vector>

class Wav
{
public:
	void CreateFromFile(const char* filename, int bits_per_sample = 16);
	void GetDescription();
	void ExtractData16bit(const char* filename);
	void ExtractData8bit(const char* filename);
	void ConvertStereoToMono();
	void SaveToFile(const char* filename, int sample_rate);

private:
	void ResetHeader();
	void CheckHeaderValidity(size_t file_size_bytes, int bits_per_sample);
	void PrefillHeader(int bits_per_sample);
	void FillHeader(int chan_count, int bits_per_sample, int sample_rate, int samples_count_per_chan);

	wav_header_s header_ptr;
	std::vector< std::vector<short> > channels_data;
	std::vector< std::vector<short> > edited_data;
};

