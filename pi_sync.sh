#!/bin/bash

# -- Config --
echo "Reading configuration..."
source config

# -- Run Sync --
echo "Sync in progress..."
rsync -avz -v --stats --progress --delete ./robot_agent rescue@$ROBOT_IP:/home/rescue/TDDD07/LabGroup$LAB_GROUP
echo "Sync done!"
ssh rescue@$ROBOT_IP "make -C /home/rescue/TDDD07/LabGroup$LAB_GROUP/robot_agent"
echo "Compilation Done!"
