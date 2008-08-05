UPDATES
The latest version of this software can always be found at:
http://putty-launchy-plugin.googlecode.com/


USAGE
Version 2.0+ of PuTTY Plugin for Launchy will work only with v2.0+ of Launchy.

If PuTTY.exe is not in your system PATH, you will need to set the path to PuTTY 
in the options dialog.  To access the options, right click on the Launchy window 
and click "Options".  Next go to the "Plugins" tab and select "PuTTY".  You can 
customize and enable/disable many features of this plugin here.

To begin completion, type "ssh" or "putty" followed by tab or space. The plugin 
will list all of your PuTTY sessions.  After you select a session in Launchy, it 
will be launched in PuTTY.

If you want to use this plugin in portable mode you will first need PuTTY Tray 
or portaPuTTY in order to save sessions to a filesystem.  Next set the "Path to 
PuTTY" in the plugin options dialog to the RELATIVE path from the Launchy 
directory to the PuTTY executable.  Finally, enable loading sessions from the 
filesystem in the plugin options dialog.


CHANGELOG
v2.2
    New icon in PNG format for compatibility with Launchy 2.1
    PuTTY configuration dialog now starts when no session is specified

v2.1
    Added option to run PuTTY maximized
    Added loading sessions from filesystem
    Added error message when putty.exe can't be found

v2.0
    Now supports Launchy v2.0 (complete rewrite)
    Added configurable options for just about everything
    Trigger words can be changed (defaults to "ssh" and "putty")
    Sessions can be added directly to index, with no trigger word required

v1.3
    Parameters not matching a session name will now be treated as a host name
    Session names containing quote marks now work

v1.2:
    Removed dependency on VC++ Runtime and .NET CLR
    Unsupported plugin for x64 systems

v1.1:
    Added options dialog to set PATH to PuTTY.
    Added icon.
v1.0:
    Initial Release


KNOWN BUGS
See http://code.google.com/p/putty-launchy-plugin/issues/list


CREDITS
Author:
    Matt Cowell
Portions of this code are part of the Launchy project:
    http://www.launchy.net
Portions of this code originated from the PuTTY project:
    http://www.chiark.greenend.org.uk/~sgtatham/putty/
The icon is used from the PuTTY Portable project:
	http://portableapps.com/apps/internet/putty_portable
