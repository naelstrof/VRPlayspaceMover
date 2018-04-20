# Playspace Mover

Lets you drag around your playspace by gripping certain buttons. This allows you to "climb" around certain games, and allows you to adjust your floor level on the fly.
This is compatible with KinectToVR, and can run in tandem with any SteamVR app.

## Installation

* Requires VR Input Emulator, which an easy installer can be found [here](https://github.com/matzman666/OpenVR-InputEmulator/releases).
* Grab a PlayspaceMover executable from the releases page found above.
* Extract anywhere and run anytime, it will automatically detect and connect to SteamVR, then it will automatically detect and work with any devices connected.

## Config

To change the buttons that trigger to move your playspace, they can be set with the console parameters -l and -r. They take a bitmask that indicates which button to press. For example `128` is the oculus' A/X button. While `1` is the Vive's menu button. Therefore the command `./PlayspaceMover.exe -l 128 -r 128` would set the only buttons that move the playspace to A and X.

By default it is set to `130` (bit 1, 2, and 7). Which is the X,Y,A,B for Oculus, and the grip and menu buttons for Vive.

## Tips

* VR Input emulator likes to keep HUGE logs if you're on oculus (18+GB), set `X:/Steam/steamapps/common/SteamVR/drivers/00vrinputemulator/bin/win64/driver_vrinputemulator.log` to read-only to prevent this!
