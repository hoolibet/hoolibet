
Debian
====================
This directory contains files used to package hoolibetd/hoolibet-qt
for Debian-based Linux systems. If you compile hoolibetd/hoolibet-qt yourself, there are some useful files here.

## hoolibet: URI support ##


hoolibet-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install hoolibet-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your hoolibetqt binary to `/usr/bin`
and the `../../share/pixmaps/hoolibet128.png` to `/usr/share/pixmaps`

hoolibet-qt.protocol (KDE)

