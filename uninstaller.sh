#!/bin/bash

USER=$(whoami)
DESTINATION="/home/$USER/Documents/Shape-Shooter/"
desktop_file="/home/$USER/.local/share/applications/shpshoot.desktop"
exitcode=0

# Remove the destination directory and its contents
if [ -d "$DESTINATION" ]; then
    echo "Removing $DESTINATION..."
    rm -rf "$DESTINATION"
    if [ $? -eq 0 ]; then
        echo "Successfully removed $DESTINATION."
    else
        echo "Warning: Failed to remove $DESTINATION."
        exitcode=1
    fi
else
    echo "No installation found in $DESTINATION."
fi

# Remove the .desktop file
if [ -f "$desktop_file" ]; then
    echo "Removing $desktop_file..."
    rm "$desktop_file"
    if [ $? -eq 0 ]; then
        echo "Successfully removed $desktop_file."
    else
        echo "Warning: Failed to remove $desktop_file."
        exitcode=2
    fi
else
    echo "No desktop entry found at $desktop_file."
fi

echo "Uninstallation complete."

exit $exitcode
