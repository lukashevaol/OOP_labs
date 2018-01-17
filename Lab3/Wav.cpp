#include "stdafx.h"
#include "Wav.h"
#include "WavHeader.h"
#include "Errors.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>


void Wav::CreateFromFile(const char* filename, int bits_per_sample) {
	printf(">>>> Create header from file( %s )\n", filename);
	ResetHeader(); // Fill header with zeroes.

	FILE* f = fopen(filename, "rb");
	if (!f) {
		throw Errors::IO_ERROR("No file with such name");
	}

	size_t blocks_read = fread(&header_ptr, sizeof(wav_header_s), 1, f);
	if (blocks_read != 1) {
		// can't read header, because the file is too small.
		throw Errors::BAD_FORMAT("Can't read header, because the file is too small");
	}

	fseek(f, 0, SEEK_END); // seek to the end of the file
	size_t file_size = ftell(f); // current position is a file size!
	fclose(f);

	CheckHeaderValidity(file_size, bits_per_sample);
}

void Wav::ResetHeader() {
	memset(&header_ptr, 0, sizeof(wav_header_s));
}

void Wav::CheckHeaderValidity(size_t file_size_bytes, int bits_per_sample) {
	// Go to wav_header.h for details

	if (header_ptr.chunkId[0] != 'R' ||
		header_ptr.chunkId[1] != 'I' ||
		header_ptr.chunkId[2] != 'F' ||
		header_ptr.chunkId[3] != 'F')
	{
		throw Errors::HEADER_RIFF_ERROR("HEADER_RIFF_ERROR\n");
	}

	if (header_ptr.chunkSize != file_size_bytes - 8) {
		throw Errors::HEADER_FILE_SIZE_ERROR("HEADER_FILE_SIZE_ERROR\n");
	}

	if (header_ptr.format[0] != 'W' ||
		header_ptr.format[1] != 'A' ||
		header_ptr.format[2] != 'V' ||
		header_ptr.format[3] != 'E')
	{
		throw Errors::HEADER_WAVE_ERROR("HEADER_WAVE_ERROR");
	}

	if (header_ptr.subchunk1Id[0] != 'f' ||
		header_ptr.subchunk1Id[1] != 'm' ||
		header_ptr.subchunk1Id[2] != 't' ||
		header_ptr.subchunk1Id[3] != ' ')
	{
		throw Errors::HEADER_FMT_ERROR("HEADER_FMT_ERROR");
	}

	if (header_ptr.audioFormat != 1) {
		throw Errors::HEADER_NOT_PCM("HEADER_NOT_PCM");
	}

	if (header_ptr.subchunk1Size != bits_per_sample) {
		throw Errors::HEADER_SUBCHUNK1_ERROR("HEADER_SUBCHUNK1_ERROR");
	}

	if (header_ptr.byteRate != header_ptr.sampleRate * header_ptr.numChannels * header_ptr.bitsPerSample / 8) {
		throw Errors::HEADER_BYTES_RATE_ERROR("HEADER_BYTES_RATE_ERROR");
	}

	if (header_ptr.blockAlign != header_ptr.numChannels * header_ptr.bitsPerSample / 8) {
		throw Errors::HEADER_BLOCK_ALIGN_ERROR("HEADER_BLOCK_ALIGN_ERROR");
	}

	if (header_ptr.subchunk2Id[0] != 'd' ||
		header_ptr.subchunk2Id[1] != 'a' ||
		header_ptr.subchunk2Id[2] != 't' ||
		header_ptr.subchunk2Id[3] != 'a')
	{
		throw Errors::HEADER_FMT_ERROR("HEADER_FMT_ERROR");
	}

	if (header_ptr.subchunk2Size != file_size_bytes - 44)
	{
		throw Errors::HEADER_SUBCHUNK2_SIZE_ERROR("HEADER_SUBCHUNK2_SIZE_ERROR");
	}
}

void Wav::GetDescription() {
	printf("-------------------------\n");
	printf(" audioFormat   %u\n", header_ptr.audioFormat);
	printf(" numChannels   %u\n", header_ptr.numChannels);
	printf(" sampleRate    %u\n", header_ptr.sampleRate);
	printf(" bitsPerSample %u\n", header_ptr.bitsPerSample);
	printf(" byteRate      %u\n", header_ptr.byteRate);
	printf(" blockAlign    %u\n", header_ptr.blockAlign);
	printf(" chunkSize     %u\n", header_ptr.chunkSize);
	printf(" subchunk1Size %u\n", header_ptr.subchunk1Size);
	printf(" subchunk2Size %u\n", header_ptr.subchunk2Size);
	printf("-------------------------\n");
}

void Wav::ExtractData16bit(const char* filename) {
	printf(">>>> Extract data in 16-bit( %s )\n", filename);

	CreateFromFile(filename, 16);

	if (header_ptr.bitsPerSample != 16) {
		// Only 16-bit samples is supported.
		throw Errors::UNSUPPORTED_FORMAT("UNSUPPORTED_FORMAT");
	}

	FILE* f = fopen(filename, "rb");
	if (!f) {
		throw Errors::IO_ERROR("No file with such name");
	}
	fseek(f, 44, SEEK_SET); // Seek to the begining of PCM data.

	int chan_count = header_ptr.numChannels;
	int samples_per_chan = (header_ptr.subchunk2Size / sizeof(short)) / chan_count;

	// 1. Reading all PCM data from file to a single vector.
	std::vector<short> all_channels;
	all_channels.resize(chan_count * samples_per_chan);
	size_t read_bytes = fread(all_channels.data(), 1, header_ptr.subchunk2Size, f);
	if (read_bytes != header_ptr.subchunk2Size) {
		std::string s_err = "extract_data_int16() read only " + std::to_string(read_bytes) + " of " + std::to_string(header_ptr.subchunk2Size);
		throw Errors::IO_ERROR(s_err);
	}
	fclose(f);

	// 2. Put all channels to its own vector.
	channels_data.resize(chan_count);
	for (size_t ch = 0; ch < channels_data.size(); ch++) {
		channels_data[ch].resize(samples_per_chan);
	}

	for (int ch = 0; ch < chan_count; ch++) {
		std::vector<short>& chdata = channels_data[ch];
		for (size_t i = 0; i < samples_per_chan; i++) {
			chdata[i] = all_channels[chan_count * i + ch];
		}
	}
}

void Wav::ExtractData8bit(const char* filename) {
	printf(">>>> Extract data in 8-bit( %s )\n", filename);

	CreateFromFile(filename, 8);

	if (header_ptr.bitsPerSample != 8) {
		// Only 8-bit samples is supported.
		throw Errors::UNSUPPORTED_FORMAT("UNSUPPORTED_FORMAT");
	}

	FILE* f = fopen(filename, "rb");
	if (!f) {
		throw Errors::IO_ERROR("No file with such name");
	}
	fseek(f, 44, SEEK_SET); // Seek to the begining of PCM data.

	int chan_count = header_ptr.numChannels;
	int samples_per_chan = (header_ptr.subchunk2Size / sizeof(short)) / chan_count;

	// 1. Reading all PCM data from file to a single vector.
	std::vector<short> all_channels;
	all_channels.resize(chan_count * samples_per_chan);
	size_t read_bytes = fread(all_channels.data(), 1, header_ptr.subchunk2Size, f);
	if (read_bytes != header_ptr.subchunk2Size) {
		std::string s_err = "extract_data_int8() read only " + std::to_string(read_bytes) + " of " + std::to_string(header_ptr.subchunk2Size);
		throw Errors::IO_ERROR(s_err);
	}
	fclose(f);

	// 2. Put all channels to its own vector.
	channels_data.resize(chan_count);
	for (size_t ch = 0; ch < channels_data.size(); ch++) {
		channels_data[ch].resize(samples_per_chan);
	}

	for (int ch = 0; ch < chan_count; ch++) {
		std::vector<short>& chdata = channels_data[ch];
		for (size_t i = 0; i < samples_per_chan; i++) {
			chdata[i] = all_channels[chan_count * i + ch];
		}
	}
}

void Wav::ConvertStereoToMono() {
	printf(">>>> Converting stereo to mono\n");
	int chan_count = (int)channels_data.size();

	if (chan_count != 2) {
		throw Errors::BAD_PARAMS("BAD_PARAMS");
	}

	int samples_count_per_chan = (int)channels_data[0].size();

	// Verify that all channels have the same number of samples.
	for (size_t ch = 0; ch < chan_count; ch++) {
		if (channels_data[ch].size() != (size_t)samples_count_per_chan) {
			throw Errors::BAD_PARAMS("BAD_PARAMS");
		}
	}

	edited_data.resize(1);
	std::vector<short>& mono = edited_data[0];
	mono.resize(samples_count_per_chan);

	// Mono channel is an arithmetic mean of all (two) channels.
	for (size_t i = 0; i < samples_count_per_chan; i++) {
		mono[i] = (channels_data[0][i] + channels_data[1][i]) / 2;
	}
}

void Wav::PrefillHeader(int bits_per_sample) {
	header_ptr.chunkId[0] = 'R';
	header_ptr.chunkId[1] = 'I';
	header_ptr.chunkId[2] = 'F';
	header_ptr.chunkId[3] = 'F';

	header_ptr.format[0] = 'W';
	header_ptr.format[1] = 'A';
	header_ptr.format[2] = 'V';
	header_ptr.format[3] = 'E';

	header_ptr.subchunk1Id[0] = 'f';
	header_ptr.subchunk1Id[1] = 'm';
	header_ptr.subchunk1Id[2] = 't';
	header_ptr.subchunk1Id[3] = ' ';

	header_ptr.subchunk2Id[0] = 'd';
	header_ptr.subchunk2Id[1] = 'a';
	header_ptr.subchunk2Id[2] = 't';
	header_ptr.subchunk2Id[3] = 'a';

	header_ptr.audioFormat = 1;
	header_ptr.subchunk1Size = 16;
	header_ptr.bitsPerSample = bits_per_sample;
}

void Wav::FillHeader(int chan_count, int bits_per_sample, int sample_rate, int samples_count_per_chan) {
	if (bits_per_sample != 16 && bits_per_sample != 8) {
		throw Errors::UNSUPPORTED_FORMAT("UNSUPPORTED_FORMAT");
	}

	if (chan_count < 1) {
		throw Errors::BAD_FORMAT("BAD_FORMAT");
	}
	PrefillHeader(bits_per_sample);

	int file_size_bytes = 44 + chan_count * (bits_per_sample / 8) * samples_count_per_chan;

	header_ptr.sampleRate = sample_rate;
	header_ptr.numChannels = chan_count;
	header_ptr.bitsPerSample = bits_per_sample;

	header_ptr.chunkSize = file_size_bytes - 8;
	header_ptr.subchunk2Size = file_size_bytes - 44;

	header_ptr.byteRate = header_ptr.sampleRate * header_ptr.numChannels * header_ptr.bitsPerSample / 8;
	header_ptr.blockAlign = header_ptr.numChannels * header_ptr.bitsPerSample / 8;
}

void Wav::SaveToFile(const char* filename, int sample_rate) {
	printf(">>>> Save to file( %s )\n", filename);
	wav_header_s header;

	int chan_count = (int)edited_data.size();

	if (chan_count < 1) {
		throw Errors::BAD_PARAMS("BAD_PARAMS");
	}

	int samples_count_per_chan = (int)edited_data[0].size();

	// Verify that all channels have the same number of samples.
	for (size_t ch = 0; ch < chan_count; ch++) {
		if (edited_data[ch].size() != (size_t)samples_count_per_chan) {
			throw Errors::BAD_PARAMS("BAD_PARAMS");
		}
	}

	FillHeader(chan_count, 16, sample_rate, samples_count_per_chan);

	std::vector<short> all_channels;
	all_channels.resize(chan_count * samples_count_per_chan);

	for (int ch = 0; ch < chan_count; ch++) {
		const std::vector<short>& chdata = edited_data[ch];
		for (size_t i = 0; i < samples_count_per_chan; i++) {
			all_channels[chan_count * i + ch] = chdata[i];
		}
	}

	FILE* f = fopen(filename, "wb");
	fwrite(&header, sizeof(wav_header_s), 1, f);
	fwrite(all_channels.data(), sizeof(short), all_channels.size(), f);
	if (!f) {
		throw Errors::IO_ERROR("No file with such name");
	}

	fclose(f);
}