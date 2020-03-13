VarroaPop - Varroa Mite Population Model

VarroaPop is a honeybee colony population model that incorporates the effects of a varroa mite infestation.  VarroaPop allows the user to evaluate the effects of a variety of initial conditions and weather on the population growth of a Varroa mites and honeybees.  It also allows the user to simulate the effects of varroa immigration into an existing colony.

*********************************************************************
New Features in Version 2.3

Fixed file location issues.  Adapted to more common file browsing method.  
Fixed problem with multi-treatment of chemicals


*********************************************************************
New Features in Version 2.2

Increased the number of actions that can be simulated 
Fixed bug that over committed the graphics resources - used to crash if many (>50) charts were displayed over the life of a session

*********************************************************************
New Features in Version 2.1

Added command-line control capabilities.  See VarroaPop Help for details


*********************************************************************
New Features in Version 2.0

Re-Queening feature
	Now you can re-queen the colony to more accurately perform a multi-year simulation.  Re-queening can either be automatic based on colony parameters or tied to specific dates.


Mite Treatment
	You can now simulate the treatment of a colony with Varroa pesticides.  

Miscellaneous
	Added capability to toggle betweeen weekly and daily results display and storage.  This helps if you are looking at multi-year data but don't want to generate reams of paper.  Also, the "Weekly" selection results in faster simulation runs.
	
	Now each output graph remains on the screen until the user closes it.  This facilitates making several runs with varied initial conditons and graphically comparing the results.


*********************************************************************



Software Information

1. System Requirements

In order to use VarroaPop, you must have Microsoft Windows 95, Windows NT 4.0 , or later versions installed on your computer.  The VarroaPop software will consume approximately 1 MB of memory on your hard drive.

2. VarroaPop Files

After installing VarroaPop, you will have the following files in the installation directory (e.g. C:\VarroaPop):

VarroaPop.exe - the VarroaPop program
Midwest.wth - a weather file containing 1 year of Midwestern US weather date
Southwest.wth - a weather file containing 1 year of Southwestern US weather data
Midwest5yr.wth - a weather file containing five years of Midwestern US weather data
Sthwest5yr.wth - a weather file containing five years of Southwestern US weather data
Example.col - an example colony file containing a set of initial conditions.
Default.vrp - a sample VarropPop session file.
Readme.txt - this file

3. Running the VarroaPop program

As with any Windows program, you can run VarroaPop from the Start button or from Windows Explorer.  

To run from the Start button, press Start->Programs and scroll down to and select the VarroaPop folder.  When you select this, you will see the VarroaPop icon.  Double-click on this and VarroaPop will begin.

 
To run from Windows Explorer, start Windows Explorer (normally listed when you select Start->Programs).  Navigate to the folder containing VarroaPop.exe.  Double-click on VarroaPop.exe and VarroaPop will start.

Alternately, you can place a shortcut to VarroaPop on your desktop.  This can be done by first navigating to the VarroaPop program with Windows Explorer in the same way as described above.  If you drag the VarroaPop icon from the Explorer window and drop it on your desktop, Windows will create a shortcut for you.  From that point on, you can run VarroaPop by double-clicking the shortcut.


Using VarroaPop

1.  Overview

The basic concept of VarroaPop is to take some initial conditions for a honeybee colony, combine that with some weather data, then step day-by-day over a specific simulation period and monitor the colony's population changes.  As you read this file, we will refer to a VarroaPop session.  A session is comprised of the weather data and the colony initial conditions.

After the simulation has been run, VarroaPop displays the results two ways.  First, a graphical result is displayed on the screen.  This graphical result can simultaneously show the value of multiple user-selected parameters plotted over time.  The other result information consists of tabular data that is shown on the VarroaPop main window.  The tabular and graphical results can be saved to a file for later use and it can be printed.

As a comment on notation, when we refer to a menu selection, the notation "File->Save->Results" means, press the "File" menu selection, then press the "Save" selection on the resulting menu, and finally, press the "Results" selection on the next menu.


2. Details

2.1 Initial Screen

When VarroaPop first starts, the initial screen is displayed:

No session is loaded yet so no simulation can be performed.  The first step is to either create a simulation from scratch or load an existing simulation.  

2.2 Starting a VarroaPop Session

The easiest thing to do is to load an existing simulation.  That can be done from the menu by selecting File->Open.  At that point, a file open dialog is displayed and you can select an existing VarroaPop session file.  First time users should choose the DEFAULT.VRP file by navigating to the installation directory.  Files can also be opened by pressing the File Open button on the toolbar.

To see the initial conditions that were just loaded with the file, select View->Model Parameters.  You will then see the colony name and the initial numbers of workers and drones at all life stages.  The number of days a worker can forage is displayed and can be controlled by sliding the small rectangle horizontally.  Information about the queen is also displayed.  You can change any of these values but you must press the OK button for the changes to take effect.

A new session can be created by selecting File->New.  This will display a blank Colony Initial Conditions screen that can be filled out as desired (more about that later).  Again, the same result can be obtained by pressing the New button on the toolbar.

2.3 Running a Simulation

Once a session is loaded, the simulation start and stop dates on the right side of the screen become active and are initially set to the beginning and ending dates in the weather data.  You can change the simulation start and stop times to be any timespan of interest within the limits of the weather data dates.  Once you have selected the Simulation Start and Stop dates (e.g. Start 01/01/1999, Stop 12/31/2001) press the Run button (found on the lower right side of the main screen) to begin the simulation.  When the simulation is complete, the results are displayed graphically.  To better see the tabular form press OK on the graphic screen.  You may print and/or save the tabular results.

The simulation may be re-run at any time by just pressing the Run button again.  Rather than getting the same results over and over, however, you will usually want to make some changes in your session between successive simulations.

2.4 Modifying the Session

There are many variables you can modify before running a simulation.  These include initial conditions, weather data, graphical display parameters, and immigration profiles.  All these modifications are stored when you save the session.

Modifying Initial Conditions and Weather Data

Initial conditions and weather data are modified using the Model Parameters dialog box.  This display is obtained by selecting View->Model Parameters or by pressing the Model Parameters toggle button on the toolbar.  This button looks like a honeycomb.

The Colony Name field (at the top of the screen) will be blank on a new session.  The Colony Name field will contain the name of the current colony data for an existing session.  Pressing the down arrow next to the Colony Name field will display all the existing colony files in the current folder.

You can enter initial conditions for Worker and Drone populations in each of the life stages.  You can also enter initial conditions for Varroa mite infestation percentages, mite reproductive rate and survivorship.  You can also set the forager lifespan as well as parameters relating to the amount of sperm in the queen's spermacetha and the maximum potential number of eggs the queen can lay per day.  In order for any changes made to take effect, you must press the Apply button.

You can also modify the weather data you are using by pressing the Weather tab.  This will display a field identifying the current weather file (blank if this is a new session).  Similar to the Colony Name field, pressing the down arrow next to the weather file name field will display all weather files in the folder.  You must press the Select button to actually choose a weather file.  

The weather files are text files that can be read by any application that can load text (WordPad, Write, MS Word, etc.)  If you need to actually change the data in a weather file, you can load the file into an application, edit it, and re-save it as a text file but be sure the file extension is ".wth".

To save the changes made to the Session, select File->Save->Save Session.  If you want to save the changes to a file with a different name, select File->SaveAs and enter the filename you would like to use.

Modifying Parameters to Graph

You can graphically display all important parameters resulting from the simulation.  In order to allow you to focus on just the data you want, a Graph Selection dialog is displayed by either selecting View->Plot Data Selection or by pressing the Plot Data Selection button on the toolbar. 

You can select as many of these parameters as you would like, however with more than four or five, individual lines are difficult to discern.  Select the parameter to be included in the graph by placing the pointer n the box next to the parameter and pressing the left mouse button.  Also note that the Proportion of Infested Brood values will always be between 0 and 1 while parameters such as Colony Size can get very large.  Therefore, you won't see the Proportion parameters if you plot them with population counts. 


Modifying Immigration Profiles

You can enable or disable immigration of adult Varroa mites by making the proper selection on the main screen.  To change or set up the immigration profiles, press the Setup button in the Immigration block on the main screen.  The Immigration block is above the simulation block on the right side of the screen.  To set up an immigration event, click on Setup.  You will see a screen that allows you to select the immigration distribution from six predefined distributions by clicking on the one desired.  You can define how many mites will immigrate by entering a number in the Number of Mites Immigrating box on the left side of the screen.  Finally, you can select the Immigration Start and End dates on the right side of the screen.  These dates must lie within the simulation timespan.  When you are finished, select OK.

Saving Simulation Results

To save simulation results in tabular form, choose File->Save->Simulation Results and enter the name for the file.



You are now ready to run a VarroaPop simulation.  You can obtain additional information any time during the operation of the program by select Help from the main menu.


