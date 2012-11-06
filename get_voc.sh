#!/bin/sh
echo "Use get_voc.sh server filename_without_txt ;)"
wget http://$1/voc/$2.txt
mv $2.txt.1 $2.txt
