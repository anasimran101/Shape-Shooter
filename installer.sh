#!/bin/bash

USER=$(whoami)
currdir=$(pwd)
exitcode=0


# Check if SFML is installed
if ldconfig -p | grep -q sfml; then
    echo "SFML is already installed."
else
    echo "SFML is not installed. Installing SFML..."
    

    sudo apt update
    sudo apt install libsfml-dev -y
    
    # Confirm installation
    if ldconfig -p | grep -q sfml; then
        echo "SFML has been successfully installed."
    else
        echo "There was an error installing SFML."
        exitcode=1
    fi
fi

# build source files


#check if makefile exists
if [ -f "makefile" ]; then
	make
else
	echo "No make file found"
	exit 2
fi


# Source and destination paths
SourceFiles=("$currdir/dat/config.txt" "$currdir/dat/background.jpg" "$currdir/dat/Arial.ttf" "$currdir/dat/ufo.png" "$currdir/shape-shooter")
DESTINATION="/home/$USER/Documents/Shape-Shooter/"

# Check if the destination directory exists; if not, create it
if [ ! -d "$DESTINATION" ]; then
    mkdir -p "$DESTINATION"
    if [ $? -eq 0 ]; then
        echo "Directory $DESTINATION created successfully."
        
        chmod 755 "$DESTINATION"  # rwxr-xr-x
        if [ $? -eq 0 ]; then
            echo "Permissions for $DESTINATION set to 755 (read, write, execute for the user)."
        else
            echo "Error: Failed to set permissions for $DESTINATION."
            exit 1
        fi
    else
        echo "Error: Failed to create directory $DESTINATION."
        exit 3
    fi
fi

# Loop through each source file and attempt to copy it
for element in "${SourceFiles[@]}"; do
    if [ -f "$element" ]; then
        cp "$element" "$DESTINATION"
        
        # Check if the copy operation was successful
        if [ $? -eq 0 ]; then
            echo "$element copied successfully to $DESTINATION"
        else
            echo "Warning: Failed to copy $element to $DESTINATION"
            exitcode=4
        fi
    else
        echo "Error: Installation failed. $element not found in the current directory."
        exitcode=4
    fi
done


# Create the .desktop file
desktop_file="/home/$USER/.local/share/applications/shpshoot.desktop"
touch "$desktop_file"

if [ $? -ne 0 ]; then
  echo "Error: Failed to create $desktop_file"
  exit 5
fi

echo "created .desktop file"

# Write the .desktop entry
echo -e "[Desktop Entry]\nVersion=1.0\nName=Shape-Shooter\nComment=Shoot em up!\nExec=${DESTINATION}shape-shooter %f\nIcon=${DESTINATION}ufo.png\nTerminal=true\nType=Application\nCategories=Game;Application;" > "$desktop_file"

chmod +x "$desktop_file"

if [ $? -ne 0 ]; then
  echo "Error: Failed to set executable permissions on $desktop_file"
  exit 6
fi

#clean binary
if [ -f "makefile" ]; then
	make clean
else
	echo "No make file found"
	exit 7
fi
exit $exitcode
