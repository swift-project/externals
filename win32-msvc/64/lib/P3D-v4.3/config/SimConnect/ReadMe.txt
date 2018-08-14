  The SimConnect.xml and SimConnect.ini files located in this dir are examples of
data files that are used to configure the Prepar3D SimConnect system. These two files are not
needed to use SimConnect under normal circumstances. By default, SimConnect client addons that
run on the same machine as Prepar3D will work without either of these two files needing to be
present. 

  The SimConnect.xml file contains config parameters for the SimConnect communications layer.
This file is placed in the "C:\Users\<username>\AppData\Roaming\Lockheed Martin\Prepar3D vX"
directory - the same directory where the Prepar3D.cfg file is located where X is the version of Prepar3D. 
This file does not need to be present if the SimConnect system is used in the default configuration that supports 
all SimConnect clients that are running on the same machine that Prepar3D is running on. Only if it is desired to have
SimConnect clients running on other machines than the one running Prepar3D, is a SimConnect.xml file
needed.

  The SimConnect.ini file contains config parameters for the SimConnect diagnostic system. This file
is placed in the "My Documents\Prepar3D Files" directory. This file only needs to be present
if the user wants to examine the SimConnect diagnostic output data; it is not needed otherwise.

  The DLL.xml and EXE.xml files are examples of the configuration files that are used to
specify addon code that is launched or loaded by Prepar3D when it starts. These files are
placed in the "C:\Users\<username>\AppData\Roaming\Lockheed Martin\Prepar3D vX" directory - the
same directory where the Prepar3D.cfg file is located where X is the version of Prepar3D.