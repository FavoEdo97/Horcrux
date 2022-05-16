# Horcrux
Encrypt your files and chunks them (like an horcrux)
# Horcrux
Encrypt your files and chunks them (like an horcrux)
With this project you will be able to encrypt your files and split them into chunks , save the key and don’t lose it.
This project was tested on Windows 10, using Visual Studio, cmake, Google Test.

### BoostLib 1.79.0
To configure Boost to work properly in Windows I followed the steps below.
First install boost 1.79.0 (binary) from this link:
https://sourceforge.net/projects/boost/files/boost-binaries/1.79.0_b1/boost_1_79_0_b1-msvc-14.2-64.exe/download
Then set the following environment variables:
*BOOST_LIBRARYDIR*,
*BOOST_INCLUDEDIR*,
*BOOST_ROOT*

The first tip to my binarinel was set to this value: 
**C:\Users\E.Favorido\boost_1_79_0_vc142\lib64-msvc-14.2**
The second point to include in my case was set to this value:
**C:\Users\E.Favorido\boost_1_79_0_vc142**
The third point to the boost folder in my case was set to this value:
**C:\UsersE.Favorido\boost_1_79_0_vc142\boost**

### OpenSSL 3_0_3
For the openssl configuration I downloaded the library from this link:
https://slproweb.com/download/Win64OpenSSL-3_0_3.exe

## CMake Command And Run
Now depending on the version of openssl installed on your computer you should take the specific dll and insert it into the src\dll folder.
I left you the dll that I used which is the same that you will find by downloading openssl from the link above.
Now to start everything correctly open the command line as administrator (to be sure) and run the command cmake using the option -DOPENSSL_ROOT_DIR and set this option with the path of the folder openssl installed.
Ora creare la cartella horcrux\build
In my case I launched this command:

```cmake . -DOPENSSL_ROOT_DIR="C:\Users\E.Favorido\OpenSSL\OpenSSL-Win64" -S "C:\Users\E.Favorido\C++Progetti\horcrux\src" -B "C:\Users\E.Favorido\C++Progetti\horcrux\build"```

Running this command should copy the DLL in the src dll folder into the **${CMAKE_INSTALL_PREFIX}/bin** CMake folder
In my case the folder is: **"C:\Program Files (x86)\horcrux\bin"**
Now open Visual Studio 2019 as administrator and launch the build of the project "Installation", if all goes well it should compile the source code and once it is completed it will save the executables generated inside the bin folder of cmake , that is the same above.

If you don’t open VS as an administrator, it won’t be able to move the executables to the cmake bin folder.
Now you can finally try to encrypt your files (momentarily only with pure AES) by running the commands from CMake bin directory:
```
1.user@hostname:~$ horcrux create -n <horcrux count> <inputpath> <output path>
2.user@hostname:~$ horcrux load -k <decryption key> <inputfiles> <output_file>
```



