
# Playspace Mover

Lets you drag around your playspace by gripping certain buttons. This allows you to "climb" around certain games, and allows you to adjust your floor level on the fly.
This is compatible with KinectToVR, and can run in tandem with any SteamVR app.

If you want to help support this kind of development, feel free to support me through google wallet (naelstrof@gmail.com), paypal (https://paypal.me/naelstrof), or through my [Digital Tip Jar](https://digitaltipjar.com/naelstrof)!

## Installation

* Requires VR Input Emulator, which an easy installer can be found [here](https://github.com/matzman666/OpenVR-InputEmulator/releases).
* Grab a PlayspaceMover executable from the [releases page](https://github.com/naelstrof/VRPlayspaceMover/releases).
* Extract anywhere!

## Usage

* While SteamVR is running, run PlayspaceMover.exe. Make sure it says `Ready!`.
* In VR, hold down either X/Y or the Vive menu buttons to drag your playspace!

# Configuration (Advanced!)

PlayspaceMover can be configured to use different kinds of bindings, currently it only supports button presses. You can change the default buttons by setting bindings in the options. This can be done through either a shortcut, or a commandline.

I find the easiest way is to experiment using the command line, then set up a permanent config with a shortcut!

Open a command line in the folder with PlayspaceMover.exe like so:

![GIF Showing how to open cmd](https://i.imgur.com/jgifVnJ.gif)

## Bindings

The bindings for the different toggles are set up using short input strings. You specify `Left:` , `Right:` or `Both: ` (both is enabled by default) to define which controller, followed by a comma separated list of button aliases. Examples:

Move playspace when either X or A is held on oculus:
```cpp
--moveBinding "Left: X, Right: A"
``` 
Reset playspace when both joysticks are clicked on oculus:
```cpp
--resetBinding "& Joystick"
``` 

Toggle physics when the Grip button, and the right trigger are pressed on vive:
```cpp
--togglePhysicsBinding "Left: Grip & Right: Trigger"
```

### Binding Commands

These are the currently bindable actions using the binding system.

| Option                 | Description                          |
|------------------------|--------------------------------------|
| --moveBinding          | Moves the playspace                  |
| --togglePhysicsBinding | Turns physics on and off             |
| --resetBinding         | Resets the plasypace back to default |

#### Button aliases

These are the currently supported aliases for the controllers.

| Oculus                 | Vive                                 |
|------------------------|--------------------------------------|
| A/X                    | Menu                                 |
| B/Y                    | System                               |
| Grip                   | Grip                                 |
| Trigger                | Trigger                              |
| Joystick               | Touchpad                             |

###### *Vive button names may need clarification

#### Modifiers

Modifiers go at any position within the input string, and specify that any button after that character will have the trait of that modifier. If none are specified, all buttons are treated as being in | (OR) mode.

| Modifier               | Effect                                                          |
|------------------------|-----------------------------------------------------------------|
| &#124; (default)       | Any of the following buttons can activate                       |
| &                      | All of the following buttons need to be pressed as well as previous conditions being met                    |

#### Physics

PlayspaceMover supports some basic physics!

Here's a list of some of the options, their defaults, and what they do.

| Option　　　　　　　　　　　| Default | Description                                                                                                                 |
|--------------------------|---------|-----------------------------------------------------------------------------------------------------------------------------|
| --physics                | N/A     | If specified, enables physics. Physics will automatically be enabled anyway if you specify any of the options below though. |
| --gravity                | 9.81    | How much acceleration is applied to your y velocity every frame in meters per second.                                       |
| --friction               | 8       | How much force is applied opposing your velocity while on the ground.                                                       |
| --airFriction            | 0       | How much force is applied opposing your velocity while in the air.                                                          |
| --jumpMultiplier         | 80      | How much force is applied when you try throwing yourself.                                                                   |
| --noGround               | N/A     | If specified, disables ground collision (You probably should have gravity set to 0 if you use this tag.)                    |
| --leftTogglePhysicsMask  | 0       | Specifies which buttons you have to hold simultaneously on the left controller to toggle physics.                           |
| --rightTogglePhysicsMask | 0       | Specifies which buttons you have to hold simultaneously on the right controller to toggle physics.                          |
| --physicsStartOff | N/A | If you don't want physics to be automatically enabled, pass this flag |

#### Fake Full Body Tracking

If your game requires full body tracking, you can make PlayspaceMover spawn some fake trackers! The following options enable/configure 
this feature.

| Options　　　　　　　| Default | Description                                                                                                                                           
|--------------------|---------|------------
| --fakeTrackers | N/A     | Enables and spawns fake trackers that follow your HMD around.                                                                                         
| --orbitTracker | N/A     | Alternative positioning scheme for the trackers                                                                                                       
| --bodyHeight   | 2       | Sets your body height in meters, it uses this to calculate where to place your feet and hip trackers. This also automatically enables --fakeTrackers. |

### Permanent Config

To make this change "permanent", you can create a new shortcut by right-click dragging it to your desktop.

![GIF showing how to make a shortcut](https://i.imgur.com/zPM27WN.gif)

Then edit it's properties by right-clicking on it, and hitting properties.
Then append your desired button mask parameters to the "Target:" section. (Don't override it!)

![Image of shortcut properties](https://i.imgur.com/rUSzA8l.png)

Done! Double click on that shortcut to launch your configured playspace mover!

Try `./PlayspaceMover.exe --help` for more help on bit-masks and other stuff!


## Tips

* VR Input emulator likes to keep HUGE logs if you're on oculus (18+GB), set `X:/Steam/steamapps/common/SteamVR/drivers/00vrinputemulator/bin/win64/driver_vrinputemulator.log` to read-only to prevent this!
* Restarting the application by closing it or pressing Ctrl+C will reset your playspace.

## Compiling Instructions (Developers Only)

### Download Third-Party Libraries

First we have to collect all the libraries that can't be grabbed using nuget.

1. Git-clone and extract VRPlayspaceMover's source code. (Download Zip on the top right, or use a git client to clone.)
2. Download and extract glm-0.9.9-a2 from [here](https://github.com/g-truc/glm/releases/tag/0.9.9-a2) to `PlayspaceMover/third-party`.
3. Download and extract the latest OpenVR zip from [here](https://github.com/ValveSoftware/openvr/releases) to `PlayspaceMover/third-party`.
4. Download and extract the latest OpenVR-InputEmulator source code zip from [here](https://github.com/matzman666/OpenVR-InputEmulator/releases) to `PlayspaceMover/third-party`.

### Compile Third-Party Dependencies

1. Ensure you have Visual Studio, and the Windows 10 SDK installed. You can find them [here](https://www.visualstudio.com/downloads/) and [here](https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk) respectively.
2. Open `PlayspaceMover/third-party/OpenVR-InputEmulator-1.3/VRInputEmulator.sln` with whichever Visual Studio you have.
3. You should get a *Retarget Solution* window where you can retarget the solution to the v141 toolset.
4. Install `boost`, and `boost-vc141` with nuget. (Right-click on the project and press *Manage nuget packages.*)
5. In lib_vrinputemulator's properties, add `PlayspaceMover/third-party/openvr/headers` as an include folder.
6. Right-click on lib_vrinputemulator, and press *Build*. You should now have a libvrinputemulator.lib.
7. Rename that library to libvrinputemulator-d.lib, then compile a release version too by changing into Release mode.
![Release Mode Image](https://i.imgur.com/QhnLzfu.png)

### Configure VRPlayspaceMover Until It Compiles!

1. Open `PlayspaceMover/PlayspaceMover.sln` with Visual Studio.
2. Install `boost`, and `boost-vc141` with nuget. (Right-click on the project and press *Manage nuget packages.*)
3. Right-click on the VRPlayspaceMover project, and change the include directories to look similar to the image below.
![Directory Images](https://i.imgur.com/T5iciwK.png)
4. Then change the library directories to look similar to the image below.
![Directory Images](https://i.imgur.com/4JHyubs.png)
5. Compile and you're done!
