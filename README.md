BinauralPanner


	Linux

		Binary (Linux VST)
		-> Builds/LinuxMakefile/build/BinauralPanner.so

	Compile
		1. install libsamplerate and fftw3 with floating point enabled
		2. cd to Builds/LinuxMakefile/build
		3. make CONFIG=Release


	MacOSX

		Binary (AU, VST and VST3)
			-> Builds/MacOSX/build/Release/BinauralPanner_MacOSX_v1_0_10.zip

		Compile
			1. open terminal
			2. type "brew install fftw --universal" or  download fftw from http://www.fftw.org/ and install fftw3 with floating point and universal binary enabled. (./configure CFLAGS="-arch i386 -arch x86_64"--enable-float --enable-threads
			3.  type "brew install libsamplerate"
			4. open the xcode project in Builds/MacOSX
			5. build for profile


	Windows

		will be available in the future
