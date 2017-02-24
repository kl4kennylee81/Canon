# HelloWorld (Android)

This directory contains a combined Ant/Eclipse project for building and installing the 
the basic demo on to Android. This demo draws the logo and moves it about the screen. 
It also has a button to quit the application at the bottom of the screen.

There are trade-offs for using Ant over Eclipse and vice versa.  You should read the
instructions below before making a decision.

Installing Android Tools
------------------------
Regardless of how you chose to build the demo, you will need both the Android SDK and NDK 
installed. The easiest way to install these is through Android Studio.

https://developer.android.com/studio/index.html

While Android Studio is itself a pre-beta hunk of garbage, the included SDK Manager is the
best way to install the Android SDK.  You should download the tools for Android N 
(Android 24) as well as the NDK (under SDK Tools). On OS X we recommend that you install 
in **/Library/Android/sdk**. On Windows, we recommend that you install in **C:\Android\sdk**.

Linking the Demo Assets
-----------------------
Both the Ant and Eclipse builds are very fragile when it comes to packaging assets (e.g
the PNG file) from outside of the project directory.  Therefore, you will either 
need to copy or link the file "anders.png" (found in the **assets** folder two levels
up) into the **assets** folder in this directory.

To Build the Demo with Ant
--------------------------
The project files in this directory allow you build an Android application with `ant`.
However, to work properly, you must modify **local.properties** to refer to the location
of the Android SDK ad NDK, respectively.

Once this is set-up, simply build the application with the command `ant debug`.  The APK
will be stored in the **bin** directory.  You can run the the APK on your device using
the Android Debug Bridge Utility.

https://developer.android.com/studio/command-line/adb.html

This utility is in the **platform-tools** folder of your Android SDK.

**_Warning_**: The Ant project depends on both the **demo** and the **src** folder two
levels up.  If either folder is moved, Ant will not be able to compile the project.


To Build the Demo with Eclipse
------------------------------
If you are trying to run the demo on a device, Eclipse is often the preferred way to do
that.  Eclipse simplifies the process of installing the APK, and it shows all of the
log and error statements in the debug monitor.

While this directory is also an Eclipse project, there are several steps that you must
take to get Eclipse ready for Android.  First, you need to install the ADT plugin.  While 
Google has deprecated this plugin, the instructions can still be found at 

https://stuff.mit.edu/afs/sipb/project/android/docs/sdk/installing/installing-adt.html.

When you install the plugin, do not allow Eclipse to install an alternate version of the
SDK (as those SDKs are older).  Instead, set it to your SDK installation directory via 
**Eclipse > Preferences > Android**.

Once, this is done, you can import this project normally.  However, there is one more
change that you have to make before building.  The ADT does not normally support C++ 
builds with the NDK.  We have added a custom NDK tool to solve this, but you will need
to configure it to your NDK directory.  Right-click on the project and select **Properties**.
Select **Builders > NDK Build** and choose **Edit**.  At the first text box, change the 
location to the path of your `ndk-build` executable.

Once you have done this, you can build the application normally (though you may need to
build it twice the first time).  Select the project in the package explore and run as an
Android Application.

**_Warning_**: The Eclipse project depends on the **demo** folder containing **main.cpp** 
two levels up.  If this folder is moved, Eclipse will not be able to import the project.
In addition, it will not be able to compile the libraries if the **src** folder two
levels up is moved.

Cleaning Up
-----------
While Eclipse can build everything, it cannot clean everything.  In particular, even a
complete clean will leave the libraries in **lib** and the C++ object files in **obj**. 
If you want to completely clean everything you will need to use `ant` and the command line.
Follow the instructions for building with `ant`, but type `ant clean` instead.

**_Warning_**: If you clean from the command line, you must clean in Eclipse before 
rebuilding.  Eclipse is not smart enough to know that the NDK libraries were erased
at the command line. However, once you clean it will know to rebuild the NDK libraries
(even though nothing happened).