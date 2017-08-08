#!/bin/bash

libDir="/Users/pbeeken/Google Drive/dev/Arduino/PhysBot/PhysBot-Development/Test5_RotoryEncoder"
dstDir="/Users/pbeeken/Google Drive/dev/Arduino/PhysBot/PhysBot-Development"

#echo '{$libDir}'
#echo '{$dstDir}'

# Use rsync
rsync -avruW --include='*/' --include '*.txt'  --exclude '*' "$libDir/lib/" "$dstDir/lib/" # --dry-run
rsync -avruW --include='*/' --include '*.cpp'  --exclude '*' "$libDir/lib/" "$dstDir/lib/" # --dry-run
rsync -avruW --include='*/' --include '*.h'  --exclude '*' "$libDir/lib/" "$dstDir/lib/"   # --dry-run

rsync -avruW --include='*/' --include '*.txt'  --exclude '*' "$dstDir/lib/" "$libDir/lib/" # --dry-run
rsync -avuW --include='*/' --include '*.cpp'  --exclude '*' "$dstDir/lib/" "$libDir/lib/"  # --dry-run
rsync -avuW --include='*/' --include '*.h'  --exclude '*' "$dstDir/lib/" "$libDir/lib/"    # --dry-run

#find "$libDir/" -iname '*.ino' -print0 |  rsync -0 -n --files-from=- "$libDir/" "$dstDir/" --dry-run
