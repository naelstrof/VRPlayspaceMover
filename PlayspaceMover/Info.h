#pragma once

namespace info
{
	const char* version = "v0.1.9-terri-b";

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
    $ # Moves the playspace with ONLY A/X on Oculus.
    $ PlayspaceMover -l 128 -r 128

Button Mappings
  We take as integers as a button mask, but they actually represent a bitmask.
  You'll have to exercise your CompSci brain to generate these masks. Each
  button is represented by a bit in a 32bit integer. Bit number 7 (1000000)
  would be 2^7, which is 128 as an integer. Button number 7 also happens to
  be the A and X buttons on the Oculus controllers. Therefore setting either
  button mask to `128` would make it so only the A or X button activated...
  Similarly, you can combine bits, so if you wanted button 2 and button 7
  to work with it, you could pass in `130` (2^2 + 2^7), then either would
  work!
  Below is a list of some known button masks (The mask is what you supply!).
    Oculus Masks    Button   Bit   Mask
                      A/X      7     128
                      B/Y      1     2

    Vive Masks      Button   Bit   Mask
                      Menu     1     2
                      Grip     2     4

Tips
    * Restarting the app resets your playspace!
    * VR Input Emulator has a log file that can be dozens of gigabytes if
you're on Oculus, it's in your SteamVR folder under drivers. Set it to
read-only to keep it from growing indefinitely.

)VRPSM0";
}