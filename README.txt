To get this project running on your machine you will need to follow the steps below.

Prerequisites:
	1. Download CMake version 3.0+
	2. Have C++ 20

1. Download the project from git Repo

2. Run CMake using commands below depending on what OS and build system:
	-- For all use the flags I set to OFF to ensure it doesn't use libs that aren't needed/included in the repo

	Windows OS:
	If using Visual Studio:		
		- NOTE: Swap for your version of visual studio 
		a. cmake -G "Visual Studio 17 2022" -DSDL2IMAGE_DAV1D=OFF -DSDL2IMAGE_VENDORED=OFF -DSDL2IMAGE_AVIF_VENDORED=OFF ..
		b. cmake --build . --config Release

	- NOTE: I don't have Linux OS to test these, feel free to reach out and let me know if this works
	Linux:
	To use default Makefiles:
		a. cmake -DSDL2IMAGE_DAV1D=OFF -DSDL2IMAGE_VENDORED=OFF -DSDL2IMAGE_AVIF_VENDORED=OFF ..
		b. make -j$(nproc)
	Or if using Ninja:
		a. cmake -G "Ninja" -DSDL2IMAGE_DAV1D=OFF -DSDL2IMAGE_VENDORED=OFF -DSDL2IMAGE_AVIF_VENDORED=OFF ..
		b. ninja

	- NOTE: Haven't tested in Mac OS yet...
	Mac OS:
	To build using Xcode
		a. cmake -G "Xcode" -DSDL2IMAGE_DAV1D=OFF -DSDL2IMAGE_VENDORED=OFF -DSDL2IMAGE_AVIF_VENDORED=OFF ..
		b. cmake --build . --config Release	

	If unsure, you can simply run (CMake will pick automatically):
		a. cmake -DSDL2IMAGE_DAV1D=OFF -DSDL2IMAGE_VENDORED=OFF -DSDL2IMAGE_AVIF_VENDORED=OFF ..
		b. cmake --build . --config Release

3. EXE file will be found in the "Release" directory found within the "Build" directory
 - You should see 2 other folders there called "data" and "resources" along with the dll files of SDL2 libs