# **VarroaPop – A Varroa Mite Population Model**

VarroaPop is a honeybee population model that incorporates the effects of a colony’s infestation by the varroa mite.  VarroaPop allows the user to evaluate the effects of a variety of initial conditions and weather on the population of a honeybee colony.  It also allows the user to simulate the effects of varroa migration into an existing colony.


## **Software Information**

1. **System Requirements**

In order to use VarroaPop, you must have Microsoft Windows 95, WIndows NT 4.0 , or later versions installed on your computer.  The VarroaPop software will consume approximately 1 MB of memory on your hard drive.

1. **VarroaPop Files**

After installing VarroaPop, you will have the following files in the installation directory (e.g. C:\VarroaPop):

**VarroaPop.exe** – the VarroaPop program

**Midwest5yr.wth** – a weather file containing five years of Midwestern US weather data

**Sthwest5yr.wth** – a weather file containing five years of Southwestern US weather data

**LG\_HI.col** – an example colony file containing a set of initial conditions.

**Figure2.vrp** – a sample VarropPop session file.

**Readme.doc** – this file

1. **Running the VarroaPop program**

As with any Windows program, you can run VarroaPop from the Start button or from Windows Explorer.  

To run from the Start button, press Start->Run and Browse to the folder containing VarroaPop.exe.  Select VarroaPop.exe in the Browser dialog box and press Open.  This will return you Run dialog box where you will press OK.  VarroaPop will start.

To run from Windows Explorer, start Windows Explorer (normally listed when you select Start->Programs).  Navigate to the folder containing VarroaPop.exe.  Double-click on VarroaPop.exe and VarroaPop will start.

Alternately, you can place a shortcut to VarroaPop on your desktop.  This can be done by first navigating to the VarroaPop program with Windows Explorer in the same way as described above.  If you drag the VarroaPop icon from the Explorer window and drop it on your desktop, Windows will create a shortcut for you.  From that point on, you can run VarroaPop by double-clicking the shortcut.


### **Using VarroaPop**

**1.  Overview**

The basic concept of VarroaPop is to take some initial conditions for a honeybee colony, combine that with some weather data, then step day-by-day over a specific simulation period and monitor the colony’s population changes.  A key notion is the VarroaPop *session* which is comprised of the weather data and the colony initial conditions.

After the simulation has been run, VarroaPop generates two types of results information.  First, a graphical result is displayed on the screen.  This graphical result can simultaneously show the value of multiple user-selected parameters plotted over time.  The other result information consists of tabular data which is shown on the VarroaPop main window.  The tabular results can be saved to a file for later use and it can be printed.

As a comment on notation, when this document is referring to a menu selection, the notation “File->Save->Results” should be taken to mean, press the “File” menu selection, then press the “Save” selection on the resulting menu, and finally, press the “Results” selection on the next menu.



2. **Details**

1. **Initial Screen**

When VarroaPop first starts, the iniital screen is displayed:

No session is loaded yet so no simulation can be performed.  The first step is to either create a simulation from scratch or load an existing simulation.  

1. **Starting a VarroaPop Session**

The easiest thing to do is to load an existing simulation.  That can be done from the menu by selecting File->Open.  At that point, a file open dialog is displayed and you can select an existing VarroaPop session file.  The same result is also obtained by pressing the File Open button on the toolbar.

A new session can be created by selecting File->New.  This will display a blank Colony Initial Conditions screen which can be filled out as desired (more about that later).  Again, the same result can be obtained by pressing the New button on the toolbar.

1. **Running a Simulation**

Once a session is loaded, the simulation start and stop dates on the right side of the screen become active and are initially set to the beginning and ending dates in the weather data.  You can change the simulation start and stop times to be any timespan of interest within the limits of the weather data dates.  Once you have selected the simulation dates of interest, pressing the Run button (found on the lower right side of the main screen) begins the simulation.  When the simulation is complete, the results are displayed graphically and in tabular form.  You may print and/or save the tabular results.

The simulation may be re-run at any time by just pressing the Run button again.  Rather than getting the same results over and over, however, you will usually want to make some changes in your session between successive simulations.

1. **Modifying the Session**

There are many variables you can modify before running a simulation.  These include initial conditions, weather data, graphical display parameters, and immigration profiles.  All these modifications are stored when you save the session.

## **Modifying Initial Conditions and Weather Data**

Initial conditions and weather data are modified using the Model Parameters dialog box.  This display is obtained by selecting View->Model Parameters or by pressing the Model Parameters toggle button on the toolbar.  This button looks like a honeycomb.

The Colony Name field will be blank on a new session and will contain the name of the current colony data for an existing session.  Pressing the down arrow next to the Colony Name field will display all the existing colony files in the current folder.

You can enter initial conditions for Worker and Drone populations in each of the life stages.  You can also enter initial conditions for Varroa mite infestation percentages, mite reproductive rate and survivorship.  You can also set the forager lifespan as well as parameters relating to the amount of sperm in the queen’s spermacetha and the maximum eggs layed per day.  In order for any changes made to take effect, you must press the Apply button.

You can also modify the weather data you are using by pressing the Weather tab.  This will display a field identifying the current weather file (blank if this is a new session).  Similar to the Colony Name field, pressing the down arrow next to the weather file name field will display all weather files in the folder.  You must press the Select button to actually choose a weather file.  

The weather files are text files that can be read by any application that can load text (WordPad, Write, MS Word, etc.)  If you need to actually change the data in a weather file, you can load the file into an application, edit it, and re-save it as a text file.

## **Modifying Parameters to Graph**

You can graphically display all important parameters resulting from the simulation.  In order to allow you to focus on just the data you want, a Graph Selection dialog is displayed by either selecting View->Plot Data Selection or by pressing the Plot Data Selection button on the toolbar. 

You can select as many of these parameters as you would like, however with more than four or five, the graph gets quite busy.  Also note that the Proportion of Infested Brood values will always be between 0 and 1 while parameters such as Colony Size can get very large.  Therefore, you won’t see much interesting if you display Proportion parameters along with population counts. 


## **Modifying Immigration Profiles**

You can enable or disable immigration by making the proper selection on the main screen.  To change or set up the immigration profiles, press the Setup button in the Immigration block on the main screen.  When you do this, the following Immigration Setup screen is displayed:

This screen allows you to select the immigration distribution from six predefined distributions by clicking on the one desired.  You can enter how many mites will immigrate.  Finally, you can select the Immigration Start and End dates.  These dates must lie within the simulation timespan.  When you are finished, select OK.


