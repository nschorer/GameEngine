#ifndef PACKAGE_HEADER_H
#define PACKAGE_HEADER_H

struct PackageHeader {
	static const unsigned int PACKAGE_VERSION_SIZE = 20;
	static const unsigned int PACKAGE_NAME_SIZE = 20;

	char			packageName[PACKAGE_NAME_SIZE] = "DEFAULT";
	char			versionString[PACKAGE_VERSION_SIZE] = "1.0";
	unsigned int	numChunks;
	unsigned int	packageSize;   // size of file (without package header)
};

#endif
