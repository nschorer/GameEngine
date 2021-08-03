//--------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef FILE_H
#define FILE_H

// Paste your Gam475 File system here


	class File
	{
	public:

		typedef void* Handle;

		enum class Mode
		{
			READ = 0x7A000000,
			WRITE,
			READ_WRITE
		};

		enum class Location
		{
			BEGIN = 0x7B000000,
			CURRENT,
			END
		};

		enum class Error
		{
			SUCCESS = 0x7C000000,
			OPEN_FAIL,
			CLOSE_FAIL,
			WRITE_FAIL,
			READ_FAIL,
			SEEK_FAIL,
			TELL_FAIL,
			FLUSH_FAIL,
			FIND_ALL_FILES_FAIL,
			UNDEFINED
		};

	public:
		static File::Error Open(File::Handle& fh, const char* const fileName, File::Mode mode) noexcept;
		static File::Error Close(File::Handle& fh) noexcept;
		static File::Error Write(File::Handle fh, const void* const buffer, const size_t inSize) noexcept;
		static File::Error Read(File::Handle fh, void* const _buffer, const size_t _size) noexcept;
		static File::Error Read(File::Handle fh, void* const _buffer, const size_t _size, unsigned int& hash) noexcept;
		static File::Error Seek(File::Handle fh, File::Location location, int offset) noexcept;
		static File::Error Tell(File::Handle fh, unsigned int& offset) noexcept;
		static File::Error Flush(File::Handle fh) noexcept;
		static bool IsHandleValid(File::Handle fh) noexcept;

		static File::Error FindAllFilesWithExt(const char* const ext, char**& fileNames, int& fileCount);

	};


#endif

// ---  End of File ---------------
