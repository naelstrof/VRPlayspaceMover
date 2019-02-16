#pragma once

namespace info
{

// Version info
const char * version = "v0.1.9";

// Binding system info
const char * notify_legacy = "";

// Text to print when --help is called
const char * help_text = R"VRPSM0(
Copyright (C) 2017 Dalton Nell, PlayspaceMover Contributors
(https://github.com/naelstrof/VRPlayspaceMover/graphs/contributors)
Usage: VRPlayspaceMover [options]

PlayspaceMover is an application that queries for a button press from
VR devices and moves the playspace based on it.

-h, --help                    Print help and exit
-v, --version                 Print version and exit
Options
  -p, --physics               Enables physics.
  --noGround                  Disables ground collisions when physics is enabled.
  --fakeTrackers              Spawns some fake full body trackers that follow you.
  --orbitTracker              Fake trackers move relative to your HMD rotation.

  --moveBinding=BINDING-STRING
							  Bind to trigger the playspace grab
  --togglePhysicsBinding=BINDING-STRING       
							  Bind that toggles physics
  --resetBinding=BINDING-STRING             
							  Bind to reset playspace

  -g, --gravity=FLOAT
                              Sets how much gravity pulls down on you. Setting
                              this variable will also enable physics.
  -f, --friction=FLOAT
                              Sets how much friction slows you down when you're
                              on the ground. Higher friction means sharper stops.
                              Setting this variable will also enable physics.
  --airFriction=FLOAT
                              Sets how much friction slows you down when you're
                              in the air. Higher friction means sharper stops.
                              Setting this variable will also enable physics.
  -j, --jumpMultiplier=FLOAT
                              Multiplies the force applied to you when you throw
                              yourself. A good value would be around 0 and 100.
                              Setting this variable will also enable physics.
  --bodyHeight=FLOAT
                              Sets the height of your body for fake trackers.
                              Fake trackers are enabled and spawned at your feet
                              and in the middle of your body (hopefully your hips.)
Examples
    $ # Moves the playspace with A OR X on Oculus.
    $ PlayspaceMover --moveBinding "A, X"
    
    $ # Resets the playspace when the left grip and right trigger are pressed on Vive
    $ PlayspaceMover --resetBinding "left: grip & right: trigger"

Button Aliases
  Oculus		Vive
  A / X			Menu
  B / Y			System
  Grip			Grip
  Trigger		Trigger
  Joystick		Touchpad

Tips
    * Restarting the app resets your playspace!
    * VR Input Emulator has a log file that can be dozens of gigabytes if
	    you're on Oculus, it's in your SteamVR folder under drivers. Set it to
	    read-only to keep it from growing indefinitely.

)VRPSM0";
}