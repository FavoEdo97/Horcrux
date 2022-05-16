# Horcrux
Encrypt your files and chunks them (like an horcrux).

With this project you will be able to encrypt your files and split them into chunks , save the key and don’t lose it.
This project was tested on Windows 10, using Visual Studio, CMake, Google Test.

### BoostLib 1.79.0 (REQUIRED)
To configure Boost to work properly in Windows I followed the steps below.
First install boost 1.79.0 (binary) from this link:
https://sourceforge.net/projects/boost/files/boost-binaries/1.79.0_b1/boost_1_79_0_b1-msvc-14.2-64.exe/download
Then set the following environment variables:
*BOOST_LIBRARYDIR*,
*BOOST_INCLUDEDIR*,
*BOOST_ROOT*

I set the first variable with this value:**C:\Users\E.Favorido\boost_1_79_0_vc142\lib64-msvc-14.2**

The second: **C:\Users\E.Favorido\boost_1_79_0_vc142**

The third:**C:\UsersE.Favorido\boost_1_79_0_vc142\boost**

### OpenSSL 3_0_3
For the openssl configuration I downloaded the library from this link:
https://slproweb.com/download/Win64OpenSSL-3_0_3.exe

## CMake Command And Run
Now depending on the version of OpenSSL installed on your computer, you should take the specific dll and insert it into the src\dll directory.
I left you the dll that I used, which is the same that you will find by downloading OpenSSL from the link above.
In order to start everything correctly, open the command line and run the command **cmake** using the option **-DOPENSSL_ROOT_DIR** and set this option with the path of the directory OpenSSL installed.
You should also set the **-DCMAKE_INSTALL_PREFIX** option to have the dll and executables all end up in the **..\build\bin** directory from which you can run the executables. I set it to this value= **"C:\Users\E.Favorido\C++Progetti\cmake\horcrux\build\bin"**.

So I ran this command:

```
cmake . -DCMAKE_INSTALL_PREFIX="C:\Users\E.Favorido\C++Progetti\cmake\horcrux\build\bin"  -DOPENSSL_ROOT_DIR="C:\Users\E.Favorido\OpenSSL\OpenSSL-Win64" -S "C:\Users\E.Favorido\C++Progetti\cmake\horcrux\src" -B "C:\Users\E.Favorido\C++Progetti\cmake\horcrux\build"

```

This command should copy the DLL from the **src\dll** directory into the **${CMAKE_INSTALL_PREFIX}** directory set above.

Then open Visual Studio 2019 and launch the build of the project "INSTALL", if everything goes well it should compile the source code and once it is completed it will save the executables generated inside the directory **${CMAKE_INSTALL_PREFIX}**, that is the same above.

Now you can finally try to encrypt your files (at the moment only with AES) by running the commands from the ${CMAKE_INSTALL_PREFIX} directory:
```
1.user@hostname:~$ horcrux create -n <horcrux count> <inputpath> <output path>
2.user@hostname:~$ horcrux load -k <decryption key> <inputfiles> <output_file>
```



