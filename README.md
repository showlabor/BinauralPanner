BinauralPanner


	Linux

		Binary (Linux VST)
		-> Builds/LinuxMakefile/build/BinauralPanner.so

	Compile
		1. install fftw3 with floating point and universal binary enabled.
		2. cd to Builds/LinuxMakefile/build
		3. make CONFIG=Release


	MacOSX

		Binary (AU and VST)
			-> Builds/MacOSX/build/Release/BinauralPanner.vst
			-> Builds/MacOSX/build/Release/BinauralPanner.component

		Compile
			1. install fftw3 with floating point and universal binary enabled. (./configure CFLAGS="-arch i386 -arch x86_64"--enable-float --enable-threads
			2. open the xcode project in Builds/MacOSX
			3. compile 


	Windows

		will be available in the future
