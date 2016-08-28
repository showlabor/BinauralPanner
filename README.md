BinauralPanner

//========================================================================================
	Linux

		Binary (Linux VST)
		-> Builds/LinuxMakefile/build/BinauralPanner.so


	Compile
		1. install libsamplerate and fftw3(with fftw3f) from your package manager		2. cd to Builds/LinuxMakefile/build
		3. make CONFIG=Release
//========================================================================================

	MacOSX

		Binary (AU, VST and VST3)
			-> Builds/MacOSX/build/Release/BinauralPanner_MacOS_v1_0_10b.zip

		Compile
			1. open terminal
			2. type "brew install fftw --universal" or  download fftw from http://www.fftw.org/ and install fftw3 with floating point and universal binary enabled. (./configure CFLAGS="-arch i386 -arch x86_64"--enable-float --enable-threads
			3. type "brew install libsamplerate"
			4. open the xcode project in Builds/MacOSX
			5. Project->build for profile
			6. type the following in the terminal
				install_name_tool -change /usr/local/opt/libsamplerate/lib/libsamplerate.0.dylib @loader_path/../Frameworks/libsamplerate.0.dylib 
			DO NOT PRESS RETURN !!!!
			7. right click the AU and choose show package content
			8. go to Contents/MacOS
			9. drag the BinaurlPanner executable to the terminal and press return 
			10. create a folder called “Frameworks” in the “Contents” folder
			11. copy the libsamplerate.0.dylib from /usr/local/lib/Cellar/libsamplerate/VERSION/lib/ to Contents/Frameworks (you should have Contents/Frameworks/libsamplerate.0.dylib now)
			12. type the following in the terminal
				install_name_tool -change /usr/local/opt/libsamplerate/lib/libsamplerate.0.dylib @loader_path/../Frameworks/libsamplerate.0.dylib 
			DO NOT PRESS RETURN !!!!
			13. drag the libsamplerate.0.dylib  to the terminal and press return
			14. repeat step 6 to 13 for VST and VST3

//========================================================================================

	Windows

		will be available in the future
