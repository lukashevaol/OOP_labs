#include <ios>


namespace Errors {
	class WavFileError : public std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	class IO_ERROR : public WavFileError {
		using WavFileError::WavFileError;
	};

	class BAD_FORMAT : public WavFileError {
		using WavFileError::WavFileError;
	};

	class UNSUPPORTED_FORMAT : public BAD_FORMAT {
		using BAD_FORMAT::BAD_FORMAT;
	};

	class BAD_PARAMS : public BAD_FORMAT {
		using BAD_FORMAT::BAD_FORMAT;
	};

	class DATA_SIZE_ERROR : public BAD_FORMAT {
		using BAD_FORMAT::BAD_FORMAT;
	};

	class HeaderError : public std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	class HEADER_RIFF_ERROR : public HeaderError {
		using HeaderError::HeaderError;
	};

	class HEADER_FILE_SIZE_ERROR : public HeaderError {
		using HeaderError::HeaderError;
	};
	class HEADER_WAVE_ERROR : public HeaderError {
		using HeaderError::HeaderError;
	};
	class HEADER_FMT_ERROR : public HeaderError {
		using HeaderError::HeaderError;
	};
	class HEADER_SUBCHUNK1_ERROR : public HeaderError {
		using HeaderError::HeaderError;
	};
	class HEADER_NOT_PCM : public HeaderError {
		using HeaderError::HeaderError;
	};
	class HEADER_BYTES_RATE_ERROR : public HeaderError {
		using HeaderError::HeaderError;
	};

	class HEADER_BLOCK_ALIGN_ERROR : public HeaderError {
		using HeaderError::HeaderError;
	};

	class HEADER_SUBCHUNK2_SIZE_ERROR : public HeaderError {
		using HeaderError::HeaderError;
	};
}
